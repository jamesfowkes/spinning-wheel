#define DELAY_MS (20)

#define DIRA_PIN 12
#define PWMA_PIN 3
#define BRAKEA_PIN 8

#define DIR_SELECT_PIN 2
#define SPEED_INPUT_PIN A0

enum dir_enum
{
	CW,
	CCW
};
typedef enum dir_enum DIR_ENUM;

DIR_ENUM s_direction = CCW;
uint8_t s_pwm = 0;

uint16_t s_maxReading = 0;

void setup()
{
  	pinMode(DIRA_PIN, OUTPUT);
	pinMode(PWMA_PIN, OUTPUT);
	pinMode(BRAKEA_PIN, OUTPUT);
	
	pinMode(DIR_SELECT_PIN, INPUT);
	pinMode(SPEED_INPUT_PIN, INPUT);
	
  	digitalWrite(DIRA_PIN, LOW);
	digitalWrite(PWMA_PIN, LOW);
	digitalWrite(BRAKEA_PIN, LOW);
	
	// Maximum reading is assumed to be when foot is fully up
	// i.e. max reading is at zero speed
	s_maxReading = analogRead(SPEED_INPUT_PIN);
}

void loop()
{
	
	// Make sure the maximum reading is updated just in case 
	// the highest possible value wasn't picked up
	uint16_t newReading = analogRead(SPEED_INPUT_PIN);
	s_maxReading = max(s_maxReading, newReading);
	
	// Speed demand increased as reading reduces
	uint16_t speedDemand = s_maxReading - newReading;
	
	// Convert speed demand (up to s_maxReading) to PWM (up to 255)
	
	s_pwm = (uint8_t)map(speedDemand, 0U, s_maxReading, 0U, 255U);
	
	analogWrite(PWMA_PIN, s_pwm);
	
	/* Only allow change of direction if demand is low enough */
	if (s_pwm < 10)
	{
		s_direction = (digitalRead(DIR_SELECT_PIN) == HIGH) ? CCW : CW;
        digitalWrite(DIRA_PIN, (s_direction == CW) ? LOW : HIGH);
	}
	delay(DELAY_MS);
}
