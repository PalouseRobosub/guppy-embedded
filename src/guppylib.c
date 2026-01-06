#include "pico/stdlib.h"
#include "hardware/pwm.h"

#include "guppylib.h"

/* ---------------CAN stuff--------------- */

// Simple example of irq safe queue (this is not multi-core safe)
#define QUEUE_SIZE 128 // Must be power of 2
static struct {
    uint32_t pull_pos;
    volatile uint32_t push_pos;
    struct can2040_msg queue[QUEUE_SIZE];
} MessageQueue;

// Internal storage for can2040 module
static struct can2040 cbus;

// PIO interrupt handler
static void PIOx_IRQHandler()
{
    can2040_pio_irq_handler(&cbus);
}

void canbus_setup()
{
    uint32_t pio_num = 0;
    uint32_t sys_clock = SYS_CLK_HZ, bitrate = 500000;
    uint32_t gpio_rx = 8, gpio_tx = 9;

    // Setup canbus
    can2040_setup(&cbus, pio_num);
    can2040_callback_config(&cbus, can2040_cb);

    // Enable irqs
    irq_set_exclusive_handler(PIO0_IRQ_0, PIOx_IRQHandler);
    irq_set_priority(PIO0_IRQ_0, 1);
    irq_set_enabled(PIO0_IRQ_0, 1);

    // Start canbus
    can2040_start(&cbus, sys_clock, bitrate, gpio_rx, gpio_tx);
}

bool canbus_read_message(can2040_msg *msg)
{
    const uint32_t push = MessageQueue.push_pos;
    const uint32_t pull = MessageQueue.pull_pos;

    if (pull == push) return false;

    struct can2040_msg *qmsg = &MessageQueue.queue[pull % QUEUE_SIZE];
    msg = *qmsg;
    MessageQueue.pull_pos++;
}

int canbus_transmit_float(uint32_t id, float value)
{
    struct can2040_msg tmsg;
    tmsg.id = id; // TODO: isn't id 11 bits, why does this take 32bit?
    tmsg.dlc = 8;
    uint32_t data;
    memcpy(&data, value, sizeof(float));
    tmsg.data32[0] = data;
    int sts = can2040_transmit(&cbus, &tmsg);

    return sts;
}

float can_read_float(can2040_msg msg) // TODO: is it possible to have a type generic for what to parse to?
{
    float value;
    memcpy(&value, msg.data, sizeof(float));

    return value;
}

/* ---------------PWM stuff--------------- */

// TODO: look over function rq
void add_pwm_pin(uint pin_num)
{
    gpio_set_function(pin_num, GPIO_FUNC_PWM);

    const uint slice_num = pwm_gpio_to_slice_num(pwm_pin);
    const uint channel_num = pwm_gpio_to_channel(pwm_pin);

    const float divider = 150.0f; // this maps the level to be in micro seconds
    const int wrap = 20000;

    // Configure PWM frequency (wrap value) and duty cycle (channel level)
    pwm_set_clkdiv(slice_num, divider);
    pwm_set_wrap(slice_num, wrap); // TODO: look at wrap

    // set initial signal to 1500, which is neutral state
    pwm_set_chan_level(slice_num, channel_num, 1500);
    pwm_set_enabled(slice_num, true);
}

void pwm_write(uint pin_num, uint16_t level)
{
    const uint slice_num = pwm_gpio_to_slice_num(pwm_pin);
    const uint channel_num = pwm_gpio_to_channel(pwm_pin);

    pwm_set_chan_level(slice_num, channel_num, level);
}

int throttle_to_pwm_us(float value)
{
    return 1500 + (int)(value * 400);
}