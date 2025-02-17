#!/bin/sh

if [ -d build ]; then
  rm -rf build
fi

mkdir build

cd build
cmake .. -DCMAKE_C_COMPILER=$(which clang) -DCMAKE_CXX_COMPILER=$(which clang++)
