cmake_minimum_required(VERSION 3.15)

project(smallcv)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 11)

add_library(smallcv
    smallcv.h
    smallcv.cpp
)

set(ncnn_DIR "E:/lib/ncnn/20201210/lib/cmake/ncnn" CACHE PATH "folder that contains ncnnConfig.cmake")
find_package(ncnn REQUIRED)

target_link_libraries(smallcv ncnn)

add_executable(ncnn_squeezenet
    squeezenet.cpp
)
target_link_libraries(ncnn_squeezenet smallcv ncnn)

add_executable(ncnn_squeezenet_c_api
    squeezenet_c_api.cpp
)
target_link_libraries(ncnn_squeezenet_c_api smallcv ncnn)

add_executable(ncnn_shufflenetv2
    shufflenetv2.cpp
)
target_link_libraries(ncnn_shufflenetv2 smallcv ncnn)

add_executable(ncnn_simplepose
    simplepose.cpp
)
target_link_libraries(ncnn_simplepose smallcv ncnn)

# TODO
# Mat::clone()