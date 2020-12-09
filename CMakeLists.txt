cmake_minimum_required(VERSION 3.15)

project(smallcv)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(smallcv
    smallcv.h
    smallcv.cpp
)

set(ncnn_DIR "E:/lib/ncnn/master/lib/cmake/ncnn" CACHE PATH "folder that contains ncnnConfig.cmake")
find_package(ncnn REQUIRED)

target_link_libraries(smallcv ncnn)