/*LM75 Temperature C Driver*/
/*Reza Ebrahimi - https://github.com/ebrezadev */
/*Version 1.0*/
#include "lm75.h"

/*initiates i2c peripheral with a delay, and sets configuration register with a default value*/
void lm75_init (uint8_t deviceAddress)
{
  lm75_i2c_init();
  delay_function(I2C_STARTUP_DELAY);
  uint8_t data = REGISTER_DEFAULT_CONFIGURATION;
  lm75_write_register (deviceAddress, REGISTER_POINTER_CONFIGURATION, &data, REGISTER_LENGTH_CONFIGURATION);        /*sets the configuration register with default values*/
}

/*measures the temperature as a signed 1 byte integer*/
int8_t lm75_measure(uint8_t deviceAddress)
{
  uint8_t data[2];

  lm75_read_register(deviceAddress, REGISTER_POINTER_TEMPERATURE, data, 2);

  return (data[0]);       /*omits the 0.5 C accuracy*/
}

/*measures the temperature as a signed float, with 0.5 C accuracy. RECOMMENDED FOR CPUS WITH FPU*/
float lm75_measure_float(uint8_t deviceAddress)
{
  uint8_t data[2];

  lm75_read_register(deviceAddress, REGISTER_POINTER_TEMPERATURE, data, 2);

  return (data[0] + (data[1] >> 7) * 0.5);        /*implements the 0.5 C accuracy*/
}

/*sets the alarm feature of LM75, hysteresis temperature is HYSTERESIS_RANGE degrees less than alarm temperature. this will not affect power status*/
void lm75_set_alarm(uint8_t deviceAddress, uint8_t noiseThreshold, uint8_t alarmPolarity, uint8_t comparatorOrInterrupt, int8_t alarmTemperature)
{
  uint8_t configurationCommand = 0;

  if (lm75_power_status(deviceAddress) == POWER_OFF)
  {
    configurationCommand |= (1 << CONFIGURATION_BIT_SHUTDOWN);        /*preserving the power status*/
  }
  
  switch (noiseThreshold)
  {
    case NOISE_THRESHOLD_LOW:
      configurationCommand |= (0 << CONFIGURATION_BIT_FAULT0) + (0 << CONFIGURATION_BIT_FAULT1);
      break;
    case NOISE_THRESHOLD_MEDIUM:
      configurationCommand |= (0 << CONFIGURATION_BIT_FAULT0) + (1 << CONFIGURATION_BIT_FAULT1);
      break;
    case NOISE_THRESHOLD_HIGH:
      configurationCommand |= (1 << CONFIGURATION_BIT_FAULT0) + (1 << CONFIGURATION_BIT_FAULT1);
      break;
    default:
      configurationCommand |= (0 << CONFIGURATION_BIT_FAULT0) + (0 << CONFIGURATION_BIT_FAULT1);
      break;
  }

  switch (alarmPolarity)
  {
    case ALARM_POLARITY_LOW:
      configurationCommand |= (0 << CONFIGURATION_BIT_OS_POLARITY);
      break;
    case ALARM_POLARITY_HIGH:
      configurationCommand |= (1 << CONFIGURATION_BIT_OS_POLARITY);
      break;
    default:
      configurationCommand |= (0 << CONFIGURATION_BIT_OS_POLARITY);
      break;
  }

  switch (comparatorOrInterrupt)
  {
    case COMPARATOR_MODE:
      configurationCommand |= (0 << CONFIGURATION_BIT_COMPARATOR_INTERRUPT);
      break;
    case INTERRUPT_MODE:
      configurationCommand |= (1 << CONFIGURATION_BIT_COMPARATOR_INTERRUPT);
      break;
    default:
      configurationCommand |= (0 << CONFIGURATION_BIT_COMPARATOR_INTERRUPT);
      break;
  }

  lm75_write_register(deviceAddress, REGISTER_POINTER_CONFIGURATION, &configurationCommand, REGISTER_LENGTH_CONFIGURATION);

  uint8_t alarmTemp[2] = {alarmTemperature, 0X00};

  lm75_write_register(deviceAddress, REGISTER_POINTER_TOS, alarmTemp, REGISTER_LENGTH_TOS);

  uint8_t hystTemp[2] = {alarmTemperature - HYSTERESIS_RANGE, 0X00};       

  lm75_write_register(deviceAddress, REGISTER_POINTER_THYST, hystTemp, REGISTER_LENGTH_THYST);
}

/*sets the shutdown feature of LM75, for low power use. this will not affect other configuration bits*/
void lm75_power(uint8_t deviceAddress, uint8_t power)
{
  uint8_t configurationRegister;
  lm75_read_register (deviceAddress, REGISTER_POINTER_CONFIGURATION, &configurationRegister, REGISTER_LENGTH_CONFIGURATION);
  switch (power)
  {
    case POWER_ON:
      configurationRegister &= ~(1 << CONFIGURATION_BIT_SHUTDOWN);
      break;
    case POWER_OFF:
      configurationRegister |= (1 << CONFIGURATION_BIT_SHUTDOWN);
      break;
    default:
      configurationRegister &= ~(1 << CONFIGURATION_BIT_SHUTDOWN);
      break;
  }
  lm75_write_register (deviceAddress, REGISTER_POINTER_CONFIGURATION, &configurationRegister, REGISTER_LENGTH_CONFIGURATION);
}

/*gives back the power status, either POWER_ON or POWER_OFF*/
uint8_t lm75_power_status(uint8_t deviceAddress)
{
  uint8_t configurationRegister;
  lm75_read_register (deviceAddress, REGISTER_POINTER_CONFIGURATION, &configurationRegister, REGISTER_LENGTH_CONFIGURATION);

  if ((configurationRegister & (1 << CONFIGURATION_BIT_SHUTDOWN)) == 1)
  {
    return POWER_OFF;
  }
  else
  {
    return POWER_ON;
  }
}

/*function to read one of 4 internal registers of LM75*/
void lm75_read_register (uint8_t deviceAddress, uint8_t pointerAddress, uint8_t *data, uint8_t dataLength)
{
  lm75_write_array(deviceAddress, &pointerAddress, 1);        /*sets the pointer register first*/

  lm75_read_array (deviceAddress, data, dataLength);        /*reads the register*/
}

/*function to write inside one of 3 internal registers of LM75*/
void lm75_write_register (uint8_t deviceAddress, uint8_t pointerAddress, uint8_t *data, uint8_t dataLength)
{
  uint8_t pointerAndData[3];        /*an array with maximum length of 3, containing the pointer address, and 8 or 16 bits of data*/
  pointerAndData[0] = pointerAddress;

  for (uint8_t index = 0; index < dataLength; index++)
  {
    pointerAndData[index + 1] = data[index];
  }
  
  lm75_write_array(deviceAddress, pointerAndData, dataLength + 1);
}
