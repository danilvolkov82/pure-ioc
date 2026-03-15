#!/bin/bash
cmake -S . -B build-tests -DBUILD_TESTS=ON
cmake --build build-tests
cd build-tests
ctest "$@"
