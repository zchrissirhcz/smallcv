#!/bin/bash


BUILD_DIR=linux-x64
mkdir -p $BUILD_DIR
cd $BUILD_DIR
cmake ../.. \
    -C ../linux-x64.cache.cmake \
    -DCMAKE_BUILD_TYPE=Debug

cmake --build . -j
cd ..

