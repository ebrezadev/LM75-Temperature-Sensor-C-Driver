/*arduino example for LM75 C driver*/
#include "lm75.h"

#define ALARM_TEMPERATURE        80

void setup() {
  Serial.begin(115200);
  
  lm75_init(DEVICE0);       /*from DEVICE0 to DEVICE7, 8 independent devices can be initialized and used, based on A0, A1 and A2 pins setup*/

  lm75_set_alarm(DEVICE0, NOISE_THRESHOLD_MEDIUM, ALARM_POLARITY_LOW, COMPARATOR_MODE, ALARM_TEMPERATURE);

  if (lm75_power_status(DEVICE0) == POWER_OFF)
  {
    lm75_power(DEVICE0, POWER_ON);
  }
}

void loop() {
  Serial.println(lm75_measure_float(DEVICE0));

  delay(500);
}
