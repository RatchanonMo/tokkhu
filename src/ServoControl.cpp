#include "ServoControl.h"
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// Function implementation for servo movement
void moveServo(Servo &servo, int angleA, int angleB, int step, int delayTime, LiquidCrystal_I2C lcd, int buzzerPin)
{
 
    for (int angle = angleA; angle <= angleB; angle += step)
    {
        digitalWrite(buzzerPin, HIGH); // Trigger buzzer
        servo.write(angle);
        delay(delayTime);
        digitalWrite(buzzerPin, LOW); // Trigger buzzer
    }
    for (int angle = angleB; angle >= angleA; angle -= step)
    {
        digitalWrite(buzzerPin, HIGH); // Trigger buzzer
        servo.write(angle);
        delay(delayTime);
        digitalWrite(buzzerPin, LOW); // Trigger buzzer
    }
}
