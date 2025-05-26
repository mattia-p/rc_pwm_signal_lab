// Variables used to store the timestamp of the rising edge and the calculated pulse width
volatile unsigned long pulseStart = 0;
volatile unsigned long pulseWidth = 0;

// Pin connected to PWM signal. On the ArduinoMega, Pin 18 is interrupt 5
const int interrupt_pin = 18;

void setup() {
  Serial.begin(9600); // Initialize serial communication for debugging
  pinMode(interrupt_pin, INPUT); // Set the interrupt pin as an input

  // Attach an interrupt to the pin, triggering on both RISING and FALLING edges
  attachInterrupt(digitalPinToInterrupt(interrupt_pin), InterruptServiceRoutine, CHANGE); 
}

void InterruptServiceRoutine() {
  // Check if the signal just went HIGH (rising edge)
  if (digitalRead(interrupt_pin) == HIGH) {
    pulseStart = micros();  // Record timestamp of rising edge
  } else {
    // If the signal just went LOW (falling edge), calculate the pulse width
    pulseWidth = micros() - pulseStart;
  }
}

void loop() {

 static unsigned long lastRun = 0;

  // Print the pulse every 50 milliseconds
  if (millis() - lastRun >= 50)
  {
    lastRun = millis(); // Update the last run time

    // Output to serial monitor
    // Serial.print("Pulse width: ");
    Serial.println(pulseWidth);
    // Serial.println(" us"); 
  }
}