/*
  I2Cbus.h - Library that implements "bus clear" for an I2C bus
  Created by Maarten Pennings 2018 aug 15
*/
#ifndef __I2CBUS_H_
#define __I2CBUS_H_
 
 
// This routine implements an I2C bus clear on lines `sda` and `scl`.
// On return the pins `sda` and `scl` need to be reassigned for I2C,
// so call Wire.begin() after I2Cbus_clear().
//
// This function returns the realized bus status:
// positive numbers indicate success, negative numbers failure.
// Use I2Cbus_statusstr() to convert status to printable string.
int I2Cbus_clear(int sda, int scl);

// Convert status returned by I2Cbus_clear() into a printable string.
const char * I2Cbus_statusstr(int status);

#endif
