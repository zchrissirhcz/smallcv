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

option(SMALLCV_IMAGEIO "Image I/O?" ON)
option(SMALLCV_IMSHOW "Provide imshow related functions?" ON)

option(SMALLCV_USE_ASAN "Use address sanitizer?" OFF)
option(SMALLCV_USE_VLD  "Use VLD for memleak check?" OFF)

option(SMALLCV_EXAMPLES "Build ncnn examples?" ON)
option(SMALLCV_EXAMPLES_NCNN "ncnn examples?" ON)

include("cmake/deps.cmake")
include("cmake/overlook.cmake")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build" FORCE)
endif()
message(STATUS ">>> Build Type: ${CMAKE_BUILD_TYPE}")


#----------------------------------------------------------------------
# version
#----------------------------------------------------------------------
if(NOT DEFINED SMALLCVCV_VERSION)
    string(TIMESTAMP SMALLCVCV_VERSION "%Y%m%d")
endif()

set(SMALLCV_VERSION_MAJOR 0)
set(SMALLCV_VERSION_MINOR 0)
set(SMALLCV_VERSION_PATCH 0)
set(SMALLCV_VERSION_STRING ${SMALLCV_VERSION_MAJOR}.${SMALLCV_VERSION_MINOR}.${SMALLCV_VERSION_PATCH})
if(APPLE OR IOS)
    # macos / ios only accepts a.b.c.d.e where a=24bit b/c/d/e=10bit
    # 20201228 to 20.12.28
    set(SMALLCV_VERSION_STRING ${SMALLCV_VERSION_MAJOR}.${SMALLCV_VERSION_MINOR}.${SMALLCV_VERSION_PATCH}.0.0)
endif()
message(STATUS "SMALLCV_VERSION_STRING = ${SMALLCV_VERSION_STRING}")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/smallcv/version.h.in ${CMAKE_CURRENT_BINARY_DIR}/version.h)


#----------------------------------------------------------------------
# smallcv library
#----------------------------------------------------------------------
set(smallcv_api_files
  ${CMAKE_SOURCE_DIR}/api/smallcv/smallcv.hpp
  ${CMAKE_SOURCE_DIR}/api/smallcv/mat.hpp
  ${CMAKE_SOURCE_DIR}/api/smallcv/imdraw.hpp
  ${CMAKE_SOURCE_DIR}/api/smallcv/border.hpp
  ${CMAKE_SOURCE_DIR}/api/smallcv/log.hpp
  ${CMAKE_SOURCE_DIR}/api/smallcv/benchmark.hpp
  ${CMAKE_SOURCE_DIR}/api/smallcv/resize.hpp
)

set(smallcv_src_files
  ${CMAKE_SOURCE_DIR}/src/smallcv/smallcv.cpp
  ${CMAKE_SOURCE_DIR}/src/smallcv/improc_private.hpp
  ${CMAKE_SOURCE_DIR}/src/smallcv/improc_private.cpp
  ${CMAKE_SOURCE_DIR}/src/smallcv/imdraw.cpp
  ${CMAKE_SOURCE_DIR}/src/smallcv/border.cpp
  ${CMAKE_SOURCE_DIR}/src/smallcv/benchmark.cpp
  ${CMAKE_SOURCE_DIR}/src/smallcv/resize.cpp
)

if(SMALLCV_IMAGEIO)
  list(APPEND smallcv_api_files ${CMAKE_SOURCE_DIR}/api/smallcv/imageio.hpp)
  list(APPEND smallcv_src_files ${CMAKE_SOURCE_DIR}/src/smallcv/imageio.cpp)
endif()

if(SMALLCV_IMSHOW)
  list(APPEND smallcv_api_files ${CMAKE_SOURCE_DIR}/api/smallcv/imshow.hpp)
  list(APPEND smallcv_src_files
    ${CMAKE_SOURCE_DIR}/src/smallcv/imshow.cpp
    ${CMAKE_SOURCE_DIR}/deps/imshow/glad/glad.c
  )
endif()

add_library(smallcv STATIC
  ${smallcv_api_files}
  ${smallcv_src_files}
)

set_target_properties(smallcv PROPERTIES
  PUBLIC_HEADER
    "${smallcv_api_files}"
  DEBUG_POSTFIX
    "_d"
)

target_include_directories(smallcv
  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src"
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/api"
)

if(SMALLCV_IMAGEIO)
  target_compile_definitions(smallcv PUBLIC -DSMALLCV_IMAGEIO)
  target_include_directories(smallcv PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/deps/stb")
endif()

if(SMALLCV_IMSHOW)
  target_compile_definitions(smallcv PUBLIC -DSMALLCV_IMSHOW)
  target_link_libraries(smallcv glfw)
  target_include_directories(smallcv PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/deps/imshow")
endif()


if(SMALLCV_EXAMPLES)
  add_subdirectory(examples)
endif()

# Report summary
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/summary.cmake")

install(TARGETS smallcv)


#----------------------------------------------------------------------
# Unit tests
#----------------------------------------------------------------------
if(SmallCV_TEST)
  enable_testing()
  find_package(GTest REQUIRED)

  macro(smallcv_add_test name)
    add_executable(test_${name} tests/test_${name}.cpp)
    target_link_libraries(test_${name} PRIVATE smallcv_static GTest::gtest GTest::gtest_main)
    target_include_directories(test_${name} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
    gtest_add_tests(TARGET test_${name})
  endmacro()

  smallcv_add_test(version)
endif()
