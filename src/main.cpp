#include <ESP32Servo.h>
#include "WiFiSetup.h"
#include "ServerSetup.h"
#include "TimeoutHandler.h"
#include "ServoControl.h"
#include "DoorSensor.h"
#include "UltrasonicSensor.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <HTTPClient.h>

// Define the URL for the POST request
const char *postUrl = "https://tokkhu-discord.vercel.app/";

#define I2C_SDA 25
#define I2C_SCL 26

#define DOOR_SENSOR_PIN 14

#define BUZZER_PIN 12
#define RELAY_PIN 4
#define LED_PIN 21

#define SMOKE_PIN 35

#define TRIGGER_PIN 32
#define ECHO_PIN 33

#define SERVO_PIN 27
const unsigned long debounceDuration = 5000;
const int angleA = 0;
const int angleB = 180;

#define DHTPIN 13
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
float t, h = 0;

float distanceCm;
float distanceInch;

const char *ssid = "midnight-2.4";
const char *password = "midnightdev2022";

JsonArray currentClass;
bool isServoMoving = false;

unsigned long lastRequestTime = 0;
const unsigned long timeoutDuration = 5000;

Servo myServo;
WebServer server(80);
LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

void sendSmokeAlert()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;
        http.begin(postUrl);
        http.addHeader("Content-Type", "application/json"); // Set content type to JSON

        // Create JSON payload
         String jsonPayload = "{\"message\": \"# SMOKE DETECTED! 💨💨\", \"imageUrl\": \"\"}";
        int httpResponseCode = http.POST(jsonPayload);

        if (httpResponseCode > 0)
        {
            Serial.println("POST request sent. Response:");
            Serial.println(http.getString());
        }
        else
        {
            Serial.print("Error in POST request: ");
            Serial.println(httpResponseCode);
        }

        http.end();
    }
    else
    {
        Serial.println("WiFi not connected. Unable to send POST request.");
    }
}

void setup()
{
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // disable brownout detector
    dht.begin();
    Serial.begin(115200);
    myServo.setPeriodHertz(50);
    myServo.attach(SERVO_PIN, 500, 2200);
    myServo.write(90);

    pinMode(DOOR_SENSOR_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    pinMode(RELAY_PIN, OUTPUT);
    pinMode(TRIGGER_PIN, OUTPUT); // Sets the TRIGGER_PIN as an Output
    pinMode(ECHO_PIN, INPUT);     // Sets the ECHO_PIN as an Input

    lcd.init(I2C_SDA, I2C_SCL);
    lcd.backlight();
    // lcd.setCursor(3, 0);
    // lcd.print("Hello, world!");
    // lcd.setCursor(2, 1);
    // lcd.print("My name is TOKKHU");

    connectToWiFi(ssid, password);
    setupServer(server, currentClass, isServoMoving, lastRequestTime, LED_PIN);
}

void loop()
{
    t = dht.readTemperature();
    h = dht.readHumidity();
    lcd.setCursor(0, 0);
    lcd.print("Temp : ");
    lcd.print(t);
    lcd.setCursor(0, 1);
    lcd.print("Humi : ");
    lcd.print(h);

    // Serial.println("Smoke sensor value:");
    Serial.println(analogRead(SMOKE_PIN));
    if (analogRead(SMOKE_PIN) > 1300)
    {
        Serial.println("Smoke detected!");
        digitalWrite(BUZZER_PIN, HIGH);
        sendSmokeAlert(); // Send POST request
    }
    else
    {
        digitalWrite(BUZZER_PIN, LOW);
    }

    server.handleClient();
    bool isLightOn = measureDistance(TRIGGER_PIN, ECHO_PIN, distanceCm, distanceInch, RELAY_PIN, lcd);
    handleDoorSensor(DOOR_SENSOR_PIN, BUZZER_PIN, debounceDuration, isLightOn);

    unsigned long currentTime = millis();
    handleTimeout(currentTime, lastRequestTime, isServoMoving, currentClass, timeoutDuration);

    if (isServoMoving)
    {
        Serial.println("Moving servo...");
        moveServo(myServo, angleA, angleB, 10, 15, lcd, BUZZER_PIN);
    }
    else
    {
        myServo.write(90);
    }
}
