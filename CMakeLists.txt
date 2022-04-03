#------------------------------------------------------------
# setup default install directory
# (must in the beginning)
#------------------------------------------------------------
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Installation Directory")
endif()
message(STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")


cmake_minimum_required(VERSION 3.20)

project(smallcv)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 11)

option(SMALLCV_EXAMPLES "Build ncnn examples?" ON)
option(SMALLCV_EXAMPLES_NCNN "ncnn examples?" OFF)
option(SMALLCV_TEST "build test?" ON)


include("cmake/deps.cmake")
include("cmake/overlook.cmake")
include("cmake/output_dir.cmake")
# include("cmake/asan.cmake")
# include("cmake/tsan.cmake")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build" FORCE)
endif()
message(STATUS ">>> Build Type: ${CMAKE_BUILD_TYPE}")

#----------------------------------------------------------------------
# smallcv library
#----------------------------------------------------------------------

set(smallcv_srcs
  ${CMAKE_SOURCE_DIR}/src/smallcv.cpp
  ${CMAKE_SOURCE_DIR}/src/imgproc.cpp
  ${CMAKE_SOURCE_DIR}/src/imdraw.cpp
)

add_library(smallcv STATIC)

set_target_properties(smallcv PROPERTIES
  DEBUG_POSTFIX
    "_d"
)

target_include_directories(smallcv
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src"
  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/deps/stb"
)

if(NOT CMAKE_CROSSCOMPILING)
  list(APPEND smallcv_srcs
    ${CMAKE_SOURCE_DIR}/src/imshow.cpp
    ${CMAKE_SOURCE_DIR}/deps/imshow/glad/glad.c
  )
  target_link_libraries(smallcv glfw)
  target_include_directories(smallcv PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/deps/imshow")
endif()

target_sources(smallcv PRIVATE ${smallcv_srcs})

if(SMALLCV_EXAMPLES)
  add_subdirectory(examples)
endif()

# Report summary
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/summary.cmake")

set_target_properties(smallcv PROPERTIES PUBLIC_HEADER "src/smallcv.hpp")
install(TARGETS smallcv)


#----------------------------------------------------------------------
# Unit tests
#----------------------------------------------------------------------
if(SMALLCV_TEST)
  enable_testing()
  find_package(GTest REQUIRED)

  macro(smallcv_add_test name)
    add_executable(test_${name} ${CMAKE_SOURCE_DIR}/tests/test_${name}.cpp)
    set(dep_libs smallcv ${ARGN} GTest::gtest GTest::gtest_main)
    if(ANDROID)
      list(APPEND dep_libs log)
    endif()
    target_link_libraries(test_${name} PRIVATE ${dep_libs})
    gtest_add_tests(TARGET test_${name})
  endmacro()

  smallcv_add_test(size)
  smallcv_add_test(loadsave)
endif()
