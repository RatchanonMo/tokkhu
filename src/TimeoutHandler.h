#ifndef TIMEOUT_HANDLER_H
#define TIMEOUT_HANDLER_H

#include <Arduino.h>
#include <ArduinoJson.h>

void handleTimeout(unsigned long currentTime, unsigned long &lastRequestTime, bool &isServoMoving, JsonArray &currentClass, unsigned long timeoutDuration);

#endif
