#!/bin/sh

echo "Bash: $1/solver"

cd "$1/solver" && mkdir -p build && cd build && cmake .. && make

