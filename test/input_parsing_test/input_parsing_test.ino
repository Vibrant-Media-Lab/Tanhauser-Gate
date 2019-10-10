#define PRO 2
#define TRI	3
#define REL	4

#define DOWN LOW
#define UP HIGH


enum mode {
	PATTERN, RANDOM
};

//TIMING
const long bounce = 10;		//Period after a swap to ignore input from a button
const long long_press_length = 2000; //Threshold of a long press for the programming button
long next_relay_swap = 0;	//How long from now should the relay be swapped
long cycle_start = 0;		//Length of the cycle

//PROGRAMMING
volatile int pro_state = UP;
volatile long pro_last = 0;	// Last change of the programming button
volatile long pro_len = 0;

//TRIGGER
const long tri_period = 67;	// 1/15 s trigger activation

volatile int tri_state = UP;
volatile int tri_last = 0;	// Last change of the trigger button

//RELAY
volatile int relay_state = LOW;

//GAMEPLAY
volatile mode gate_mode = NULL;

void setup() {
	Serial.begin(9600);
 	//Configure pinouts to board and inputs
	pinMode(PRO, INPUT_PULLUP);
	pinMode(TRI, INPUT_PULLUP);
	pinMode(REL, OUTPUT);

	//Configure interrupts on the input pins
	attachInterrupt(digitalPinToInterrupt(PRO), isr_pro, CHANGE);
	attachInterrupt(digitalPinToInterrupt(TRI), isr_tri, CHANGE);
}

void loop() {
	cycle_start = millis();
	//End the relay opening for a single trigger press
	if(relay_state == HIGH && ((millis() - tri_last) > tri_period)) {
		Serial.println("TRIGGER END");
		relay_state = LOW;
	}
	//Recognize a long press of the programming button, enter pattern mode
	if(pro_len >= long_press_length && gate_mode != PATTERN) {
		gate_mode = PATTERN;
		Serial.println("PATTERN MODE ENTERED");
		pro_len = 0;
	}
	//Recognize 'simultaneous' presses and enter random mode
	if(pro_state == DOWN && tri_state == DOWN && gate_mode != RANDOM) {
		gate_mode = RANDOM;
		Serial.println("RANDOM MODE ENTERED");
	}

	if(pro_state == DOWN) {
		pro_len += (millis() - cycle_start);
	}
}

void swap_rel() {
	if(digitalRead(REL) == HIGH) {
		digitalWrite(REL, LOW);
	} else {
		digitalWrite(REL, HIGH);
	}
}

//Called when there's a change in the state of programming
void isr_pro() {
	if(millis() - pro_last > bounce) {
		pro_last = millis();
		if(pro_state == DOWN) {
			pro_state = UP;
		} else {
			pro_state = DOWN;
			pro_len = 0;
		}
	}
}

//Called when there's a change in the state of trigger
void isr_tri() {
	if(millis() - tri_last > bounce) {
		tri_last = millis();
		if(tri_state == DOWN) {
			tri_state = UP;
		} else {
			Serial.println("TRIGGER START");
			tri_state = DOWN;
			relay_state = HIGH;
		}
	}
}
