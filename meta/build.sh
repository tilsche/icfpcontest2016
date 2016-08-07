#!/bin/sh

echo "Bash: $1/solver"

cd "$1/solver" && mkdir -p build && cd build && cmake -DMIN_LOG_LEVEL=info -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS=-O3 .. && make solver
