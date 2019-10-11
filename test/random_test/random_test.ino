
unsigned long next_swap = 0;
volatile byte state = LOW;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));

  pinMode(4, OUTPUT);
}

void loop() {
  if(millis() >= next_swap) {
  	swapState();
  	next_swap = millis() + random(200, 1001);
  }

  Serial.println(state);

}

void swapState() {
	if(state) digitalWrite(4, LOW);
	else digitalWrite(4, HIGH);
	state = !state;
}