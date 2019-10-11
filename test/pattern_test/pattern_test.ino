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

void setup() {
	Serial.begin(9600);
	randomSeed(analogRead(0));
	pattern = int(random(4));
	pattern_len = sizeof(patterns[pattern]) / sizeof(int);

	pinMode(4, OUTPUT);
}

void loop() {
	if(millis() >= next_swap) {
		swapState();
		next_swap = millis() + (patterns[pattern][pattern_pos]*1000);
		pattern_pos = (pattern_pos + 1) % pattern_len;
	}

	Serial.println(state);
}

void swapState() {
	if(state) digitalWrite(4, LOW);
	else digitalWrite(4, HIGH);
	state = !state;
}
