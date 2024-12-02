#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>

// Function to perform servo movement
void moveServo(Servo &servo, int angleA, int angleB, int step, int delayTime, LiquidCrystal_I2C lcd, int buzzerPin);

#endif
