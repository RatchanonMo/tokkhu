#include "ServerSetup.h"
#include <ArduinoJson.h>

void setupServer(WebServer &server, JsonArray &currentClass, bool &isServoMoving, unsigned long &lastRequestTime, int ledPin)
{
    server.on("/send", HTTP_POST, [&server, &currentClass, &isServoMoving, &lastRequestTime]()
              {
                  if (server.hasArg("plain"))
                  {
                      String message = server.arg("plain");
                    //   Serial.println("Received POST request: " + message);

                      lastRequestTime = millis();

                      StaticJsonDocument<200> jsonDoc;
                      DeserializationError error = deserializeJson(jsonDoc, message);
                      if (error)
                      {
                          server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Invalid JSON\"}");
                          return;
                      }
                      JsonArray jsonArray = jsonDoc.as<JsonArray>();
                      bool foundAlien = false;
                      for (JsonObject obj : jsonArray) {
                            String className = obj["class"];
                 

                            // Debug print to show received data
                            Serial.println("Class: " + className);

                            if (className != "Coca-Cola") {
                                foundAlien = true;
                            }
                        }
                        currentClass = jsonArray;
                        isServoMoving = foundAlien; // Stop servo if Alien is found

                      
                      server.send(200, "application/json", "{\"status\":\"success\"}");
                  }
                  else
                  {
                      server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"No data received\"}");
                  } });

    server.on("/light", HTTP_POST, [&server, &ledPin]()
              {
                  if (server.hasArg("plain"))
                  {
                      String message = server.arg("plain");

                      StaticJsonDocument<200> jsonDoc;
                      DeserializationError error = deserializeJson(jsonDoc, message);
                        if (error)
                        {
                            server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Invalid JSON\"}");
                            return;
                        }
                    if(jsonDoc["status"] == "on")
                    {
                      Serial.println("Light on");
                        digitalWrite(ledPin, HIGH);
                    }
                    else
                    {
                      Serial.println("Light off");
                        digitalWrite(ledPin, LOW);

                    }


                      server.send(200, "application/json", "{\"status\":\"success\"}");
                  }
                  else
                  {
                      server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"No data received\"}");
                  } });

    server.begin();
    Serial.println("HTTP server started.");
}
