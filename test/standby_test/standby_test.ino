int patterns[][10] = { 	{5, 5, 4, 4, 3, 3, 2, 2, 1, 1},
						{5, 1, 4, 2, 3, 3, 2, 4, 1, 5},
						{1, 1, 2, 2, 3, 3, 4, 4, 5, 5},
						{4, 4, 2, 2, 5, 5, 1, 1, 3, 3}
					};

const int num_patterns = 4;

int pattern;
int pattern_pos = 0;
int pattern_len;

unsigned long next_swap = 0;
volatile byte state = LOW;

const byte mode = 0;		//0 For pattern, 1 for Random

const unsigned long bounce = 50;
volatile long last_standby = 0;
volatile long last_resume = 0;

volatile bool standby = 0;

void setup() {
	Serial.begin(9600);
	pinMode(4, OUTPUT);

	pinMode(3, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(3), isr_standby, FALLING);
	attachInterrupt(digitalPinToInterrupt(3), isr_resume, RISING);
}

void loop() {
	if()
}

void isr_standby() {
	if(millis() - last_standby > bounce) {

	}
}

void isr_resume() {

}