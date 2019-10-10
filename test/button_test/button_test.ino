#define BUTTON 2
#define RELAY 4

#define bounce_time 10

volatile long last_swap = 0;
volatile bool relay_open = false;


void setup() {
  Serial.begin(9600);
  
  pinMode(BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON), isr_swap, CHANGE);
}

void loop() {
  /*if(digitalRead(BUTTON) == 0) {
    down = millis();
    relay_state = 1;
  } else {
    up = millis();
    relay_state = 0;
  }

  Serial.println(relay_state);*/
}

void isr_swap() {
    if(millis() - last_swap > bounce_time) {
      last_swap = millis();

      if(relay_open) {
        relay_open = false;
        Serial.println("RELEASED");
      } else {
        relay_open = true;
        Serial.println("PRESSED");
      }
    }
}
