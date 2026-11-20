# LibPFE

Header-only C++20 library of pairing-based functional encryption: inner-product (IPFE) and quadratic (QFE) schemes,
one header per scheme in `include/PFE/`, each a set of free function templates over a LibRBP curve `C`. LibRBP
(groups, pairings, `rbp::Matrix`/`rbp::Vector`, discrete log) is the only dependency. GoPFE, RustPFE and CudaPFE
implement the same schemes; GoPFE and RustPFE share its test cases, CudaPFE batches the schemes on CPU or GPU.

## Commands

Needs Linux, CMake 3.25+, a C++20 compiler, GMP and git (plus lcov for coverage). GoogleTest is used if installed and
fetched otherwise. LibPFE sets no C++ standard itself: C++20 arrives through `RBP::RBP`. `PFE_BUILD_TESTS` defaults to
ON only when LibPFE is the top-level project.

CI runs these from the repo root. Locally, add `-DFETCHCONTENT_SOURCE_DIR_RBP=$PWD/../LibRBP` to each configure to
build against a sibling LibRBP checkout (absolute; a relative path draws a CMake warning). Without it,
`find_package(RBP)` is tried, then LibRBP `main` is fetched (`FetchContent_Declare(RBP ...)` in `CMakeLists.txt`).

```bash
cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug -DPFE_ENABLE_COVERAGE=ON
cmake --build build --parallel
ctest --test-dir build --output-on-failure
lcov --capture --directory build --output-file all.info --filter brace,blank \
     --ignore-errors mismatch,inconsistent,unused,negative
lcov --extract all.info "$PWD/include/PFE/*" --output-file coverage.info --ignore-errors unused
lcov --summary coverage.info --fail-under-lines 100

cmake -B build -S . -DCMAKE_BUILD_TYPE=Release -DPFE_BUILD_BENCH=ON
cmake --build build --parallel && ctest --test-dir build --output-on-failure
./build/bench/pfe_bench 1 4
sudo cmake --install build && sudo ldconfig
cmake -B demo/build -S demo && cmake --build demo/build --parallel && ./demo/build/demo
```

The two blocks are separate CI jobs on fresh runners. Locally, give the second its own build directory, since `build`
keeps `PFE_ENABLE_COVERAGE=ON` cached. The `lcov` steps need lcov 2.x (for `--filter`). Without sudo, install with
`cmake --install <build> --prefix <dir>` and configure the demo with `-DCMAKE_PREFIX_PATH=<dir>`.
`pfe_bench [runs] [lengths...]` times every scheme on every curve.

## Layout

- `include/PFE/ipfe.hpp` — `IPFE::IntVec`, `to_vector`, and `IPFE::detail` helpers (`OneOf`, `require_dimension`,
  `negated`, `joined`).
- `include/PFE/qfe.hpp` — `QFE::IntMat`, `to_matrix`, and `QFE::detail::masked` and `add_bilinear`.
- `include/PFE/ipfe_<abbr>.hpp`, `qfe_<abbr>.hpp` — one scheme each, in namespace `IPFE::<ABBR>` or `QFE::<ABBR>`;
  `README.md` § Schemes maps each to its paper.
- `test/schemes.hpp` — one adapter struct per scheme (`name`, `setup`, `decryptor`), shared by tests and benchmark.
- `test/scheme_types.hpp` — `InnerProductSchemes`, `QuadraticSchemes`: every adapter on every curve in `CurveTuple`.
- `test/test_ipfe.cpp`, `test/test_qfe.cpp`, `test/test_concurrency.cpp` — GoogleTest typed suites over those lists.
- `bench/bench.cpp` — benchmark of every operation, which throws if any decryption is wrong.
- `demo/` — standalone consumer of the installed package; `cmake/PFEInstall.cmake` and `cmake/PFEConfig.cmake.in`
  export `PFE::PFE` (an INTERFACE target linking `RBP::RBP`). `Dockerfile` builds, tests and installs LibPFE and
  builds the demo, which `docker run` runs.

## Scheme shape and adding a scheme

An IPFE scheme is secret-key: `setup<C>(n)` returns `Msk`, `keygen(msk, IntVec)` and `enc(msk, IntVec)` return `Sk` and
`Ct`. A QFE scheme is public-key: `setup<C>(n)` returns `Keys{pk, msk}`, `keygen(msk, IntMat)`, `enc(pk, x, y)`. Every
scheme has `prepare(sk)`, which turns the key's G2 points into `rbp::PreparedG2` inside a `PreparedSk`, and `dec`
accepting either key, by a `detail::OneOf<Sk<C>, PreparedSk<C>>` template or two overloads. `dec` either takes a shared
`rbp::DlogTable` built over the scheme's `base<C>()` (TAO keeps its base in `Msk::base`), or, for BJK and KIM, a
`lower_bound, upper_bound` range passed to `rbp::dlog`. Tests and bench call `keygen`, `enc`, `prepare` and `dec`
unqualified, so they resolve by ADL: a scheme's structs must live in its own namespace.

A new scheme touches, together: its header; an adapter plus `#include` in `test/schemes.hpp` (`table_decryptor` or
`range_decryptor`); `InnerProductSchemes` or `QuadraticSchemes` in `test/scheme_types.hpp`; the `measure_each` lists in
`bench.cpp` `benchmark`; the README Schemes table; and, for a QFE scheme, the field selector in `test_qfe.cpp`
`RejectsCiphertextsMissingAPoint`. Keep GoPFE (`test/`), RustPFE (`tests/schemes.rs`) and CudaPFE (`test/fe/`) on
the same construction and test cases.

## Invariants

- `dec` returns `std::nullopt` when the result lies outside the searched range; pinned by
  `DecryptsInnerProductsExactlyWithinTheRange` and `DecryptsQuadraticFormsExactlyWithinTheRange`.
- Every bad shape throws `rbp::ShapeError`: `setup(0)` via `detail::require_dimension`, wrong-length IPFE inputs and
  QFE keys via LibRBP arithmetic, wrong-length QFE `enc` inputs via `QFE::detail::masked`, and a QFE ciphertext missing
  a G2 point via `QFE::detail::add_bilinear`. Pinned by `RejectsDimensionsBelowOne`, `RejectsVectorsOfTheWrongLength`,
  `RejectsCiphertextsOfAnotherDimension`, `RejectsInputsOfTheWrongShape` and `RejectsCiphertextsMissingAPoint`.
- Decryption only reads keys, prepared keys and the table, so threads share one of each;
  `test_concurrency.cpp` `ThreadsShareOneKeyAndTable` checks every result for `Sk` and `PreparedSk`. A race that
  leaves the results intact shows only under TSan, which CI does not run.
- Tests cover every curve in LibRBP's `RBP_CURVES` (`test/CMakeLists.txt` generates `curves.hpp` with `CurveTuple`),
  but `bench.cpp` `main` and `demo/` name the curves explicitly and need all of them built.

## CI

`.github/workflows/ci.yml` runs on pushes and PRs to `main` and on manual dispatch; both jobs fetch LibRBP `main`
fresh, so a LibRBP change can turn LibPFE red. `coverage`: Debug build with `PFE_ENABLE_COVERAGE`, tests, 100% line
coverage of `include/PFE/` via lcov, Codecov upload. `package`: Release build with the benchmark, tests,
`pfe_bench 1 4`, install, then the demo built and run against the installed package.
