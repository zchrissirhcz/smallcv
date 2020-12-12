#!/bin/bash

BUILD_DIR=Xcode
mkdir -p $BUILD_DIR
cd $BUILD_DIR
cmake ../.. -G "Xcode" -Dncnn_DIR=/Users/chris/work/ncnn/build/install/lib/cmake/ncnn
cd ..
