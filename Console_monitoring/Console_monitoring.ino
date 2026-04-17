const int pin12v = 10;
const int pin5v = 11;
const int pinTX = 12; 
int state12v, state5v, stateTX;


void setup() {
  // Input setup
  pinMode(pin12v, INPUT_PULLUP);  // Red wire -- +12V power on
  pinMode(pin5v, INPUT_PULLUP);  // Green wire -- +5V power on
  pinMode(pinTX, INPUT_PULLUP);  // Orange wire -- RF TX power on

  // Output setup
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  // Pin state setup
  state12v = digitalRead(pin12v);
  state5v = digitalRead(pin5v);
  stateTX = digitalRead(pinTX);

  print_pin_state(pin12v);
  print_pin_state(pin5v);
  print_pin_state(pinTX);
}

void loop() {

  state12v = check_pin_state_change(state12v, pin12v);
  state5v = check_pin_state_change(state5v, pin5v);
  stateTX = check_pin_state_change(stateTX, pinTX);

  delay(500);
}


int check_pin_state_change(int old_state, int pin_number) {
  /* 
  Checks for a difference between a pin's old state and new state
  If there is a difference between the two, send a message to the console
  In any case, returns new_state 
  */

  int new_state = digitalRead(pin_number);

  if (old_state != new_state) {
    print_pin_state(pin_number);
  }

  return new_state;
}

void print_pin_state(int pin_number) {

  // Print pin reference
  if (pin_number == pin12v) {
    Serial.print("12V: ");
  } 
  else if (pin_number == pin5v) {
    Serial.print("5V: ");
  } 
  else if (pin_number == pinTX) {
    Serial.print("TX: ");
  } 

  // Print pin state
  if (digitalRead(pin_number) == 1) {
    Serial.println("OFF");
  }
  else {
    Serial.println("ON");
  }
}
