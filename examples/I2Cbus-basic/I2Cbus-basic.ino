/*
  I2Cbus-basic.ino - Basic skeleton showing how to bus clear
  Created by Maarten Pennings 2018 aug 15
*/


#include <Wire.h>   // I2C library
#include "I2Cbus.h" // I2Cbus library


int i2cbusstatus;


void setup() {
  // Enable serial
  Serial.begin(115200);
  Serial.println("");
  Serial.println("Starting I2Cbus basic demo");

  // Clear bus before Wire.begin()
  i2cbusstatus= I2Cbus_clear(SDA,SCL);
  Serial.println( I2Cbus_statusstr(i2cbusstatus) );
  
  // Enable I2C
  Wire.begin(SDA,SCL); // For ESP8266 NodeMCU boards [VDD to 3V3, GND to GND, SDA to D2, SCL to D1]
  //Wire.begin(); // Arduino pro mini or Nano [VDD to VCC/3V3, GND to GND, SDA to A4, SCL to A5]

  // Setup complete
  Serial.println("setup() completed");
}


void loop() {
  if( i2cbusstatus<0 ) Serial.println("Fatal error for I2C bus");
  if( i2cbusstatus>0 ) Serial.println("loop() can use I2C bus");
  delay(5000);
}

