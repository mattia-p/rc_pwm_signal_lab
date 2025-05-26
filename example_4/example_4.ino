
// Variables for averaging
volatile unsigned long steeringHighTimeSum = 0; 
volatile unsigned int steeringHighTimeCount = 0;

volatile unsigned long throttleHighTimeSum = 0; 
volatile unsigned int throttleHighTimeCount = 0;

volatile unsigned long engageHighTimeSum = 0; 
volatile unsigned int engageHighTimeCount = 0;

unsigned long steeringRiseTime = 0;
unsigned long throttleRiseTime = 0;
unsigned long engageRiseTime = 0;

// Pin connected to PWM signal. On the ArduinoMega, Pin 18 is interrupt 5
const int steering_interrupt_pin = 18; // Pin 18 is interrupt 5
const int throttle_interrupt_pin = 19; // Pin 19 is interrupt 4
const int engage_interrupt_pin = 3; // Pin 3 is interrupt 1


void setup(){
  Serial.begin(9600); // Initialize serial communication for debugging

  // Set all interrupt pins as inputs
  pinMode(steering_interrupt_pin, INPUT); 
  pinMode(throttle_interrupt_pin, INPUT); 
  pinMode(engage_interrupt_pin, INPUT); 

  // Attach an interrupt to teach pin
  attachInterrupt(digitalPinToInterrupt(steering_interrupt_pin), SteeringInterruptServiceRoutine, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(throttle_interrupt_pin), ThrottleInterruptServiceRoutine, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(engage_interrupt_pin), EngageInterruptServiceRoutine, CHANGE);
}


void SteeringInterruptServiceRoutine() {

  unsigned long now = micros();
  bool currentState = digitalRead(steering_interrupt_pin);

  if (currentState == HIGH){
    // Rising edge: store the timestamp
    steeringRiseTime = now;
  } else{
    // Falling edge: compute time spent HIGH
    unsigned long duration = now - steeringRiseTime;

    // Could potentially ignore pulses shorter than x or larger than y

    steeringHighTimeSum += duration;
    steeringHighTimeCount++;
  }
}

void ThrottleInterruptServiceRoutine(){
  unsigned long now = micros();
  bool currentState = digitalRead(throttle_interrupt_pin);

  if (currentState == HIGH){
    // Rising edge: store the timestamp
    throttleRiseTime = now;
  } else{
    // Falling edge: compute time spent HIGH
    unsigned long duration = now - throttleRiseTime;

    // Could potentially ignore pulses shorter than x or larger than y

    throttleHighTimeSum += duration;
    throttleHighTimeCount++;
  }
}

void EngageInterruptServiceRoutine(){
  unsigned long now = micros();
  bool currentState = digitalRead(engage_interrupt_pin);

  if (currentState == HIGH){
    // Rising edge: store the timestamp
    engageRiseTime = now;
  } else{
    // Falling edge: compute time spent HIGH
    unsigned long duration = now - engageRiseTime;

    // Could potentially ignore pulses shorter than x or larger than y

    engageHighTimeSum += duration;
    engageHighTimeCount++;
  }
}


void loop() {
  static unsigned long lastRun = 0;

  // Print the pulse every 50 milliseconds
  if (millis() - lastRun >= 50)
  {
    noInterrupts();

    unsigned long steeringAverageHighTime = 0;
    if (steeringHighTimeCount > 0){
      steeringAverageHighTime = steeringHighTimeSum / steeringHighTimeCount;
    }

    unsigned long throttleAverageHighTime = 0;
    if (throttleHighTimeCount > 0){
      throttleAverageHighTime = throttleHighTimeSum / throttleHighTimeCount;
    }

    unsigned long engageAverageHighTime = 0;
    if (engageHighTimeCount > 0){
      engageAverageHighTime = engageHighTimeSum / engageHighTimeCount;
    }

    // Reset for next averaging period
    steeringHighTimeSum = 0; 
    steeringHighTimeCount = 0;
    throttleHighTimeSum = 0; 
    throttleHighTimeCount = 0;
    engageHighTimeSum = 0; 
    engageHighTimeCount = 0;

    interrupts();

    lastRun = millis(); // Update the last run time
  
    Serial.print(steeringAverageHighTime);
    Serial.print(",");
    Serial.print(throttleAverageHighTime);
    Serial.print(",");
    Serial.println(engageAverageHighTime);
  }
}