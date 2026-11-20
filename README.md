# Pairing-based Functional Encryption Library (LibPFE)

[![LibPFE CI](https://github.com/WeiqiNs/LibPFE/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/WeiqiNs/LibPFE/actions/workflows/ci.yml)
[![codecov](https://codecov.io/gh/WeiqiNs/LibPFE/graph/badge.svg?token=RQ5Z4BVJ6W)](https://codecov.io/gh/WeiqiNs/LibPFE)

Functional encryption lets the holder of a key learn one function of encrypted data, such as an inner product, and
nothing else. LibPFE is a header-only C++20 library of eight published pairing-based schemes, six function-hiding
inner-product functional encryption (IPFE) schemes and two quadratic functional encryption (QFE) schemes, built on
[LibRBP](https://github.com/WeiqiNs/LibRBP) so every scheme runs on any of its curves (BLS12-381, BN254, SS1536).

```cpp
#include <PFE/ipfe_opt.hpp>

using C = rbp::BLS12_381;
const auto msk = IPFE::OPT::setup<C>(3);
const rbp::DlogTable<C> table(IPFE::OPT::base<C>(), -1000, 1000);
const auto sk = IPFE::OPT::keygen(msk, IPFE::IntVec{1, 2, 3});
const auto ct = IPFE::OPT::enc(msk, IPFE::IntVec{4, -5, 6});
const auto result = IPFE::OPT::dec(table, sk, ct);
```

`result` is 12, the inner product of the two vectors, and the key reveals nothing else about the message.

## What functional encryption is for

In ordinary encryption a key decrypts everything or nothing. In functional encryption the holder of the master key
issues keys for specific functions: a key for the vector y, applied to an encryption of x, yields ⟨x, y⟩ and nothing
else about x. That is enough for weighted sums, statistics or a linear classifier over data the computing party never
sees; quadratic FE extends it to degree-2 polynomials such as xᵀFy, and a function-hiding scheme also hides y, so
neither side learns the other's vector. The inner-product schemes here are secret-key (`enc` takes the master key),
the quadratic schemes public-key, and all rely on bilinear pairings, hence pairing-friendly curves such as BLS12-381.

## Highlights

- **Eight schemes from the literature**, each one header under 150 lines that reads like the paper, with one shared
  test suite run against every scheme on three curves.
- **Fast decryption.** A key can be prepared once so its pairing work is precomputed. With Acharya et al.'s scheme on
  BLS12-381, a prepared key decrypts a length-10 inner product in about 2 ms, and 16 threads sharing the prepared
  keys reach about 3,400 decryptions per second.
- **Thread-safe by construction.** Keys and lookup tables never change after they are built, and a concurrency test
  decrypts from many threads with one shared key.
- **Four implementations.** The same schemes, tests and benchmark inputs exist in
  [GoPFE](https://github.com/WeiqiNs/GoPFE) and [RustPFE](https://github.com/WeiqiNs/RustPFE), and
  [CudaPFE](https://github.com/WeiqiNs/CudaPFE) batches them on NVIDIA GPUs.
- **100% line coverage** in CI, plus a benchmark smoke test and a demo built against the installed package.

## Schemes

Inner-product FE, where a key for y decrypts a ciphertext of x to ⟨x, y⟩:

| Scheme | Header | Function hiding | Security | Paper |
| --- | --- | :---: | --- | --- |
| Bishop et al. | `PFE/ipfe_bjk.hpp` | weak | SXDH | [ASIACRYPT 2015](https://doi.org/10.1007/978-3-662-48797-6_20) |
| Tomida et al. | `PFE/ipfe_tao.hpp` | full | XDLIN | [ISC 2016](https://doi.org/10.1007/978-3-319-45871-7_24) |
| Kim et al. | `PFE/ipfe_kim.hpp` | full | generic group model | [SCN 2018](https://doi.org/10.1007/978-3-319-98113-0_29) |
| Lin | `PFE/ipfe_lin.hpp` | full | SXDH | [CRYPTO 2017](https://doi.org/10.1007/978-3-319-63688-7_20) |
| Kim, Kim and Seo | `PFE/ipfe_kks.hpp` | full | SXDH | [TCS 2019](https://doi.org/10.1016/j.tcs.2019.03.016) |
| Acharya et al. | `PFE/ipfe_opt.hpp` | full | generic group model | [CiC 2025](https://doi.org/10.62056/abe0zo-3y) |

Quadratic FE, where a key for a matrix F decrypts a ciphertext of (x, y) to xᵀFy and anyone with the public key can
encrypt:

| Scheme | Header | Security | Paper |
| --- | --- | --- | --- |
| Baltico et al. | `PFE/qfe_bcfg.hpp` | adaptive, generic group model | [CRYPTO 2017](https://doi.org/10.1007/978-3-319-63688-7_3) |
| Ryffel et al. | `PFE/qfe_sgp.hpp` | generic group model | [NeurIPS 2019](https://proceedings.neurips.cc/paper_files/paper/2019/hash/9d28de8ff9bb6a3fa41fddfdc28f3bc1-Abstract.html) |

Every scheme has `setup`, `keygen`, `enc`, `prepare` and `dec`. `dec` returns `std::nullopt` when the result falls
outside the searched range. Bishop et al. and Kim et al. take the range directly; the others share one
`rbp::DlogTable` across decryptions.

## Build and test

Linux, CMake 3.25+, a C++20 compiler, GMP and git. LibRBP is used if installed and fetched otherwise.

```bash
cmake -B build -S .
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

`-DPFE_BUILD_BENCH=ON` builds `bench/bench.cpp`, which times every scheme on every curve and checks each result. After
`cmake --install build`, consumers use `find_package(PFE REQUIRED)` and link `PFE::PFE`; [demo](demo) is a complete
example.

## The PFE family

LibPFE, GoPFE, RustPFE and CudaPFE implement the same eight inner-product and quadratic functional encryption
schemes. LibPFE, GoPFE and RustPFE share test cases and benchmark inputs, and CudaPFE batches the schemes on CPUs
and NVIDIA GPUs.

| Repository | Language | Pairing backend | Role |
| --- | --- | --- | --- |
| **LibPFE** (this repo) | C++20 | [LibRBP](https://github.com/WeiqiNs/LibRBP) | The eight schemes as header-only templates over any LibRBP curve |
| [GoPFE](https://github.com/WeiqiNs/GoPFE) | Go | [gnark-crypto](https://github.com/Consensys/gnark-crypto) | The eight schemes on BLS12-381, parallel across `GOMAXPROCS` |
| [RustPFE](https://github.com/WeiqiNs/RustPFE) | Rust, Python | [arkworks](https://github.com/arkworks-rs/algebra) | The eight schemes in safe Rust over any arkworks pairing curve, with a PyO3 Python binding |
| [CudaPFE](https://github.com/WeiqiNs/CudaPFE) | C++20, CUDA | Own BLS12-381 arithmetic on [sppark](https://github.com/supranational/sppark) fields | The eight schemes batched on a CPU or NVIDIA GPU engine |
