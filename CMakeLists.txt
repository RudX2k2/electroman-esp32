# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

# Add each module CMake list
set(EXTRA_COMPONENT_DIRS ${CMAKE_SOURCE_DIR}/main)
set(EXTRA_COMPONENT_DIRS ${CMAKE_SOURCE_DIR}/components/wifi_controller)
set(EXTRA_COMPONENT_DIRS ${CMAKE_SOURCE_DIR}/components/web_server)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(electroman-esp32)
