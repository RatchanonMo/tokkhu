#include "TimeoutHandler.h"
#include <Arduino.h>
#include <ArduinoJson.h>

void handleTimeout(unsigned long currentTime, unsigned long &lastRequestTime, bool &isServoMoving, JsonArray &currentClass, unsigned long timeoutDuration)
{
    if (currentTime - lastRequestTime > timeoutDuration)
    {
        isServoMoving = false;
        currentClass.clear();
    }
}
