int pin1, pin2, pin3;

void setup() {
  // Input setup
  pinMode(11, INPUT_PULLUP);  // Red wire -- +12V power on
  pinMode(12, INPUT_PULLUP);  // Green wire -- +5V power on
  pinMode(13, INPUT_PULLUP);  // Orange wire -- RF TX power on

  // Output setup
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  // Pin state setup
  pin1 = digitalRead(11);
  pin2 = digitalRead(12);
  pin3 = digitalRead(13);

  Serial.print("The pin number 1 was initially ");
  if (pin1 == 0) { Serial.println("off");}
  else { Serial.println("on");}
  Serial.print("The pin number 2 was initially ");
  if (pin2 == 0) { Serial.println("off");}
  else { Serial.println("on");}
  Serial.print("The pin number 3 was initially ");
  if (pin3 == 0) { Serial.println("off");}
  else { Serial.println("on");}
}

void loop() {

  pin1 = check_pin_state_change(pin1, 11);
  pin2 = check_pin_state_change(pin2, 12);
  pin3 = check_pin_state_change(pin3, 13);

  delay(500);
}

int check_pin_state_change(int old_state, int pin_number) {
  /* 
  Checks for a difference between a pin's old state and new state
  If there is a difference between the two, send a message to the console
  In any case, returns new_state 
  */

  int new_state = digitalRead(pin_number);

  if (old_state > new_state) {
    Serial.println("The pin number " + String(pin_number) + " has turned off");
  }

  if (old_state < new_state) {
    Serial.println("The pin number " + String(pin_number) + " has turned on");
  }

  return new_state;
}
