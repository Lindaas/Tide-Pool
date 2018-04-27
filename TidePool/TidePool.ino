/*
  Tide Pool 
  
  This program will take the inputs from two switches and control indicator 
  lights as well as pumps.  We want to have the time between low and high tide 
  roughly 6.5 hours.  We don't know exactly how long it will take the little 
  pumps to fill or drain.  So we will cycle them on/off and record the final time 
  and then stay at high or low tide until 6.5 hours is reached. 

  You can change the default of whether the tide is rising when the microcontroller 
  first initializes.
  
  The goal is to simulate a tidal cycle (~13 hours)

  Inputs ---
  Pin 2: Lower switch +5V means water is too low 
  Pin 3: High switch +5V means water is too high

  Pin 8: Reference (+5 V)

  Outputs ---
  Pin 10: Blue LED turn on when high switch is selected
  Pint 11: Green LED turn on when low switch is selected

  Pin 12: Relay IN2 = Left switch with infow pump (raising)
  Pin 13: Relay IN1 = Right switch with outflow pump (lowering)

  
  Author: Steve Lindaas - based on circuit and original program idea by Ben LeMay
  Version 1.0 April 9 2018  
  Additional Author: Paige Meyer
  Version 1.3 April 10 2018
*/

#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

boolean start;

// Pin constants won't change. 
const int InHigh =  3;
const int InLow  =  2;

const int InRef  = 8;

const int HighLED = 10;
const int LowLED  = 11;

const int PumpLeft = 12;    // This pump fills the pool
const int PumpRight = 13;   // This pump empties the pool


// Generally, you should use "unsigned long" for variables that hold time
// since the value will quickly become too large for an int to store
unsigned long startTime = 0;                        // store the tide half-cycle starting time 
unsigned long previousTime = 0;                     // store the time since the pump was either turned on or off.
 
const unsigned long TideInterval = 6.5*60*60*1000;        // 6.5 hours is the half-period of a tide (in msec)
const unsigned long pumpOnTime = 5.0*60*1000;               // Pump is ON for 5 minutes
const unsigned long pumpOffTime = 5.0*60*1000;              // Pump is OFF for 5 minutes

// Tide state variables
boolean HighTide = false;
boolean LowTide = true;
boolean Rising = false;              // Default rising tide when you first turn on the system
boolean PumpOn = false;

// Sensor varialbles - Initially set all the switches LOW (not tripped)
int sensorHigh = LOW;
int sensorLow = LOW;
int sensorRef = LOW;
// Set the last sensor states
int lastSensorHigh = sensorHigh;
int lastSensorLow = sensorLow;
int lastSensorRef = sensorRef; 
unsigned long lastTimeHigh = 0;
unsigned long lastTimeLow = 0;
unsigned long lastTimeRef = 0;
unsigned long debounceDelay = 100;

// Indicator light variables
unsigned long timeLED = 0;
int stateLED = LOW;             // ledState used to set the LED
long blinkLED = 500;           // interval at which to blink (milliseconds)
const long blinkFast = 150;
const long blinkSlow = 1000;

// Run initial setup of variables
void setup() {
  // Start serial connection so that we can print to the LCD screen for testing
  Serial.begin(115200);
  
  lcd.begin(16, 2);     // set up the LCD's number of columns and rows
  
  // Set the various digital input pins
  pinMode( InHigh, INPUT );
  pinMode( InLow, INPUT );
  pinMode( InRef, INPUT );
  
  // Set the various digital output pins
  pinMode( HighLED, OUTPUT );
  pinMode( LowLED, OUTPUT );
  pinMode( PumpLeft, OUTPUT );
  pinMode( PumpRight, OUTPUT );
  start = true;
}

