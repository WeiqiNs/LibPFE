# RELIC based IPFE Library (LibRIPFE)

[![Github CI](https://github.com/WeiqiNs/LibRIPFE/actions/workflows/ci.yml/badge.svg)](https://github.com/WeiqiNs/LibRIPFE/actions/workflows/ci.yml)
[![codecov](https://codecov.io/gh/WeiqiNs/LibRIPFE/graph/badge.svg?token=RQ5Z4BVJ6W)](https://codecov.io/gh/WeiqiNs/LibRIPFE)

**LibRIPFE** is a modern C++ implementation of *function-hiding* inner-product functional encryption (IPFE) schemes. It builds on the RELIC cryptographic toolkit through the convenient **LibRBP** wrapper. For installation and usage, one need to install RELIC and LibRBP first, see the accompanying [Dockerfile](Dockerfile) for details.

## Supported schemes

|     Scheme     | Highlights                                                                        |                               Reference                                |
|:--------------:|-----------------------------------------------------------------------------------|:----------------------------------------------------------------------:|
|   Kim et al.   | Short ciphertexts, security in the generic group model                            | [link](https://link.springer.com/chapter/10.1007/978-3-319-45871-7_24) |
| Tomida et al.  | Optimized under standard-model security                                           |                [link](https://eprint.iacr.org/2016/440)                |
| Ojaswi et al.  | Faster setup, enc, and keygen than Kim et al., with a modest decryption trade-off |               [link](https://eprint.iacr.org/2024/1857)                |

