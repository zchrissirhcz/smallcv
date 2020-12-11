cmake_minimum_required(VERSION 3.15)

project(smallcv)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 11)

option(SMALLCV_IMSHOW "Provide imshow related functions" ON)

# check memory leak with VLD in Visual Studio
# add_definitions(-DVLD)

set(smallcv_srcs
    smallcv/smallcv.hpp
    smallcv/smallcv.cpp
    smallcv/mat.hpp
    smallcv/fc_log.h
    smallcv/fc_log.c
    smallcv/improc_private.hpp
    smallcv/improc_private.cpp
)

if(SMALLCV_IMSHOW)
    list(APPEND smallcv_srcs
        smallcv/imshow.hpp
        smallcv/imshow.cpp
        deps/imshow/glad/glad.c
    )
    set(glfw3_ROOT "D:/lib/glfw/3.3/x64/vc15/lib/cmake/glfw3")
    find_package(glfw3 REQUIRED)
endif()

add_library(smallcv
    ${smallcv_srcs}
)

target_include_directories(smallcv PRIVATE "deps/stb")
target_include_directories(smallcv PUBLIC "smallcv")
if(SMALLCV_IMSHOW)
    target_link_libraries(smallcv glfw)
    target_include_directories(smallcv PRIVATE "deps/imshow")
    target_compile_definitions(smallcv PUBLIC -DSMALLCV_IMSHOW)
endif()

set(ncnn_DIR "E:/lib/ncnn/20201210/lib/cmake/ncnn" CACHE PATH "folder that contains ncnnConfig.cmake")
find_package(ncnn REQUIRED)

target_link_libraries(smallcv ncnn)

add_executable(ncnn_squeezenet
    examples/ncnn/squeezenet.cpp
)
target_link_libraries(ncnn_squeezenet smallcv ncnn)

add_executable(ncnn_squeezenet_c_api
    examples/ncnn/squeezenet_c_api.cpp
)
target_link_libraries(ncnn_squeezenet_c_api smallcv ncnn)

add_executable(ncnn_shufflenetv2
    examples/ncnn/shufflenetv2.cpp
)
target_link_libraries(ncnn_shufflenetv2 smallcv ncnn)

add_executable(ncnn_simplepose
    examples/ncnn/simplepose.cpp
)
target_link_libraries(ncnn_simplepose smallcv ncnn)

# TODO
# Mat::clone()