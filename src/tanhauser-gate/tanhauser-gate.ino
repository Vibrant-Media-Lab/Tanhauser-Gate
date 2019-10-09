#define PROG_BTN  2
#define TRIG_BTN  3
#define RELAY     4

//TIMING
volatile long prog_down = 0;
volatile long prog_dur = 0;
volatile long relay_active = 0;

void setup() {
  // The programming button is set to pullup using the internal resistors
  // on the chip and interrupts are attached to its press and release
  pinMode(PROG_BTN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PROG_BTN), isr_prog_down, FALLING);
  attachInterrupt(digitalPinToInterrupt(PROG_BTN), isr_prog_up, RISING);
  // The trigger button is set to pullup using the internal resistors
  // on the chip and an interrupt is attached to its press
  pinMode(TRIG_BTN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(TRIG_BTN), isr_trig_down, FALLING);
  // The relay acts an an interface to the game board, and is pulled
  // down using the internal resistors. When active, the pin is set
  // to high, otherwise it is low.
  pinMode(RELAY, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

}

/**
 * Interrupt Service Routine: Programming -> Down
 * Detects start of input on the programming button. Sets the global 
 * 'prog_down' to the system time when the interrupt was triggered.
 * This is a blocking routine and it's completion time will not factor into 
 * the button press length.
 */
void isr_prog_down() {
  prog_down = millis();
}

/**
 * Interrupt Service Routine: Programming -> Up
 * Detects end of input on the programming button. Sets the global
 * 'prog_dur' variable based on 'prog_down' and the
 * system time when the interrupt was triggered.
 */
void isr_prog_up() {
  prog_dur = (millis() > prog_down) ?
                               millis() - prog_down : (0xFFFFFFFF - prog_down) + millis();
 }

/**
 * Interrupt Service Routine: Trigger -> Down
 * Detects start of input on the trigger button. Sets the global
 * 'relay_active' to the system time when the interrupt was triggered. This
 * is a blocking routine, and cannot track the length of time that the relay
 * must remain open for, so that must be done from 
 */
 void isr_trig_down() {
  digitalWrite(RELAY, HIGH);
  
 }
