cmake_minimum_required(VERSION 3.15)

project(smallcv)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 11)

option(SMALLCV_IMSHOW "Provide imshow related functions?" ON)
option(SMALLCV_NCNNEXAMPLES "Build ncnn examples?" ON)

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
    set(glfw3_DIR "~/soft/glfw/lib/cmake/glfw3" CACHE PATH "Directory that contains glfw3Config.cmake")
    find_package(glfw3 QUIET) # 3.3-stable recommended
    if(glfw3_FOUND)
        message(STATUS "[Found glfw3] glfw3_DIR is ${glfw3_DIR}")
    else()
        message(STATUS "[Not Found glfw3]")
        message(STATUS "Please remove cmake build cache, then:")
        message(STATUS "* If already installed glfw3, please set glfw3_DIR in one of:")
        message(STATUS "      file CMakeLists.txt, before `find_package(glfw3)`")
        message(STATUS "      file build/xxx.cache.cmake")
        message(STATUS "      file build/xxx.{sh,cmd}, with -Dglfw3_DIR=/path/to/glfw3Config.cmake")
        message(STATUS "      You may get inspired from build/xxx.cache.cmake :)")
        message(STATUS "* If not installed glfw3, may install binary via:")
        message(STATUS "      sudo apt install libglfw3-dev  # ubuntu")
        message(STATUS "      brew install glfw              # mac")
        message(STATUS "      vcpkg install glfw             # windows")
        message(STATUS "* You may also build and install glfw3 manually, e.g")
        message(STATUS "      git clone https://gitee.com/mirrors/glfw -b 3.3-stable glfw-3.3-stable")
        message(STATUS "      cd glfw-3.3-stable && mkdir build && cd build")
        message(STATUS "      cmake .. -DCMAKE_INSTALL_PREFIX=./install && cmake --build . && cmake --install .")
        message(STATUS "      then set glfw3_DIR before `find_package(glfw3)`")
        message(FATAL_ERROR "")
    endif()
endif()

add_library(smallcv STATIC
    ${smallcv_srcs}
)

target_include_directories(smallcv PRIVATE "deps/stb")
target_include_directories(smallcv PUBLIC "smallcv")
if(SMALLCV_IMSHOW)
		target_link_libraries(smallcv glfw)
		target_include_directories(smallcv PRIVATE "deps/imshow")
		target_compile_definitions(smallcv PUBLIC -DSMALLCV_IMSHOW)
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
	if(SMALLCV_NCNNEXAMPLES)
		find_package(ncnn QUIET) # >= 20200908
		if(ncnn_FOUND)
			message(STATUS "[Found ncnn] ncnn_DIR is ${ncnn_DIR}")
		else()
			message(STATUS "[Not Found ncnn]")
			message(STATUS "Please remove cmake build cache, then:")
			message(STATUS "* If already installed ncnn (version >= 20200908), please set ncnn_DIR in one of:")
			message(STATUS "      file CMakeLists.txt, before `find_package(ncnn)`")
			message(STATUS "      file build/xxx.cache.cmake")
			message(STATUS "      file build/xxx.{sh,cmd}, with -Dncnn_DIR=/path/to/ncnnConfig.cmake")
			message(STATUS "      You may get inspired from build/xxx.cache.cmake :)")
			message(STATUS "* You may also build and install ncnn manually, e.g")
			message(STATUS "      git clone https://gitee.com/mirrors/ncnn -b 20201208 ncnn20201208")
			message(STATUS "      cd ncnn20201208 && mkdir build && cd build")
			message(STATUS "      cmake .. -DCMAKE_INSTALL_PREFIX=./install && cmake --build . && cmake --install .")
			message(STATUS "      then set ncnn_DIR before `find_package(ncnn)`")
			message(FATAL_ERROR "")
		endif()


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
endif()

# TODO
# Mat::clone()
