#include <LiquidCrystal_I2C.h>

#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

bool measureDistance(int trigPin, int echoPin, float &distanceCm, float &distanceInch, int relayPin, LiquidCrystal_I2C lcd);

#endif
