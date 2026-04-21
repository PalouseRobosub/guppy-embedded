add_library(led_strip INTERFACE)

pico_enable_stdio_usb(led_strip 1)
pico_enable_stdio_uart(led_strip 0)

target_include_directories(led_strip INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

# Pull in pico libraries that we need
target_link_libraries(led_strip INTERFACE 
    pico_stdlib
    hardware_pwm
    pico_neopixel
)