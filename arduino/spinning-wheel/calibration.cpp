#include <Arduino.h>

#include "app.h"
#include "resistors.h"

// Stores off, minimum and maximum readings from each bias resistor
static unsigned int s_off_readings[NRESISTORS];
static unsigned int s_off_threshold;

static unsigned int s_max_reading;
static unsigned int s_min_reading;
static unsigned int s_res;

static void get_average_off(int res)
{
		for(char i = 0; i < 10; i++)
		{
			delay(5);
			s_off_readings[res] += analogRead(A0);
		}
		
		s_off_readings[res] /= 10;
}

static void calibrate_off()
{
	app_prompt_for_off();

	s_off_threshold = 0;
	
	for(unsigned char res = 0; res < NRESISTORS; res++)
	{
		set_bias_resistor(res);
		
		get_average_off(res);

		if (s_off_readings[res] > s_off_threshold)
		{
			s_off_threshold = s_off_readings[res] - 10;
			s_res = res;
		}
	}
}

static void calibrate_minmax()
{
	app_prompt_for_cycle();

	set_bias_resistor(s_res);
	
	unsigned long start = millis();

	while((millis() - start) < 10000UL)
	{
		int reading = analogRead(A0);
		if (reading < s_off_threshold)
		{
			s_max_reading = max(s_max_reading, reading);
			s_min_reading = min(s_min_reading, reading);
		}
		delay(5);
	}
}

int get_res() { return s_res; }
int get_off() { return s_off_threshold; }
int get_min() { return s_min_reading; }
int get_max() { return s_max_reading; }

void calibrate()
{
	for(unsigned char i = 0; i < NRESISTORS; i++)
	{
		s_off_readings[i] = 0;
		s_off_threshold = 0;
	}
	
	s_max_reading = 0;
	s_min_reading = 1023;
	
	calibrate_off();
	calibrate_minmax();
}
