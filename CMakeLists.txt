# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.20)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set name of project (as PROJECT_NAME) and C/C++ standards
project(ds1307 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

# turn on all compiler warnings 
add_compile_options(-Wall)

# Tell CMake where to find the executable source file #0
add_executable(${PROJECT_NAME} 
  examples/ds1307/main.cpp
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

# Add libraries #1
add_library(pico_ds1307 INTERFACE )

# Add target sources #2
target_sources(pico_ds1307 INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/ds1307/ds1307.cpp)


# Add Target include directories  #3
target_include_directories(
pico_ds1307 INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include
)

# Pull in pico libraries that we need #4
target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_i2c pico_ds1307)


# Enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

