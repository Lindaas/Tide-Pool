// Adafruit IO Publish Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

// pin states
//bool currentTrigger = false;
//bool last = false;
const int TRIGGER_PIN = 12;
const int ECHO_PIN = 13;

// TODO: update communication so time can be sent
// set up the 'timeFeed' feed
AdafruitIO_Feed *timeFeed = io.feed("timeFeed");
// set up the 'UltrasonicSensorFeed' feed
AdafruitIO_Feed *ultrasonicSensorFeed = io.feed("UltrasonicSensorFeed");

// time variable
unsigned long lastTimeUpdated = 0;
short delayUpdate = 3000;               // update only every 3000 milliseconds (3 seconds)

void setup() {
  // set trigger and echo pins as an input
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();
  
  // update online log with water level if over delayUpdate seconds
  if((millis() - lastTimeUpdated) > delayUpdate) {
    float duration, distance;
    
    // send ultrasonic pulse
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
    
    // see how long pulse takes to return to echo pin
    duration = pulseIn(ECHO_PIN, HIGH);
    distance = (duration / 2) / 29.1;    // Change time to get pulse back to centimeters
    Serial.print("duration->");
    Serial.println(duration);
    
    // save count to the 'distance' feed on Adafruit IO
    Serial.print("level sending -> ");
    Serial.println(distance);
    ultrasonicSensorFeed->save(distance);

    // update last time updated
    lastTimeUpdated = millis();
  }
  
}
