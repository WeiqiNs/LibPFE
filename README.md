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

Every IPFE scheme also has `prepare(sk)`, which precomputes the key's pairing lines once (LibRBP's `PreparedG2`);
`dec` with the prepared key returns the same result at about two thirds of the cost on BLS12-381 and BN254, so prepare a
key that will decrypt many ciphertexts. A prepared key holds about 20 KB per G2 point on BLS12-381.

## Benchmarks

[`bench/bench.cpp`](bench/bench.cpp) times every scheme on every curve and checks each decryption against the true
result. Build with `-DPFE_BUILD_BENCH=ON` and run `./build/bench/pfe_bench [runs] [lengths...]`; with no arguments
it runs 10 times at n = 10 and n = 100 and prints tables like the ones below for BLS12-381, BN254 and SS1536.

The numbers below are the mean milliseconds per operation on BLS12-381, from a Release build with GCC 15 on an AMD Ryzen
7 9800X3D, with LibRBP's RELIC on its GMP backend. Inputs are random vectors (and matrices) whose results lie in [0,
10000]. Fixed-base schemes reuse one discrete-log table, which takes about 0.4 ms to build and is excluded from Dec;
Bishop et al. and Kim et al. search the range on every decryption. Prepare is the one-time cost of `prepare(sk)`, and
Prepared Dec decrypts with the prepared key; the QFE schemes have no prepared keys.

Inner-product FE, n = 10:

| Scheme | Setup | KeyGen | Enc | Dec | Prepare | Prepared Dec |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| Bishop et al. | 2.42 | 3.30 | 1.20 | 6.21 | 2.31 | 4.23 |
| Tomida et al. | 2.73 | 3.15 | 1.14 | 5.14 | 2.22 | 3.18 |
| Kim et al. | 0.29 | 1.38 | 0.49 | 3.45 | 0.87 | 2.67 |
| Lin | 0.04 | 2.78 | 0.97 | 4.58 | 1.91 | 2.87 |
| Kim, Kim and Seo | 0.07 | 3.49 | 1.24 | 5.68 | 2.44 | 3.50 |
| Ojaswi et al. | 0.07 | 1.75 | 0.63 | 3.13 | 1.22 | 2.04 |

Inner-product FE, n = 100:

| Scheme | Setup | KeyGen | Enc | Dec | Prepare | Prepared Dec |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| Bishop et al. | 1001.43 | 29.38 | 12.60 | 39.92 | 18.61 | 23.36 |
| Tomida et al. | 963.97 | 29.04 | 11.85 | 38.82 | 18.41 | 22.46 |
| Kim et al. | 120.12 | 13.67 | 5.27 | 20.20 | 9.02 | 12.18 |
| Lin | 0.39 | 25.47 | 8.94 | 38.01 | 18.33 | 21.85 |
| Kim, Kim and Seo | 0.62 | 26.24 | 9.22 | 39.34 | 18.47 | 22.44 |
| Ojaswi et al. | 0.25 | 13.11 | 4.61 | 19.88 | 9.23 | 11.50 |

Quadratic FE, n = 10:

| Scheme | Setup | KeyGen | Enc | Dec | Prepare | Prepared Dec |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| Baltico et al. | 1.83 | 1.83 | 6.89 | 6.83 | – | – |
| Dufour-Sans et al. | 1.72 | 0.14 | 8.03 | 5.19 | – | – |

Quadratic FE, n = 100:

| Scheme | Setup | KeyGen | Enc | Dec | Prepare | Prepared Dec |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| Baltico et al. | 17.32 | 19.18 | 61.90 | 65.82 | – | – |
| Dufour-Sans et al. | 17.84 | 1.89 | 69.12 | 51.17 | – | – |

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
