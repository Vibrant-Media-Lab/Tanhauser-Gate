/**
 * OdysseyNow Computer Controller
 * Author: Joshua Arabia
 * Pitt Vibrant Media Lab: 2019
 */

#define TRIGGER_BUTTON    2
#define PROGRAMMING_BUTTON  3
#define RELAY           4

// Associate physical button states with their output
#define UP          HIGH
#define DOWN        LOW

// Set the bounds for random gate intervals
#define RANDOM_LOWER_BOUND_UP 1600
#define RANDOM_UPPER_BOUND_UP 2401
#define RANDOM_LOWER_BOUND_DOWN 600
#define RANDOM_UPPER_BOUND_DOWN 1401
int rand_up = 0;
#define NUM_PATTERNS       4

// Define the two major modes for automated state switching
enum mode {
  NONE,
  PATTERN,
  RANDOM
};

  // GENERAL TIMING //
const long contact_bounce = 50;
const long long_press_threshold = 2000;
long standby_bounce = 0;

unsigned long next_relay_swap = 0;
long cycle_start;

  // PROGRAMMING //
volatile int programming_button_state = UP;
volatile unsigned long programming_button_last_input = 0;
volatile unsigned long programming_button_hold_duration = 0;

  // TRIGGER //
const long trigger_active_period = 35; // Roughly 1/30 s
volatile int trigger_button_state = UP;
volatile unsigned long trigger_button_last_input = 0;

  // RELAY //
volatile int relay_state = LOW;
volatile int gate_output = LOW;
volatile int trigger_output = LOW;

  // GAMEPLAY //
mode gameplay_mode = NONE;
int pattern_index;
int pattern_position;

long patterns[NUM_PATTERNS][10] = {   
            {5, 5, 4, 4, 3, 3, 2, 2, 1, 1},
            {5, 1, 4, 2, 3, 3, 2, 4, 1, 5},
            {1, 1, 2, 2, 3, 3, 4, 4, 5, 5},
            {4, 4, 2, 2, 5, 5, 1, 1, 3, 3}
          };

int standby = 0;

void setup() {
  // For debug
  Serial.begin(9600);
  randomSeed(analogRead(0));

  // Configure pinouts for the controller
  pinMode(PROGRAMMING_BUTTON, INPUT_PULLUP);
  pinMode(TRIGGER_BUTTON, INPUT_PULLUP);
  pinMode(RELAY, OUTPUT);

  // Configure interrupts on the input buttons
  attachInterrupt(digitalPinToInterrupt(PROGRAMMING_BUTTON), isr_programming, CHANGE);
  attachInterrupt(digitalPinToInterrupt(TRIGGER_BUTTON), isr_trigger, CHANGE);

}

void loop() {
  cycle_start = millis();

  if(millis() - trigger_button_last_input >= trigger_active_period && trigger_output == HIGH) {
    Serial.println("TRIGGER END");
    trigger_output = LOW;
  }

  // Detect a long press of the programming button to activate pattern mode
  if(millis() - programming_button_last_input >= long_press_threshold && programming_button_state == DOWN && gameplay_mode != PATTERN) {
    Serial.println("ENTER PATTERN MODE");
    gameplay_mode = PATTERN;
  }

  if(programming_button_state == DOWN && trigger_button_state == DOWN && gameplay_mode != RANDOM) {
    Serial.println("ENTER RANDOM MODE");
    gameplay_mode = RANDOM;
  }

  if(millis() >= next_relay_swap) {
    if(gameplay_mode == PATTERN) {
      next_relay_swap = millis() + patterns[pattern_index][++pattern_position] * 1000;
      gate_output = !gate_output;
    } else if(gameplay_mode == RANDOM) {
      if(rand_up) next_relay_swap = millis() + long(random(RANDOM_LOWER_BOUND_DOWN, RANDOM_UPPER_BOUND_DOWN));
      else next_relay_swap = millis() + long(random(RANDOM_LOWER_BOUND_UP, RANDOM_UPPER_BOUND_UP));
      rand_up = !rand_up;
      gate_output = !gate_output;
    }
  }

  if(gameplay_mode) relay_state = gate_output;
  else relay_state = !trigger_output;

  digitalWrite(RELAY, relay_state);
  Serial.print(gameplay_mode); Serial.print(gate_output); Serial.print(!trigger_output); Serial.println(relay_state);

  if(standby) {
    relay_state = DOWN;
    pattern_index = random(0, NUM_PATTERNS);
    pattern_position = 0;
  }
}

/**
 * Interrupt Service Routine called on press and release of the programming button:
 * If the interrupt doesn't bounce, update the state and timing variables for the
 * associated button. 
 */
void isr_programming() {
  if(millis() - programming_button_last_input >= contact_bounce) {
    programming_button_last_input = millis();

    if(programming_button_state == DOWN) {
      programming_button_state = UP;
      if(programming_button_hold_duration < long_press_threshold) {
        standby = 0;
      }
    } else {
      programming_button_state = DOWN;
      standby = 1;
    }

    programming_button_hold_duration = 0;
  }
}

/**
 * Interrupt Service Routine called on press and release of the programming button:
 * If the interrupt doesn't bounce, update the state and timing variables for the
 * associated button. 
 */
void isr_trigger() {
  if(millis() - trigger_button_last_input >= contact_bounce) {
    trigger_button_last_input = millis();

    if(trigger_button_state == DOWN) {
      trigger_button_state = UP;
    } else {
      Serial.println("TRIGGER START");
      trigger_button_state = DOWN;
      trigger_output = HIGH;
    }
  }
}
