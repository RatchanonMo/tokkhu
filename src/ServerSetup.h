#ifndef SERVER_SETUP_H
#define SERVER_SETUP_H

#include <WebServer.h>
#include <ArduinoJson.h>

// Setup server routes
void setupServer(WebServer &server, JsonArray &currentClass, bool &isServoMoving, unsigned long &lastRequestTime, int ledPin);

#endif
