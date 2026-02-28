add_library(guppy_lib INTERFACE)

target_sources(guppy_lib INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/guppy_lib.c
)

pico_enable_stdio_usb(guppy_lib 1)
pico_enable_stdio_uart(guppy_lib 0)

target_include_directories(guppy_lib INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

# Pull in pico libraries that we need
target_link_libraries(guppy_lib INTERFACE 
    pico_stdlib
    hardware_pwm
)