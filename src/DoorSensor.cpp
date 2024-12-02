#include "DoorSensor.h"
#include <Arduino.h>

void handleDoorSensor(int doorSensorPin, int buzzerPin, unsigned long debounceDuration, bool isLightOn)
{
    static unsigned long doorHighStartTime = 0;

    int doorValue = digitalRead(doorSensorPin); // Read sensor value
    Serial.println(doorValue);
    if (doorValue == HIGH)
    { // Sensor triggered
        if (doorHighStartTime == 0)
        {
            doorHighStartTime = millis(); // Start timing
        }
        else if (millis() - doorHighStartTime >= debounceDuration)
        {
            if (!isLightOn)
                digitalWrite(buzzerPin, HIGH); // Trigger buzzer
        }
    }
    else
    {
        digitalWrite(buzzerPin, LOW); // Turn off buzzer
        doorHighStartTime = 0;        // Reset timer if sensor is not triggered
    }
}