// Main program
void loop() {
  // Read the current time... everything is testing time intervals.
  unsigned long currentTime = millis();

  // If you are at High or Low tide you need to wait until the tide interval is complete.  The time has to reach  
  // the end of the TideInterval.

  // initializes variables at the start of a rising or falling tide
  if ( (currentTime - startTime) >= TideInterval || start ) {
    start = false;          // start variable makes sure this loop is executed at the beginning when t=0
      
    if ( Rising ) {         // Possibly the high and low tide sensor switches never get 
      Rising = false;       // triggered in the TideInteval (testing situation)
    } else {
      Rising = true;
    }
    
    if ( HighTide ) {
      HighTide = false;
      Rising = false;
    } else if ( LowTide ) {
      LowTide = false;
      Rising = true;
    }

    startTime = currentTime;  // Reset the start of the tide cycle
    previousTime = currentTime;
    
    PumpOn = true;            // Turn the pump on to either raise or lower the tide
    blinkLED = blinkFast;

    // update LCD Screen
    lcd.clear();            // resets cursor to (0,0)
    if (Rising) {
      lcd.print("R Done Draining");
    }
    else {
      lcd.print("R Done Filling");
    }
    lcd.setCursor(0,1);
    if (sensorHigh) {
      lcd.print("High Water!");
    }
    else if (sensorLow) {
      lcd.print("Low Water!");
    }
    Serial.print("Tide interval is over. The variable Rising is ");
    Serial.println( Rising );
    Serial.print("   FYI: High Water Sensor = ");
    Serial.println(sensorHigh);
    Serial.print("   FYI: Low Water Sensor = ");
    Serial.println(sensorLow);
    Serial.print("   FYI: Reference Sensor = ");
    Serial.println(sensorRef);
  }

  // Now we need to check our sensors.... but make sure to debounce the readings
  // 
  int readingSensorHigh = digitalRead(InHigh);
  int readingSensorLow = digitalRead(InLow);
  int readingSensorRef = digitalRead(InRef);
    
  // Check to see if the switches changed state due to either noise or water level
  // 
  if ( readingSensorHigh != lastSensorHigh ) {
    // Set the time and then we will will wait to see if the state change is stable
    lastTimeHigh = currentTime;
  }
  // If enough time has passed then the sensor is not fluctuating
  if ( (currentTime - lastTimeHigh) > debounceDelay ) {
    if ( readingSensorHigh != sensorHigh ) {
        sensorHigh = readingSensorHigh;
        // If Sensor reading switched than print out an update        
        lcd.setCursor(0,1);
        if (sensorHigh) {
          lcd.print("High Water!     ");
          lcd.setRGB(0, 255, 0);
        }
        else {
          lcd.print("                 ");
          lcd.setRGB(0, 0, 255);
        }
   
        Serial.print("High Water Sensor = ");
        Serial.println(sensorHigh);
    }
  }
  lastSensorHigh = readingSensorHigh;  

  if ( readingSensorLow != lastSensorLow ) {
    // Set the time and then we will will wait to see if the state change is stable
    lastTimeLow = currentTime;
  }
  // If enough time has passed then the sensor is not fluctuating
  if ( (currentTime - lastTimeLow) > debounceDelay ) {
    if ( readingSensorLow != sensorLow ) {
        sensorLow = readingSensorLow;
        // If Sensor reading switched than print out an update 
        lcd.setCursor(0,1);
        if (sensorLow) {
          lcd.print("Low Water!       ");
          lcd.setRGB(0, 255, 0);
        }
        else {
          lcd.print("                 ");
          lcd.setRGB(0, 0, 255);
        }
        
        Serial.print("Low Water Sensor = ");
        Serial.println(sensorLow);
        lcd.setRGB(0, 255, 0);
    }
  }
  lastSensorLow = readingSensorLow;  
    
  if ( readingSensorRef != lastSensorRef ) {
    // Set the time and then we will will wait to see if the state change is stable
    lastTimeRef = currentTime;
  }
  // If enough time has passed then the sensor is not fluctuating
  if ( (currentTime - lastTimeRef) > debounceDelay ) {
    if ( readingSensorRef != sensorRef ) {
        sensorRef = readingSensorRef;
        // If Sensor reading switched than print out an update
        Serial.print("Reference Sensor = ");
        Serial.println(sensorRef);
    }
  }
  lastSensorRef = readingSensorRef;  

  /* 
   *  Add a user programming feature here where you could action based on both triggers set. 
   *  Could enter program mode with input set by toggling switches.
   *  
   *  Not implemented yet... 
   */

  // Set the HighTide or LowTide boolean values and then turn off the appropriate pump
  // Indicator light: solid LED if a sensor is set, blink the HighLED if you are Rising 
  // and blink the LowLED if you are not Rising 
  //
  if ( sensorHigh == HIGH ) {
    HighTide = true;
    digitalWrite( HighLED, HIGH);
    // Turn the PumpLeft off since we want to stop filling the pool
    digitalWrite( PumpLeft, LOW);
  } else {
    HighTide = false;
    digitalWrite( HighLED, LOW);
  }
  
  if (sensorLow == HIGH) {
    LowTide = true;
    digitalWrite( LowLED, HIGH);
    // Turn the PumpRight off since we want to stop draining the pool
    digitalWrite( PumpRight, LOW);
  } else {
    LowTide = false;
    digitalWrite( LowLED, LOW);
  }

  // if both the low sensor and high sensor are tripped, we have an error
  if (sensorLow == HIGH and sensorHigh == HIGH) {
    lcd.setCursor(0, 1);
    lcd.print("SENSOR ERROR    ");
    lcd.setRGB(255, 0, 0);
  }

  // Blink the indicator LED if it is not high or low tide
  if ( !HighTide && !LowTide ) {
    if ( (currentTime - timeLED) >= blinkLED ) {
      // if the LED is off turn it on and vice-versa:
      if ( stateLED == LOW) {
        stateLED = HIGH;
      } else {
        stateLED = LOW;
      }
      timeLED = currentTime;
    }
    if ( Rising ) {
      digitalWrite( HighLED, stateLED);
    } else {
      digitalWrite( LowLED, stateLED);
    }
  }


  /* 
   *  Need to wait until the High or Low tide time interval has been met. Then start a pump.
   *  We want to cycle the correct pump on and off with the cycle time. 
   *  
   *  PumpLeft fills the pool
   *  PumpRight empties the pool
  */
  
  if ( Rising && !HighTide ) {
    if( PumpOn && (currentTime - previousTime) >= pumpOnTime ) {
      PumpOn = false;
      blinkLED = blinkSlow;
      previousTime = currentTime;
      
      // print if is paused
      lcd.setCursor(0,0);
      lcd.print("Pause Filling R ");
      
      Serial.println("Turning PumpLeft off - pause filling");
    } else if ( !PumpOn && (currentTime - previousTime) >= pumpOffTime ) {
      PumpOn = true;
      blinkLED = blinkFast;
      previousTime = currentTime;

      // print if is filling
      lcd.setCursor(0,0);
      lcd.print("Continue Filling R");
      
      Serial.println("Turning PumpLeft on - continue filling");
    }

    // TODO: Shouldn't this be in timed
    if ( PumpOn ) {
      // Pump stays on for the pumpOnTime
      digitalWrite( PumpLeft, HIGH);
    } else {
      // Pump stays off for the pumpOffTime
      digitalWrite( PumpLeft, LOW);
    } 
  }
  if ( !Rising && !LowTide ) {
    if( PumpOn && (currentTime - previousTime) >= pumpOnTime ) {
      PumpOn = false;
      blinkLED = blinkSlow;
      previousTime = currentTime;

      // print if puased draining
      lcd.setCursor(0,0);
      lcd.print("Pause Draining R");
      
      Serial.println("Turning PumpRight off - pause draining");
    } else if ( !PumpOn && (currentTime - previousTime) >= pumpOffTime ) {
      PumpOn = true;
      blinkLED = blinkFast;
      previousTime = currentTime;
      
      // print if is draining
      lcd.setCursor(0,0);
      lcd.print("Continue Draining R");
      
      Serial.println("Turning PumpRight on - continue draining");
    }

    // TODO: shouldn't this be in timed conditional?
    if ( PumpOn ) {
      // Pump stays on for the pumpOnTime
      digitalWrite(PumpRight, HIGH);
    } else {
      // Pump stays off for the pumpOffTime
      digitalWrite(PumpRight, LOW);
    } 
  }
}  
// End of void() loop 
