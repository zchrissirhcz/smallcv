if(DEFINED ENV{ARTIFACTS_DIR})
  set(ARTIFACTS_DIR "$ENV{ARTIFACTS_DIR}")
else()
  message(WARNING "ARTIFACTS_DIR env var not defined, abort")
endif()


# check memory leak with VLD in Visual Studio
if(SMALLCV_USE_VLD)
  add_definitions(-DSMALLCV_USE_VLD)
endif()

#----------------------------------------------------------------------#
# glfw
#----------------------------------------------------------------------#
if(SMALLCV_IMSHOW)
  find_package(glfw3 QUIET) # 3.3-stable branch recommended
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

#----------------------------------------------------------------------#
# ncnn
#----------------------------------------------------------------------#
if(SMALLCV_EXAMPLES_NCNN)
  find_package(ncnn REQUIRED) # >= 20201218 recommended
  if(ncnn_FOUND)
    message(STATUS "[Found ncnn] ncnn_DIR is ${ncnn_DIR}")
  else()
    message(STATUS "[Not Found ncnn]")
    message(STATUS "Please remove cmake build cache, then:")
    message(STATUS "* If already installed ncnn (version >= 20201218), please set ncnn_DIR in one of:")
    message(STATUS "      file CMakeLists.txt, before `find_package(ncnn)`")
    message(STATUS "      file build/xxx.cache.cmake")
    message(STATUS "      file build/xxx.{sh,cmd}, with -Dncnn_DIR=/path/to/ncnnConfig.cmake")
    message(STATUS "      You may get inspired from build/xxx.cache.cmake :)")
    message(STATUS "* You may also build and install ncnn manually, e.g")
    message(STATUS "      git clone https://gitee.com/mirrors/ncnn -b 20201218 ncnn20201218")
    message(STATUS "      cd ncnn20201218 && mkdir build && cd build")
    message(STATUS "      cmake .. -DCMAKE_INSTALL_PREFIX=./install && cmake --build . && cmake --install .")
    message(STATUS "      then set ncnn_DIR before `find_package(ncnn)`")
    message(FATAL_ERROR "")
  endif()
endif()


#--- GoogleTest
if(ANDROID)
  set(GTest_DIR "${ARTIFACTS_DIR}/googletest/1.11.0/android-arm64/lib/cmake/GTest" CACHE PATH "Directory that contains GTestConfig.cmake")
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
  set(GTest_DIR "${ARTIFACTS_DIR}/googletest/1.11.0/linux-x64/lib/cmake/GTest" CACHE PATH "Directory that contains GTestConfig.cmake")
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
  set(GTest_DIR "${ARTIFACTS_DIR}/googletest/1.11.0/windows/vs2019-x64/lib/cmake/GTest" CACHE PATH "Directory that contains GTestConfig.cmake")
elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin")
  set(GTest_DIR "${ARTIFACTS_DIR}/googletest/1.11.0/mac-x64/lib/cmake/GTest" CACHE PATH "Directory that contains GTestConfig.cmake")
else()
  message(WARNING "GTest_DIR not set yet")
endif()
message(STATUS ">>> GTest_DIR is: ${GTest_DIR}")
#find_package(GTest REQUIRED)