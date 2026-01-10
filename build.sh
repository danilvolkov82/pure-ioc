#!/bin/bash
cmake -S . -B build -DBUILD_TESTS=OFF
cmake --build build
