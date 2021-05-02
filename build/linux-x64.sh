#!/bin/bash


BUILD_DIR=linux-x64
mkdir -p $BUILD_DIR
cd $BUILD_DIR
cmake ../.. -C ../linux-x64.cache.cmake -DUSE_ASAN=OFF
make -j2
cd ..

