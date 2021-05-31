/*LM75 Temperature C Driver*/
/*Reza Ebrahimi - https://github.com/ebrezadev */
/*Version 1.0*/
#include "lm75.h"
#include <Wire.h>

#define I2C_SPEED                               400000

/*writes an array of data, with length of dataLength to deviceAddress*/
void lm75_write_array(uint8_t deviceAddress, uint8_t *data, uint8_t dataLength)
{
  Wire.beginTransmission(deviceAddress);
  for (uint8_t index = 0; index < dataLength; index++)
  {
    Wire.write(data[index]);
  }
  Wire.endTransmission(deviceAddress);
}

/*reads an array of data, with length of dataLength from deviceAddress*/
void lm75_read_array (uint8_t deviceAddress, uint8_t *data, uint8_t dataLength)
{
  Wire.requestFrom(deviceAddress, dataLength, 0);
  while (Wire.available())
  {
    *data = Wire.read();
    data++;
  }
}

/*initiates the I2C peripheral and sets its speed*/
void lm75_i2c_init()
{
  Wire.begin();
  Wire.setClock(I2C_SPEED);
}

/*implements a delay function in milliseconds*/
void delay_function (uint32_t delayInMS)
{
  delay(delayInMS);
}
