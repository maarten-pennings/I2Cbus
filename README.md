# I2C bus clear
Arduino library library that implements "bus clear" for an I2C bus.

Based on the work at http://www.forward.com.au/pfod/ArduinoProgramming/I2C_ClearBus/index.html


## Introduction
In rare occasions, an I2C bus can deadlock.
One such occasion is when a master is reading a byte from a slave, the slave is pulling the SDA line low, and then the master is reset.
The slave waits for an SCL pulse (from the master), but the master does not send clock pulses, because it sees the bus busy: SDA is low.

To get out of this deadlock, this library implements a bus clear feature.
Note that `I2cbus_clear()` returns a status code: negative for not being able to clear the bus, or positive for success.
Note also that `I2cbus_clear()` must be followed by `Wire.begin()` to assign the Arduino pins from GPIO to I2C.

(end of doc)
