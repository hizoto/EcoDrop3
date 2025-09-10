#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include "webinterface.h"
#include "secrets.h"

void logToWebinterface(String log){
    delay(10);
}

AsyncWebServer server(80);

void startWebinterface() {
  if(!LittleFS.begin(true)){
    Serial.println("LittleFS Mount fehlgeschlagen");
    return;
  }

  
  // Access Point starten
  WiFi.softAP(ssid, password);
  Serial.println("Access Point gestartet");
  Serial.print("IP-Adresse: ");
  Serial.println(WiFi.softAPIP());  // meist 192.168.4.1

  // Webseitenrouten
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html", "text/html");
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/style.css", "text/css");
  });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/script.js", "application/javascript");
  });

  server.begin();
}
