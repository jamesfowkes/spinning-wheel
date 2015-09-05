#include <Arduino.h>

#include "app.h"

void app_prompt_for_off()
{
	Serial.println("Set pedal to OFF and hit ENTER.");
	char c;
	do{
	    c = Serial.read();
	} while (c != '\n');
}

void app_prompt_for_cycle()
{
	Serial.println("When ready, hit ENTER and begin cycling the pedal through its full travel.");

	char c;
	do{
	    c = Serial.read();
	} while (c != '\n');
}
