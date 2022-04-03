#!/bin/bash

BUILD_DIR=Xcode
mkdir -p $BUILD_DIR
cd $BUILD_DIR
cmake ../.. -G "Xcode"
cd ..
