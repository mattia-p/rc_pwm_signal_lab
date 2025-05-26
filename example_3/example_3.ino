// Variables used to store the timestamp of the rising edge and the calculated period
volatile unsigned long lastRiseTime = 0;
volatile unsigned long period = 0;

// Variables for averaging
volatile unsigned long periodSum = 0;
volatile unsigned int periodCount = 0;

// Pin connected to PWM signal. On the ArduinoMega, Pin 18 is interrupt 5
const int interrupt_pin = 18; // Pin 18 is interrupt 5


void setup(){
    Serial.begin(9600); // Initialize serial communication for debugging
    pinMode(interrupt_pin, INPUT); // Set the interrupt pin as an input

    // Attach an interrupt to the pin, triggering on the RISING edge only
    attachInterrupt(digitalPinToInterrupt(interrupt_pin), InterruptServiceRoutine, RISING);
}


void InterruptServiceRoutine() {
    // Capture the current time in microseconds
    unsigned long currentTime = micros();

    // Calculate the time elapsed since the last rising edge (i.e., the period of the signal)
    period = currentTime - lastRiseTime;

    // Update the last rise time for the next calculation
    lastRiseTime = currentTime;

    // Accumulate period for averaging
    periodSum += period;
    periodCount++;
}


void loop() {
    static unsigned long lastRun = 0;
    
    // Print the pulse every 50 milliseconds
    if (millis() - lastRun >= 50)
    {
      noInterrupts();

      unsigned long averagePeriod = 0;
      if (periodCount > 0){
        averagePeriod = periodSum / periodCount;
      }

      // Reset for next averaging period
      periodSum = 0;
      periodCount = 0;
      interrupts();

      lastRun = millis(); // Update the last run time

      // Output to serial monitor
      // Serial.print("Period: ");
      
      Serial.println(averagePeriod);
      // Serial.println(" us");
    }
}
