cmake_minimum_required(VERSION 3.13)

# Pico SDK
set(PICO_SDK_FETCH_FROM_GIT on)
include(pico_sdk_import.cmake)

project(guppy_embedded)
set(CMAKE_C_STANDARD 11)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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

# add libraries
add_subdirectory(lib/pico_neopixels)
add_subdirectory(lib/led)
add_subdirectory(lib/barometer)
add_subdirectory(lib/guppy_lib)

# add stuff from modules folder
file(GLOB MODULE_SOURCES
    src/modules/*.c
    src/modules/*.cpp
)


# Motor Controller Board
add_executable(board_motor
    src/main.cpp
    ${MODULE_SOURCES}
)

pico_set_binary_type(board_motor copy_to_ram)
target_link_libraries(board_motor
    pico_stdlib
    can2040
    guppy_lib
    pico_neopixel
    led_strip
    barometer
)

target_compile_definitions(board_motor PRIVATE BOARD_TYPE=1)

pico_enable_stdio_usb(board_motor 1)
pico_enable_stdio_uart(board_motor 0)

pico_add_extra_outputs(board_motor)


# Wet Board
add_executable(board_wet
    src/main.cpp
    ${MODULE_SOURCES}
)

pico_set_binary_type(board_wet copy_to_ram) # TODO: look at this
target_link_libraries(board_wet
    pico_stdlib
    can2040
    guppy_lib
    pico_neopixel
    led_strip
    barometer
)

target_compile_definitions(board_wet PRIVATE BOARD_TYPE=2)

pico_enable_stdio_usb(board_wet 1)
pico_enable_stdio_uart(board_wet 0)

pico_add_extra_outputs(board_wet)

# Test Board
add_executable(board_test
        src/main.cpp
        ${MODULE_SOURCES}
)

pico_set_binary_type(board_test copy_to_ram)
target_link_libraries(board_test
        pico_stdlib
        can2040
        guppy_lib
        pico_neopixel
        led_strip
        barometer
)

target_compile_definitions(board_test PRIVATE BOARD_TYPE=-1)

pico_enable_stdio_usb(board_test 1)
pico_enable_stdio_uart(board_test 0)

pico_add_extra_outputs(board_test)
