// Digital pin parameters
const int pin12v = 10;  // Red wire -- +12V power on
const int pin5v = 11;   // Green wire -- +5V power on
const int pinTX = 12;   // Orange wire -- RF TX power on
int state12v, state5v, stateTX;

// Basic parameters for the Vishay thermistor
const float BETA = 3977.0;
const float R25 = 10000.0;
const float T25 = 298.15;
const float REXT = 10000.0; // The upper arm of the voltage divider has a 10k resistor

// Variables for calculating average temperature as cumulT/count
// Where 'cumulT' is the cumulative sum of instant temperatures and 'count' the number of summed temperatures 
int count;
float cumulT;



// Functions for digital values management
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


// Functions for temperature management
float temperatureNTC(uint16_t adc, float rfixed) {
  // Formula for converting the adc-value into degrees Celsius
  float a = adc / 1023.0;
  float r = a / (1.0 - a) * rfixed;
  float T = BETA / (logf(r / R25) + BETA / T25);
  return T - 273.15;
}

void print_temperature(float cumulT, int count) {
  // Prints the average temperature calculated over count measures
  Serial.print("T: ");
  Serial.println(cumulT/count);
}



void setup() {
  // Input setup
  pinMode(pin12v, INPUT_PULLUP);
  pinMode(pin5v, INPUT_PULLUP);
  pinMode(pinTX, INPUT_PULLUP);
  pinMode(A0, INPUT);

  // Output setup
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  // Pin state setup
  state12v = digitalRead(pin12v);
  state5v = digitalRead(pin5v);
  stateTX = digitalRead(pinTX);
  cumulT = 0.0;
  count = 0;

  print_pin_state(pin12v);
  print_pin_state(pin5v);
  print_pin_state(pinTX);
}

void loop() {
  // Watchdog tensions monitoring
  state12v = check_pin_state_change(state12v, pin12v);
  state5v = check_pin_state_change(state5v, pin5v);
  stateTX = check_pin_state_change(stateTX, pinTX);

  // Temperature monitoring
  cumulT = cumulT + temperatureNTC(analogRead(A0), REXT);
  count++;

  // Ping receiver
  if (Serial.available() > 0) {
    String request = Serial.readString();
    request.trim();
    
    if (request == "SendT") {
      print_temperature(cumulT, count);
      // Start a new average
      cumulT = 0;
      count = 0;
    }
  }

  delay(500);
}
