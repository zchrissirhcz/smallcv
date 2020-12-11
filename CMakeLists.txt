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

    #----------------------------------------
    # glfw
    #----------------------------------------
    #set(glfw3_ROOT "D:/lib/glfw/3.3/x64/vc15/lib/cmake/glfw3")
    set(glfw3_ROOT "${CMAKE_SOURCE_DIR}/deps/imshow/glfw")
    find_package(glfw3 QUIET)
    if (NOT glfw3_FOUND)
        message(STATUS "--- Didn't find glfw3, will git clone then compile it")

        include(ExternalProject)
        set(glfw3_INSTALL ${glfw3_ROOT})

        ExternalProject_Add(glfw3
            GIT_REPOSITORY https://github.com/glfw/glfw.git
            GIT_TAG "3.3.2"
            CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${glfw3_INSTALL} -DBUILD_SHARED_LIBS=OFF -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF
        )
        set(glfw3_FOUND TRUE)
        set(glfw3_INCLUDE_DIR ${glfw3_INSTALL}/include)
        set(glfw3_EXTERNAL TRUE)
    else()
        message(STATUS "--- Found glfw in ${glfw3_ROOT}")
    endif()
endif()

add_library(smallcv STATIC
    ${smallcv_srcs}
)

target_include_directories(smallcv PRIVATE "deps/stb")
target_include_directories(smallcv PUBLIC "smallcv")

if(SMALLCV_IMSHOW)
    target_include_directories(smallcv PRIVATE "deps/imshow")
    target_compile_definitions(smallcv PUBLIC -DSMALLCV_IMSHOW)

    if (glfw3_EXTERNAL)
        add_dependencies(smallcv glfw3)
    else()
        get_target_property(glfw3_INCLUDE_DIR glfw INTERFACE_INCLUDE_DIRECTORIES)
    endif()
    
    if ( (NOT glfw3_FOUND) OR glfw3_EXTERNAL)
        if (WIN32)
            set(glfw3_LIBRARY "${glfw3_ROOT}/lib/glfw3.lib")
        elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
            set(glfw3_LIBRARY "${glfw3_ROOT}/lib/libglfw3.a")
        endif()
        target_link_libraries(smallcv PUBLIC ${glfw3_LIBRARY})
    else()
        target_link_libraries(smallcv PUBLIC glfw)
    endif()
    target_include_directories(smallcv PUBLIC ${glfw3_INCLUDE_DIR})

    #target_include_directories(smallcv PRIVATE ${glfw_INCLUDE_DIR})
endif()


#----------------------------------------
# basic example
#----------------------------------------
add_executable(basic_example
    examples/example.cpp
)
target_link_libraries(basic_example smallcv)

#----------------------------------------
# ncnn examples
#----------------------------------------
set(ncnn_DIR "E:/lib/ncnn/20201210/lib/cmake/ncnn" CACHE PATH "folder that contains ncnnConfig.cmake")
find_package(ncnn REQUIRED)

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
