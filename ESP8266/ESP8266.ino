// Adafruit Feather HUZZAH esp8266 Code

#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

#define rxPin 12
#define txPin 13
SoftwareSerial incoming(rxPin, txPin);
AdafruitIO_Feed *ultrasonicSensorFeed = io.feed("UltrasonicSensorFeed");

void setup() {
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  
  Serial.begin(9600);
  
  Serial.println();
  Serial.println("Serial begun.");
  incoming.begin(9600);

  

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
  
#ifndef ESP8266
  while (!incoming);     // will pause Zero, Leonardo, etc until serial console opens  Serial.begin(115200);
#endif
  
}


void loop() {
  float distance;
  char letter;
  int i = 0;

  
  io.run();

  if (incoming.available()) {
    
    Serial.print("-");
    delay(10); //allows all serial sent to be received together
    distance = incoming.parseFloat();

    if (distance > 0) {
      Serial.print("Distance: ");
      Serial.println(distance);
    
      ultrasonicSensorFeed->save(distance);
    }
    
    //sendToWeb(message);
  }
}

/*
void sendToWeb(String mssg) {
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("Connection failed!");
    return;
  }
  // Prep message for url arguement
  mssg.replace(" ", "+");
  mssg.replace("'", "%27");

  /*String request = "";
  request += "GET ";
  request += "/recordMssg.php?mssg=" + mssg;
  request += " HTTP/1.1\r\n";
  request += "Host: " ;
  request += host;
  request += "\r\nConnection: close\r\n\r\n";
  Serial.println(request);
  ultrasonicSensorFeed->save(mssg);
  /*client.print(request);
  delay(1000);
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }*/


