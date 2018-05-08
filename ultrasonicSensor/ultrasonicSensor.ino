/*
  Ultrasonic Sensor

  Demonstrates analog input by reading an analog sensor on analog pin 0 and
  turning on and off a light emitting diode(LED) connected to digital pin 13.
  The amount of time the LED will be on and off depends on the value obtained
  by analogRead().

  The circuit:
  - LED attached from pin 13 to ground
  - pushbutton attached from pin 2 to +5V
  - 10 kilohm resistor attached from pin 2 to ground

  - Note: On most Arduino boards, there is already an LED on the board connected
    to pin 13, so you don't need any extra components for this example.
*/

// constants won't change. They're used here to set pin numbers:
const int triggerPin = 10;    // the number of the trigger pin
const int echoPin = 11;      // the number of the echo pin
const int ledPin = 13;        // the ledPin

void setup() {
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
  long duration, distance;
  digitalWrite(triggerPin, LOW);
  digitalWrite(triggerPin, HIGH);
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/58.2);    // Change time to get pulse back to centimeters
  
  if (distance >= 200 || distance <= 4){
    digitalWrite(ledPin,HIGH);
    
    Serial.println("Out of range");
  }
  else {
    
    digitalWrite(ledPin,LOW);
    
    Serial.print(distance);
    Serial.println(" cm");
  }

  delay(1000);
}

