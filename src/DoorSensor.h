#ifndef DOOR_SENSOR_H
#define DOOR_SENSOR_H

void handleDoorSensor(int doorSensorPin, int buzzerPin, unsigned long debounceDuration, bool isLightOn);

#endif
