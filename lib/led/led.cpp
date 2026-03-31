extern "C" {
    #include "can2040.h"
    #include "guppy_lib.h"
}
#include "Adafruit_NeoPixel.hpp"
#include "led.hpp"

/// Number of LEDs in a row of a continuous color
#define LED_GROUP_SIZE 13

LEDController::LEDController(int pin, int led_count) : led_strip(led_count, pin, NEO_GRB + NEO_KHZ800)
{
    tick_count = 0;
    time_last_updated = nil_time; // TODO: hopefully this works
    update_rate_ms = 250;
    led_strip.begin();
    state = State::STARTUP;
    this->led_count = led_count;
}

void LEDController::two_color(uint32_t color1, uint32_t color2)
{
    for (int i = 0; i <= led_count - LED_GROUP_SIZE*2; i += LED_GROUP_SIZE*2)
    {
        for (int j = 0; j < LED_GROUP_SIZE; ++j) {
            led_strip.setPixelColor(i+j, color1);
        }
        for (int j = LED_GROUP_SIZE; j < LED_GROUP_SIZE*2; ++j) {
            led_strip.setPixelColor(i+j, color2);
        }
    }
}

bool LEDController::update(const can2040_msg& msg)
{
    if (msg.id == 0x201) // CAN ID Spreadsheet `current state`
    {
        state = static_cast<State>(can_read_int(msg));
        return true;
    }
    if (msg.id == 0x021) // CAN ID Spreadsheet `led brightness`
    {
        float brightness_float = can_read_float(msg);
        if (brightness_float > 1.0) brightness_float = 1.0;
        if (brightness_float < 0.0) brightness_float = 0.0;
        brightness = static_cast<uint8_t>(brightness_float * 255.0);
        return true;
    }
    else return false;
}

void LEDController::tick()
{
    auto current_time = get_absolute_time();
    if (absolute_time_diff_us(time_last_updated, current_time)/1000 < update_rate_ms)
    {
        return;
    }
    time_last_updated = current_time;

    switch (state)
    {
        case State::STARTUP:  this->startup();  break;
        case State::HOLDING:  this->holding();  break;
        case State::NAV:      this->nav();      break;
        case State::TASK:     this->task();     break;
        case State::TELEOP:   this->teleop();   break;
        case State::DISABLED: this->disabled(); break;
        case State::FAULT:    this->fault();    break;
        default:       this->fault();    break;
    }
    led_strip.show();
    ++tick_count;
}

void LEDController::startup()
{
    this->two_color(this->color_green(), this->color_white());
}
void LEDController::holding()
{
    this->two_color(this->color_blue(), this->color_off());
}
void LEDController::nav()
{
    this->two_color(this->color_blue(), this->color_red());
}
void LEDController::task()
{
    this->two_color(this->color_red(), this->color_red());
}
void LEDController::teleop()
{
    for (int i = 0; i <= led_count - 1; i += 1)
    {
        led_strip.setPixelColor(i, this->color_green());
    }
}
void LEDController::disabled()
{
    for (int i = 0; i <= led_count - 6; i += 6)
    {
        led_strip.setPixelColor(i, this->color_green());
        led_strip.setPixelColor(i+1, this->color_off());
        led_strip.setPixelColor(i+2, this->color_off());
        led_strip.setPixelColor(i+3, this->color_off());
        led_strip.setPixelColor(i+4, this->color_off());
        led_strip.setPixelColor(i+5, this->color_off());
    }
}
void LEDController::fault()
{
    if (this->tick_count % 2)
        two_color(this->color_red(), this->color_white());
    else
        two_color(this->color_white(), this->color_red());
}

