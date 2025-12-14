cmake_minimum_required(VERSION 3.16)

add_compile_options(-Wno-deprecated-declarations)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
set(PROJECT_NAME "esp_mesh_ptt")
project(${PROJECT_NAME})
