#include <TaskAction.h>

#include "resistors.h"
#include "calibration.h"

static String s_serial;
static bool s_pending_operation = false;

static bool s_debug = false;

static int get_last_reading()
{
	return analogRead(A0);
}

static void debug_task_fn()
{
	if (s_debug)
	{
		Serial.println(get_last_reading());
	}
}
static TaskAction debug_task(debug_task_fn, 1000, INFINITE_TICKS);

static void setup_IO()
{
	setup_bias_resistors();
	pinMode(A0, INPUT);
}

static void handleSerial()
{
	if (Serial.available())
	{
		char last;
		while (Serial.available())
		{
			last = Serial.read();
			s_serial += last;
		}

		if (last == '\n')
		{
			s_pending_operation = true;
			s_serial.trim();
		}
	}
}

static void clear_pending_operation()
{
	s_pending_operation = false;
	s_serial = "";
}

void setup()
{
	setup_IO();

	Serial.begin(115200);
}

void loop()
{
	handleSerial();
	if (s_pending_operation)
	{
		Serial.println(s_serial);

		if (s_serial.startsWith("R"))
		{
			set_bias_resistor(s_serial[1] - '0');
		}

		else if (s_serial == "DBGON")
		{
			s_debug = true;
		}
		
		else if (s_serial == "DBGOFF")
		{
			s_debug = false;
		}

		else if (s_serial == "CALIBRATE")
		{
			calibrate();
			Serial.print("Resistor: ");
			Serial.println(get_res());
			Serial.print("Off: ");
			Serial.println(get_off());
			Serial.print("Min: ");
			Serial.println(get_min());
			Serial.print("Max: ");
			Serial.println(get_max());
		}
		
		clear_pending_operation();
	}

	debug_task.tick();
}
