#ifndef __SERVO_H
#define __SERVO_H

#include "gpio.h"



#define SERVO_MIN_ANGLE 400   //PWM value for 0 degree
#define SERVO_MAX_ANGLE 3000  //PWM value for 180 degree


void SetServoPosition(uint32_t angle);


void pwm(uint32_t period, uint16_t prescaler);
void servoPulse(uint32_t pulse);

#endif
