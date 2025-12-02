#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <LittleFS.h>
#include "webinterface.h"
#include "../include/secrets.h"
#include "datenerfassung.h"
#include "communication.h"
#include <ArduinoJson.h>



AsyncWebServer server(80);
AsyncWebSocket ws("/ws");


// Standard IP: 192.168.4.1
void startWebinterface() {
  startAP();

  // Dateisystem mounten (AP läuft unabhängig davon)
  bool fsOK = LittleFS.begin(false);
  if (!fsOK) Serial.println("LittleFS Mount fehlgeschlagen – zeige Fallback-Seite.");

  // Routen
  server.on("/", HTTP_GET, [fsOK](AsyncWebServerRequest *req){
    if (fsOK && LittleFS.exists("/index.html"))
      req->send(LittleFS, "/index.html", "text/html");
    else
      req->send(200, "text/plain", "Hello from EcoDrop3 AP");
  });

  server.on("/styles.css", HTTP_GET, [fsOK](AsyncWebServerRequest *req){
    if (fsOK && LittleFS.exists("/styles.css"))
      req->send(LittleFS, "/styles.css", "text/css");
    else
      req->send(404, "text/plain", "styles.css not found");
  });

  server.on("/script.js", HTTP_GET, [fsOK](AsyncWebServerRequest *req){
    if (fsOK && LittleFS.exists("/script.js"))
      req->send(LittleFS, "/script.js", "text/javascript");
    else
      req->send(404, "text/plain", "script.js not found");
  });

  // HTTP-Endpunkt für Sensordaten
  server.on("/sensors", HTTP_GET, [](AsyncWebServerRequest *request){
    JsonDocument sensorDaten;
    sensorDaten["temp"] = temp;
    sensorDaten["voltage"] = voltage;
    sensorDaten["current"] = current;
    //sensorDaten["chargingCurrent"] = chargingCurrent;
    //sensorDaten["chargingVoltage"] = chargingVoltage;
    
    String json;
    serializeJson(sensorDaten, json);
    request->send(200, "application/json", json);
  });

  // WebSocket initialisieren
  ws.onEvent(onWsMessage);
  server.addHandler(&ws);

  server.begin();
  Serial.println("HTTP-Server gestartet");
}


void onWsMessage(AsyncWebSocket *server, AsyncWebSocketClient *client, 
                 AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_DATA) {
    String msg = "";
    for (size_t i = 0; i < len; i++) msg += (char)data[i];

    if (msg == "start") {
      sendStartSignal();
      client->text("EcoDrop3 gestartet.");
    }
    else if (msg == "stop"){
      sendStopSignal();
      client->text("EcoDrop3 gestoppt.");
    }
    else if (msg == "estop"){
      sendEmergencyStop();
      client->text("EcoDrop3 Not-Aus betätigt!");
    }
    else if (msg == "save"){
      saveSensorDataToWebinterface();
      client->text("Daten wurden manuell gespeichert.");
    }
  }
}

void logToWebinterface(String log){
  ws.textAll(log);
}

void startAP(){
  // WLAN sauber in AP-Modus
  WiFi.persistent(false);
  WiFi.mode(WIFI_OFF);
  delay(100);
  WiFi.mode(WIFI_AP);
  WiFi.setTxPower(WIFI_POWER_19_5dBm); // maximale Sendeleistung

  // Passwort-Check & AP starten
  const bool usePw = (password && strlen(password) >= 8);
  if (!usePw) Serial.println("WARNUNG: Passwort zu kurz (< 8 Zeichen). Starte offenen AP.");

  bool ok = usePw
    ? WiFi.softAP(ssid, password, /*channel*/1, /*hidden*/false, /*max_conn*/4)
    : WiFi.softAP(ssid, nullptr, 1, false, 4);

  Serial.printf("softAP: %s\n", ok ? "OK" : "FAIL");
  Serial.print("AP IP: "); Serial.println(WiFi.softAPIP());
}
