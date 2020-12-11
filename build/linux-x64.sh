#!/bin/bash


BUILD_DIR=linux-x64
mkdir -p $BUILD_DIR
cd $BUILD_DIR

cmake ../.. -G Ninja -Dncnn_DIR=/home/zz/soft/ncnn/20201208/lib/cmake/ncnn

cd ..

