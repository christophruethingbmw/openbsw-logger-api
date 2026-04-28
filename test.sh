#!/bin/sh

echo "... compiling with gcc"
g++ -O3 -std=c++17 -g main.cpp -o main-gcc && readelf -w main-gcc > main-gcc.dwarf

echo "... compiling with clang"
clang++ -O3 -std=c++17 -g main.cpp -o main-clang && readelf -w main-clang > main-clang.dwarf

echo "... running"
./main-gcc

echo "... extracting dwarf"
grep "DW_AT_name.*: AuthEnd" -B 1 -A 29 main-gcc.dwarf
