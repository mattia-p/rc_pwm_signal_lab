// Variables used to store the timestamp of the rising edge and the calculated period
volatile unsigned long lastRiseTime = 0;
volatile unsigned long period = 0;

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
}


void loop() {
    static unsigned long lastRun = 0;
    
    // Print the pulse every 50 milliseconds
    if (millis() - lastRun >= 50)
    {
        lastRun = millis(); // Update the last run time

        // Output to serial monitor
        // Serial.print("Period: ");
        Serial.println(period);
        // Serial.println(" us");
    }
}
