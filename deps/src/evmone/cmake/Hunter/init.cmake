# evmone: Fast Ethereum Virtual Machine implementation
# Copyright 2018-2019 The evmone Authors.
# Licensed under the Apache License, Version 2.0.

set(HUNTER_CONFIGURATION_TYPES Release CACHE STRING "Build type of Hunter packages")

include(HunterGate)

HunterGate(
    URL "https://github.com/FISCO-BCOS/hunter/archive/da73ad5a35aaee05b9feb77671c744487979c611.tar.gz"
    SHA1 "9bfa9577907f31a2e064a986c423651dd29e2a45"
    LOCAL
)
