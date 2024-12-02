#include "UltrasonicSensor.h"
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

bool measureDistance(int trigPin, int echoPin, float &distanceCm, float &distanceInch, int relayPin, LiquidCrystal_I2C lcd)
{
    long duration;

    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    // Sets the trigPin on HIGH state for 10 microseconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);

    // Calculate the distances
    distanceCm = duration * SOUND_SPEED / 2;
    distanceInch = distanceCm * CM_TO_INCH;

    // Print the distances to Serial Monitor
    // Serial.print("Distance (cm): ");
    // Serial.println(distanceCm);

    bool isLightOn = distanceCm <= 50;

    if (isLightOn)
    {
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print("Welcome :)");
        digitalWrite(relayPin, HIGH);
    }
    else
    {
     
        digitalWrite(relayPin, LOW);
    }

    delay(500);
    return isLightOn;
}
