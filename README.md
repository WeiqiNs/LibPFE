# Pairing-based Functional Encryption Library (LibPFE)

[![LibPFE CI](https://github.com/WeiqiNs/LibPFE/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/WeiqiNs/LibPFE/actions/workflows/ci.yml)
[![codecov](https://codecov.io/gh/WeiqiNs/LibPFE/graph/badge.svg?token=RQ5Z4BVJ6W)](https://codecov.io/gh/WeiqiNs/LibPFE)

**LibPFE** is a header-only C++20 library of pairing-based functional encryption: private-key *function-hiding*
inner-product functional encryption (IPFE) and public-key quadratic functional encryption (QFE). It builds on
[LibRBP](https://github.com/WeiqiNs/LibRBP), so every scheme is a template over a LibRBP curve and runs on any curve
LibRBP was built with.

## Supported schemes

### Inner-product FE

A key for y decrypts a ciphertext of x to the inner product ⟨x, y⟩.

| Scheme | API | Function hiding | Security | Ciphertext | Fixed-base `dec` | Reference |
| --- | --- | :---: | --- | :---: | :---: | --- |
| Bishop et al. | `IPFE::BJK` in `PFE/ipfe_bjk.hpp` | weak | SXDH | 2n + 6 | no | [ASIACRYPT 2015](https://doi.org/10.1007/978-3-662-48797-6_20) |
| Tomida et al. | `IPFE::TAO` in `PFE/ipfe_tao.hpp` | full | XDLIN | 2n + 5 | yes | [ISC 2016](https://doi.org/10.1007/978-3-319-45871-7_24) |
| Kim et al. | `IPFE::KIM` in `PFE/ipfe_kim.hpp` | full | SIM, generic group model | n + 1 | no | [SCN 2018](https://doi.org/10.1007/978-3-319-98113-0_29) |
| Lin | `IPFE::LIN` in `PFE/ipfe_lin.hpp` | full | SXDH | 2n + 2 | yes | [CRYPTO 2017](https://doi.org/10.1007/978-3-319-63688-7_20) |
| Kim, Kim and Seo | `IPFE::KKS` in `PFE/ipfe_kks.hpp` | full | SXDH | 2n + 8 | yes | [TCS 2019](https://doi.org/10.1016/j.tcs.2019.03.016) |
| Ojaswi et al. | `IPFE::OPT` in `PFE/ipfe_opt.hpp` | full | SIM, generic group model | n + 4 | yes | [CiC 2025](https://doi.org/10.62056/abe0zo-3y) |

For vectors of length n, a ciphertext has the listed number of G1 elements and a key the same number of G2 elements.
Lin's scheme is the paper's weakly function-hiding scheme, lifted to full function hiding as the paper describes.

### Quadratic FE

A key for an n × n matrix F decrypts a ciphertext of (x, y) to xᵀFy. Anyone with the public key can encrypt.

| Scheme | API | Security | Ciphertext | Key | Reference |
| --- | --- | --- | :---: | :---: | --- |
| Baltico et al. | `QFE::BCFG` in `PFE/qfe_bcfg.hpp` | adaptive, generic group model | 2n G1 + (2n + 2) G2 | (n + 2) G1 + n G2 | [CRYPTO 2017](https://doi.org/10.1007/978-3-319-63688-7_3) |
| Dufour-Sans et al. | `QFE::SGP` in `PFE/qfe_sgp.hpp` | generic group model | (2n + 1) G1 + 2n G2 | 1 G2 | [NeurIPS 2019](https://proceedings.neurips.cc/paper_files/paper/2019/hash/9d28de8ff9bb6a3fa41fddfdc28f3bc1-Abstract.html) |

Both decrypt against a fixed base, and keys also carry F. A Baltico et al. key also carries the products of F with the
master key that decryption would otherwise derive from the public key. Dufour-Sans et al.'s scheme appears in the
NeurIPS paper by Ryffel, Dufour-Sans, Gay, Bach and Pointcheval.

## Usage

```cpp
#include <PFE/ipfe_opt.hpp>

using C = rbp::BLS12_381;
const auto msk = IPFE::OPT::setup<C>(3);
const rbp::DlogTable<C> table(IPFE::OPT::base<C>(), -1000, 1000);
const auto sk = IPFE::OPT::keygen(msk, IPFE::IntVec{1, 2, 3});
const auto ct = IPFE::OPT::enc(msk, IPFE::IntVec{4, -5, 6});
const auto result = IPFE::OPT::dec(table, sk, ct);
const auto prepared = IPFE::OPT::prepare(sk);
const auto same_result = IPFE::OPT::dec(table, prepared, ct);
```

```cpp
#include <PFE/qfe_sgp.hpp>

const auto keys = QFE::SGP::setup<C>(2);
const rbp::DlogTable<C> table(QFE::SGP::base<C>(), -1000, 1000);
const auto sk = QFE::SGP::keygen(keys.msk, QFE::IntMat{{1, 2}, {0, -1}});
const auto ct = QFE::SGP::enc(keys.pk, QFE::IntVec{3, 4}, QFE::IntVec{5, -6});
const auto result = QFE::SGP::dec(table, sk, ct);
```

`dec` returns the result, or `std::nullopt` when it falls outside the searched range. Schemes with a fixed-base `dec`
decrypt against one base (`msk.base` for Tomida et al., `<SCHEME>::base<C>()` for the others), so build one
`rbp::DlogTable` for a range and reuse it across decryptions. Bishop et al. and Kim et al. derive the base from each key
and ciphertext, so their `dec` takes the bounds instead.

Every scheme also has `prepare(sk)`, which precomputes the key's pairing lines once (LibRBP's `PreparedG2`), and `dec`
with the prepared key returns the same result. An IPFE key is all of its decryption's G2 side, so its prepared `dec`
costs about two thirds of `dec` on BLS12-381 and BN254; prepare a key that will decrypt many ciphertexts. A QFE
decryption also pairs the ciphertext's own G2 points, which no key can prepare, so a prepared Baltico et al. key saves
less, and a prepared Dufour-Sans et al. key, which fixes one G2 point, decrypts in about the time of the plain key. A
prepared key holds about 20 KB per G2 point on BLS12-381.

Keys, prepared keys and `rbp::DlogTable` are immutable after construction, so any number of threads may decrypt
different ciphertexts with one shared key and table. Every `dec` builds its own `rbp::PairingProduct`, so nothing
per-call is shared; a prepared key must outlive every `dec` that uses it. `setup`, `keygen` and `enc` may run on any
thread, each drawing from that thread's own LibRBP generator.

## Benchmarks

[`bench/bench.cpp`](bench/bench.cpp) times every scheme on every curve and checks each decryption against the true
result. Build with `-DPFE_BUILD_BENCH=ON` and run `./build/bench/pfe_bench [runs] [lengths...]`; with no arguments
it runs 10 times at n = 10 and n = 100 and prints tables like the ones below for BLS12-381, BN254 and SS1536.

The numbers below are the mean milliseconds per operation on BLS12-381, from a Release build with GCC 15 on an AMD Ryzen
7 9800X3D, with LibRBP's RELIC on its GMP backend. Inputs are random vectors (and matrices) whose results lie in [0,
10000]. Fixed-base schemes reuse one discrete-log table, which takes about 0.4 ms to build and is excluded from Dec;
Bishop et al. and Kim et al. search the range on every decryption. Prepare is the one-time cost of `prepare(sk)`, and
Prepared Dec decrypts with the prepared key. Prepared Dec/s is the throughput, in decryptions per second, of one thread
per hardware thread decrypting the same ciphertexts at once with the shared prepared keys and table; each thread's
first decryption, which sets up its RELIC context, is not timed.

Inner-product FE, n = 10:

| Scheme | Setup | KeyGen | Enc | Dec | Prepare | Prepared Dec | Prepared Dec/s on 16 threads |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| Bishop et al. | 1.89 | 3.68 | 1.32 | 6.81 | 2.61 | 4.49 | 2064 |
| Tomida et al. | 2.32 | 3.57 | 1.26 | 5.72 | 2.49 | 3.49 | 2674 |
| Kim et al. | 0.26 | 1.57 | 0.57 | 3.78 | 1.07 | 2.83 | 3350 |
| Lin | 0.04 | 3.10 | 1.09 | 5.07 | 2.16 | 3.12 | 2946 |
| Kim, Kim and Seo | 0.07 | 3.99 | 1.39 | 6.31 | 2.76 | 3.81 | 2434 |
| Ojaswi et al. | 0.07 | 2.00 | 0.71 | 3.45 | 1.37 | 2.20 | 3398 |

Inner-product FE, n = 100:

| Scheme | Setup | KeyGen | Enc | Dec | Prepare | Prepared Dec | Prepared Dec/s on 16 threads |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| Bishop et al. | 720.80 | 31.56 | 12.93 | 43.27 | 20.43 | 25.01 | 366 |
| Tomida et al. | 721.90 | 30.35 | 11.65 | 42.36 | 20.32 | 24.01 | 382 |
| Kim et al. | 90.30 | 14.73 | 5.68 | 22.07 | 10.10 | 13.13 | 704 |
| Lin | 0.37 | 28.08 | 10.03 | 41.61 | 19.80 | 23.55 | 385 |
| Kim, Kim and Seo | 0.59 | 29.03 | 10.33 | 42.48 | 20.61 | 24.19 | 378 |
| Ojaswi et al. | 0.24 | 14.58 | 5.28 | 21.70 | 10.27 | 12.43 | 721 |

Quadratic FE, n = 10:

| Scheme | Setup | KeyGen | Enc | Dec | Prepare | Prepared Dec | Prepared Dec/s on 16 threads |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| Baltico et al. | 2.05 | 2.00 | 7.76 | 7.53 | 0.97 | 6.63 | 1382 |
| Dufour-Sans et al. | 1.93 | 0.15 | 9.11 | 5.68 | 0.10 | 5.58 | 1654 |

Quadratic FE, n = 100:

| Scheme | Setup | KeyGen | Enc | Dec | Prepare | Prepared Dec | Prepared Dec/s on 16 threads |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| Baltico et al. | 19.18 | 19.93 | 66.84 | 68.79 | 10.08 | 60.05 | 151 |
| Dufour-Sans et al. | 19.18 | 0.71 | 76.30 | 55.16 | 0.31 | 55.14 | 165 |

## Building

LibPFE builds on Linux with CMake, a C++20 compiler, GMP (`libgmp-dev`) and git. It uses an installed LibRBP
when it finds one, and otherwise fetches and builds LibRBP from its `main` branch, which `cmake --install` then installs
alongside it.
`-DFETCHCONTENT_SOURCE_DIR_RBP=<path>` builds from a local LibRBP checkout instead.

```bash
cmake -B build -S .
cmake --build build --parallel
ctest --test-dir build --output-on-failure
cmake --install build
```

The tests run every scheme on every curve LibRBP was built with. Consumers use
`find_package(PFE REQUIRED)` and `target_link_libraries(app PRIVATE PFE::PFE)`; the [demo](demo) folder is a
complete consumer.

| Option | Default | Effect |
| --- | --- | --- |
| `PFE_BUILD_TESTS` | on when top-level | Build the test suite |
| `PFE_ENABLE_COVERAGE` | off | Build the tests with `--coverage` |
| `PFE_BUILD_BENCH` | off | Build the benchmark in `bench/` |

## Docker

`docker build -t libpfe:dev .` builds LibPFE and LibRBP from source, runs the tests, installs both and builds the
demo; `docker run --rm libpfe:dev` runs the demo on every curve.
