/*
  I2Cbus.c - Arduino library that implements "bus clear" for an I2C bus
  Created by Maarten Pennings 2018 aug 15
  Based on the work at http://www.forward.com.au/pfod/ArduinoProgramming/I2C_ClearBus/index.html
*/


#include <Arduino.h>
#include "I2Cbus.h"


int I2Cbus_clear(int sda, int scl) {
  #if defined(TWCR) && defined(TWEN)
    // Disable the Atmel 2-Wire interface so we can control the SDA and SCL pins directly
    TWCR &= ~(_BV(TWEN)); 
  #endif
 
  // Make SDA pin (data) and SCL pin (clock) inputs with pull-up.
  pinMode(sda, INPUT_PULLUP); 
  pinMode(scl, INPUT_PULLUP);

  #if 0  
    // This is strictly only necessary on the first power
    // up of the DS3231 module to allow it to initialize properly,
    // but is also assists in reliable programming of FioV3 boards as it gives the
    // IDE a chance to start uploaded the program
    // before existing sketch confuses the IDE by sending Serial data.
    delay(2500);  // Wait 2.5 secs.
  #endif
  
  // Check status of SCL pin
  int scl_low = digitalRead(scl)==LOW; 
  if( scl_low ) { 
    // Can not correct SCL line being low
    return -1; 
  }
 
  // Check status of SDA
  int sda_low = digitalRead(sda)==LOW;
  if( !scl_low && !sda_low ) {
    // Bus is ok (SDA and SCL high)
    return 1; 
  }
 
  // Give SCL pulses, see https://www.nxp.com/docs/en/user-guide/UM10204.pdf#page=20
  int clockCount = 20; // > 2x9 clock
  while( sda_low && (clockCount>0) ) { 
    clockCount--;
    // Note: I2C bus is open collector so do NOT drive scl or sda high.
    pinMode(scl, INPUT);        // release scl pullup so that when made output it will be LOW
    pinMode(scl, OUTPUT);       // then clock scl Low
    delayMicroseconds(10);      // for >5uS
    pinMode(scl, INPUT);        // release scl LOW
    pinMode(scl, INPUT_PULLUP); // turn on pullup resistors again
    // Do not force high as slave may be holding it low for clock stretching.
    delayMicroseconds(10);      //  for >5uS
    // The >5uS is so that even the slowest I2C devices are handled.
    scl_low = digitalRead(scl)==LOW; // Check if scl is Low.
    int counter = 20;
    while (scl_low && (counter > 0)) {  // loop waiting for scl to become High only wait 2sec.
      counter--;
      delay(100);
      scl_low = digitalRead(scl)==LOW;
    }
    if (scl_low) { // still low after 2 sec error
      // I2C bus error. Could not clear. SCL clock line held low by slave clock stretch for >2sec
      return -2; 
    }
    sda_low = digitalRead(sda)==LOW; 
  }
  
  // Recheck SDA
  if( sda_low ) { 
    // I2C bus error. Could not clear. SDA data line held low
    return -3; 
  }
 
  // Pull sda line low for Start or Repeated Start
  pinMode(sda, INPUT); // remove pullup.
  pinMode(sda, OUTPUT);  // and then make it LOW i.e. send an I2C Start or Repeated start control.
  // When there is only one I2C master a Start or Repeat Start has the same function as a Stop and clears the bus.
  delayMicroseconds(10); // wait >5uS
  pinMode(sda, INPUT); // remove output low
  pinMode(sda, INPUT_PULLUP); // and make sda high i.e. send I2C STOP control.
  delayMicroseconds(10); // wait >5uS
  pinMode(sda, INPUT); // and reset pins as tri-state inputs which is the default state on reset
  pinMode(scl, INPUT);
  
  // Bus repaired from low SDA
  return 2; 
}


const char * I2Cbus_statusstr(int status) {
  if(      status==-1 ) return "Can not correct SCL being low";
  else if( status==+1 ) return "Ok (SDA and SCL are high)";
  else if( status==-2 ) return "SCL held low by slave clock stretch for >2 sec";
  else if( status==-3 ) return "SDA still low after bus clear";
  else if( status==+2 ) return "Bus repaired from low SDA";
  else                  return "Illegal status code";
}

