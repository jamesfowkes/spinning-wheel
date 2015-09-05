#ifndef _RESISTOR_H_
#define _RESISTOR_H_

#define NRESISTORS (4) // Number of resistors in bank
#define START_PIN (3) // Digital pin of smallest value resistor

void setup_bias_resistors();
void set_bias_resistor(int resistor);

#endif
