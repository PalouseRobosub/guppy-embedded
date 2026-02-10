cmake_minimum_required(VERSION 3.13)

# Pico SDK
set(PICO_SDK_FETCH_FROM_GIT on)
include(pico_sdk_import.cmake)

project(guppy_embed)
set(CMAKE_C_STANDARD 11)

pico_sdk_init()

# Add can2040 lib
include(FetchContent)

FetchContent_Declare(
    can2040
    GIT_REPOSITORY https://github.com/KevinOConnor/can2040.git
    GIT_TAG master
)

FetchContent_MakeAvailable(can2040)

add_library(can2040 STATIC
    ${can2040_SOURCE_DIR}/src/can2040.c
)

target_include_directories(can2040 PUBLIC
    ${can2040_SOURCE_DIR}/src
)

target_link_libraries(can2040
    hardware_pio
)

# add stuff from modules folder
file(GLOB MODULE_SOURCES
    src/modules/*.c
    src/modules/*.cpp
)


# The project
add_executable(guppy_embedded
    src/main.cpp
    src/guppy_lib.c
    src/barometer.cpp
    ${MODULE_SOURCES}
)

pico_set_binary_type(guppy_embedded copy_to_ram)
target_link_libraries(guppy_embedded
    pico_stdlib
    hardware_pwm
    hardware_i2c
    can2040
)


pico_enable_stdio_usb(guppy_embedded 1)
pico_enable_stdio_uart(guppy_embedded 0)

pico_add_extra_outputs(guppy_embedded)