#include "led.h"
#include "pico/rand.h"

#define NUM_LEDS 144

// Using pin 19 for testing - any gpio pin should do
Adafruit_NeoPixel init_led(int pin) {
    Adafruit_NeoPixel ledStrip(NUM_LEDS, pin, NEO_GRB + NEO_KHZ800);
    ledStrip.begin();
    return ledStrip;
}

void run_led_control()
{
    Adafruit_NeoPixel ledStrip = init_led(19);
    int red = 10;
    int green = 0;
    int blue = 0;
	int chunk_size = 6;
	int count = 0;
	int brightness = 12;
    while(1)
    {

		for (int i = 0; i < NUM_LEDS/chunk_size; ++i)
		{
			for (int j = 0; j < chunk_size; ++j)
			{
				if (i % 2 == (count % 2))
                    ledStrip.setPixelColor((chunk_size*i + j) % NUM_LEDS, ledStrip.Color(brightness, 0, 0));
				if (i % 2 == ((count + 1) % 2))
                    ledStrip.setPixelColor((chunk_size*i + j) % NUM_LEDS, ledStrip.Color(brightness, brightness, brightness));
			}
		}
        ledStrip.show();
        sleep_ms(100);
		++count;
/*
        for (int i = 0; i < NUM_LEDS; i++)
        {
            ledStrip.setPixelColor(i, ledStrip.Color(red, green, blue));
            ledStrip.show();
            sleep_ms(10);
        }
        red = get_rand_32() % 100;
        green = get_rand_32() % 100;
        blue = get_rand_32() % 100;
*/
    }
}
