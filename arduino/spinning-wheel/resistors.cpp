#include <Arduino.h>

#include "resistors.h"

void setup_bias_resistors()
{
	for(char i = 0; i < NRESISTORS; i++)
	{
		pinMode(START_PIN + i, OUTPUT);
	}
}

void set_bias_resistor(int resistor)
{
	if (resistor <= (NRESISTORS-1) )
	{
		for(char i = 0; i < NRESISTORS; i++)
		{
			digitalWrite(START_PIN + i, LOW);
		}
		digitalWrite(resistor + START_PIN, HIGH);
	}
}
