#ifndef _GUPPY_LIB_H
#define _GUPPY_LIB_H

#include "pico/stdlib.h"
#include "can2040.h"

// set up the can bus. TODO: add parameter support? shouldn't it all be the same?
void canbus_setup();

// reads a frame. Returns whether or not there is a frame to read
bool canbus_read(struct can2040_msg *msg);

// sends a float over CAN
int canbus_transmit_float(uint32_t id, float value); // TODO: should all return values for success be a bool or int?

// parses can frame as a float. (assumes data is sent as little endian)
float can_read_float(struct can2040_msg msg);
int can_read_int(struct can2040_msg msg);

// sets up a pin for PWM and gives it an initial signal of 1500 microseconds.
void add_pwm_pin(uint pin_num);

// writes a pwm signal to a pin. It must first be set up using add_pwm_pin()
void pwm_write(uint pin_num, uint16_t level);

// converts a float from -1.0 to 1.0 into a pwm signal
int throttle_to_pwm_us(float value);

#endif // _GUPPY_LIB_H





// OLD STUFF

//void guppy_start(uint32_t bitrate, uint32_t gpio_rx, uint32_t gpio_tx);

// subscribes to a can ID (should I do it still?)
//void can_listen(uint32_t id);