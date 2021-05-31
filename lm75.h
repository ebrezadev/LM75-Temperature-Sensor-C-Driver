/*LM75 Temperature C Driver*/
/*Reza Ebrahimi - https://github.com/ebrezadev */
/*Version 1.0*/
#ifndef LM75_H
#define LM75_H

#define DEVICE0                                 0X48
#define DEVICE1                                 0X49
#define DEVICE2                                 0X4A
#define DEVICE3                                 0X4B
#define DEVICE4                                 0X4C
#define DEVICE5                                 0X4D
#define DEVICE6                                 0X4E
#define DEVICE7                                 0X4F

#define REGISTER_POINTER_TEMPERATURE            0X00
#define REGISTER_POINTER_CONFIGURATION          0X01
#define REGISTER_POINTER_THYST                  0X02
#define REGISTER_POINTER_TOS                    0X03

#define REGISTER_LENGTH_TEMPERATURE             0X02
#define REGISTER_LENGTH_CONFIGURATION           0X01
#define REGISTER_LENGTH_THYST                   0X02
#define REGISTER_LENGTH_TOS                     0X02

#define REGISTER_DEFAULT_CONFIGURATION          0X00
#define I2C_STARTUP_DELAY                       1000

#define HYSTERESIS_RANGE                        0X05

#define NOISE_THRESHOLD_LOW                     0X00
#define NOISE_THRESHOLD_MEDIUM                  0X01
#define NOISE_THRESHOLD_HIGH                    0X02

#define ALARM_POLARITY_LOW                      0X00
#define ALARM_POLARITY_HIGH                     0X01

#define COMPARATOR_MODE                         0X00
#define INTERRUPT_MODE                          0X01

#define ALARM_TEMPERATURE_DEFAULT               0X55

#define CONFIGURATION_BIT_FAULT0                0X03
#define CONFIGURATION_BIT_FAULT1                0X04
#define CONFIGURATION_BIT_OS_POLARITY           0X02
#define CONFIGURATION_BIT_COMPARATOR_INTERRUPT  0X01
#define CONFIGURATION_BIT_SHUTDOWN              0X00

#define POWER_ON                                0X01
#define POWER_OFF                               0X00

void lm75_i2c_init();
void lm75_write_array(uint8_t deviceAddress, uint8_t *data, uint8_t dataLength);
void lm75_read_array(uint8_t deviceAddress, uint8_t *data, uint8_t dataLength);
void delay_function(uint32_t delayInMS);

void lm75_init(uint8_t deviceAddress);
void lm75_read_register(uint8_t deviceAddress, uint8_t pointerAddress, uint8_t *data, uint8_t dataLength);
void lm75_write_register(uint8_t deviceAddress, uint8_t pointerAddress, uint8_t *data, uint8_t dataLength);
void lm75_set_alarm(uint8_t deviceAddress, uint8_t noiseThreshold, uint8_t alarmPolarity, uint8_t comparatorOrInterrupt, int8_t alarmTemperature);
void lm75_power(uint8_t deviceAddress, uint8_t power);
uint8_t lm75_power_status(uint8_t deviceAddress);
int8_t lm75_measure(uint8_t deviceAddress);
float lm75_measure_float(uint8_t deviceAddress);

#endif
