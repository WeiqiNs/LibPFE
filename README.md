# RELIC based IPFE Library (LibRIPFE)

[![LibRIPFE CI](https://github.com/WeiqiNs/LibRIPFE/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/WeiqiNs/LibRIPFE/actions/workflows/ci.yml)
[![codecov](https://codecov.io/gh/WeiqiNs/LibRIPFE/graph/badge.svg?token=RQ5Z4BVJ6W)](https://codecov.io/gh/WeiqiNs/LibRIPFE)

**LibRIPFE** is a header-only C++20 implementation of *function-hiding* inner-product functional encryption (IPFE)
schemes. It builds on [LibRBP](https://github.com/WeiqiNs/LibRBP), so every scheme is a template over a LibRBP curve
and runs on any curve LibRBP was built with.

## Supported schemes

| Scheme | API | Highlights | Reference |
| :---: | --- | --- | :---: |
| Kim et al. | `IPFE::KIM` in `RIPFE/ipfe_kim.hpp` | Short ciphertexts, security in the generic group model | [link](https://link.springer.com/chapter/10.1007/978-3-319-45871-7_24) |
| Tomida et al. | `IPFE::TAO` in `RIPFE/ipfe_tao.hpp` | Optimized under standard-model security | [link](https://eprint.iacr.org/2016/440) |
| Ojaswi et al. | `IPFE::OPT` in `RIPFE/ipfe_opt.hpp` | Faster setup, enc, and keygen than Kim et al., with a modest decryption trade-off | [link](https://eprint.iacr.org/2024/1857) |

## Usage

```cpp
#include <RIPFE/ipfe_opt.hpp>

using C = rbp::BLS12_381;
const auto msk = IPFE::OPT::setup<C>(3);
const rbp::DlogTable<C> table(IPFE::OPT::base<C>(), -1000, 1000);
const auto sk = IPFE::OPT::keygen(msk, IPFE::IntVec{1, 2, 3});
const auto ct = IPFE::OPT::enc(msk, IPFE::IntVec{4, -5, 6});
const auto result = IPFE::OPT::dec(table, sk, ct);
```

`dec` returns the inner product, or `std::nullopt` when it falls outside the searched range. Tomida et al. and Ojaswi et
al. decrypt against a fixed base (`msk.base` and `IPFE::OPT::base<C>()`), so build one `rbp::DlogTable` for a range and
reuse it across decryptions. Kim et al. derives its base from each key and ciphertext, so its `dec` takes the bounds.

## Building

LibRIPFE builds on Linux with CMake, a C++20 compiler, GMP (`libgmp-dev`) and git. It uses an installed LibRBP
when it finds one, and otherwise fetches and builds LibRBP from its `main` branch, which `cmake --install` then installs
alongside it.
`-DFETCHCONTENT_SOURCE_DIR_RBP=<path>` builds from a local LibRBP checkout instead.

```bash
cmake -B build -S .
cmake --build build --parallel
ctest --test-dir build --output-on-failure
cmake --install build
```

The tests run every scheme on every curve the installed LibRBP provides. Consumers use
`find_package(RIPFE REQUIRED)` and `target_link_libraries(app PRIVATE RIPFE::RIPFE)`; the [demo](demo) folder is a
complete consumer.

| Option | Default | Effect |
| --- | --- | --- |
| `RIPFE_BUILD_TESTS` | on when top-level | Build the test suite |
| `RIPFE_ENABLE_COVERAGE` | off | Build the tests with `--coverage` |

## Docker

`docker build -t libripfe:dev .` builds LibRIPFE and LibRBP from source, runs the tests, installs both and builds the
demo; `docker run --rm libripfe:dev` runs the demo on every curve.
