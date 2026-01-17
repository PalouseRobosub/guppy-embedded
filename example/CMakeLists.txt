# Demo cmake rules for the can2040 example using Pico SDK
#
# This is a minimal set of build rules.  See the PICO SDK
# documentation for more information.  In brief, from a Unix
# command-line one would issue commands similar to:
#   mkdir build
#   cd build
#   cmake -D PICO_BOARD=pico ..
#   make
# Then flash the resulting canexample.uf2 to the target board.

cmake_minimum_required(VERSION 3.13)

# Download SDK from git (or "cmake -D PICO_SDK_PATH=/some/path ..." to avoid)
set(PICO_SDK_FETCH_FROM_GIT on)

include(pico_sdk_import.cmake)
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

project(can2040_example)

add_executable(canexample
    example.c
    ../lib/guppy_lib.c
)

pico_set_binary_type(canexample copy_to_ram)
target_link_libraries(canexample
    pico_stdlib
    hardware_pwm
    can2040
)


pico_enable_stdio_usb(canexample 1)
pico_enable_stdio_uart(canexample 0)

pico_add_extra_outputs(canexample)
