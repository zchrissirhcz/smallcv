#----------------------------------------
# basic example
#----------------------------------------
add_executable(basic_example
  ${CMAKE_SOURCE_DIR}/examples/example.cpp
)
target_link_libraries(basic_example smallcv)

# copy image
add_custom_command(TARGET basic_example
  POST_BUILD

  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/assets/mingren.jpg ${CMAKE_BINARY_DIR}/
)


#----------------------------------------
# ncnn examples
#----------------------------------------
if(SMALLCV_EXAMPLES_NCNN)
  add_executable(ncnn_squeezenet
    ${CMAKE_SOURCE_DIR}/examples/ncnn/squeezenet.cpp
  )
  target_link_libraries(ncnn_squeezenet smallcv ncnn)

  add_executable(ncnn_squeezenet_c_api
    ${CMAKE_SOURCE_DIR}/examples/ncnn/squeezenet_c_api.cpp
  )
  target_link_libraries(ncnn_squeezenet_c_api smallcv ncnn)

  add_executable(ncnn_shufflenetv2
    ${CMAKE_SOURCE_DIR}/examples/ncnn/shufflenetv2.cpp
  )
  target_link_libraries(ncnn_shufflenetv2 smallcv ncnn)

  add_executable(ncnn_simplepose
    ${CMAKE_SOURCE_DIR}/examples/ncnn/simplepose.cpp
  )
  target_link_libraries(ncnn_simplepose smallcv ncnn)

  set(file_lst
    000017.bmp 000017.jpg mingren.jpg
    pose.bin pose.param
    shufflenet_v2_x0.5-opt.bin shufflenet_v2_x0.5-opt.param
    squeezenet_v1.1.bin squeezenet_v1.1.param
    squeezenet_v1.1.param.bin squeezenet_v1.1.prototxt
  )
  #string(REPLACE ";" " " file_lst ${file_lst})
  #message(FATAL_ERROR "--- file_lst: ${file_lst}")
  foreach(file ${file_lst})
    message(STATUS "--- copy file: ${file}")
    execute_process(
      COMMAND ${CMAKE_COMMAND} -E copy
      ${CMAKE_SOURCE_DIR}/assets/${file}
      ${CMAKE_CURRENT_BINARY_DIR}
    )
  endforeach()
  # add_executable(ncnn_nanodet
  #   examples/ncnn/nanodet.cpp
  # )
  # target_link_libraries(ncnn_nanodet smallcv ncnn)
endif()
