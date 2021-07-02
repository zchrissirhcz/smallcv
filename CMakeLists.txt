cmake_minimum_required(VERSION 3.15)

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

set(smallcv_api_files
  ${CMAKE_SOURCE_DIR}/api/smallcv/smallcv.hpp
  ${CMAKE_SOURCE_DIR}/api/smallcv/mat.hpp
  ${CMAKE_SOURCE_DIR}/api/smallcv/imdraw.hpp
  ${CMAKE_SOURCE_DIR}/api/smallcv/border.hpp
)

set(smallcv_src_files
  ${CMAKE_SOURCE_DIR}/src/smallcv/smallcv.cpp
  ${CMAKE_SOURCE_DIR}/src/smallcv/fc_log.h
  ${CMAKE_SOURCE_DIR}/src/smallcv/fc_log.c
  ${CMAKE_SOURCE_DIR}/src/smallcv/improc_private.hpp
  ${CMAKE_SOURCE_DIR}/src/smallcv/improc_private.cpp
  ${CMAKE_SOURCE_DIR}/src/smallcv/imdraw.cpp
  ${CMAKE_SOURCE_DIR}/src/smallcv/border.cpp
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

