#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <LittleFS.h>
#include "webinterface.h"
#include "../../include/secrets.h"
#include "datenerfassung.h"

void logToWebinterface(String log){
    delay(10);
}

AsyncWebServer server(80);

void startFilesystem() {
  if(!LittleFS.begin(true)) {           // true = einmalig formatieren, wenn nötig
    Serial.println("LittleFS mount fehlgeschlagen");
    return;
  }
  Serial.println("LittleFS gemountet");
}

void startWebinterface() {
  Serial.println("startWebinterface()");

  // WLAN sauber in AP-Modus
  WiFi.persistent(false);
  WiFi.mode(WIFI_OFF);
  delay(100);
  WiFi.mode(WIFI_AP);
  WiFi.setTxPower(WIFI_POWER_19_5dBm); // optional: maximale Sendeleistung

  // Passwort-Check & AP starten
  const bool usePw = (password && strlen(password) >= 8);
  if (!usePw) Serial.println("WARNUNG: Passwort zu kurz (< 8 Zeichen). Starte offenen AP.");

  bool ok = usePw
    ? WiFi.softAP(ssid, password, /*channel*/1, /*hidden*/false, /*max_conn*/4)
    : WiFi.softAP(ssid, nullptr, 1, false, 4);

  Serial.printf("softAP: %s\n", ok ? "OK" : "FAIL");
  Serial.print("AP IP: "); Serial.println(WiFi.softAPIP());

  // Dateisystem mounten (AP läuft unabhängig davon)
  bool fsOK = LittleFS.begin(true);
  if (!fsOK) Serial.println("LittleFS Mount fehlgeschlagen – zeige Fallback-Seite.");

  // Routen
  server.on("/", HTTP_GET, [fsOK](AsyncWebServerRequest *req){
    if (fsOK && LittleFS.exists("/index.html"))
      req->send(LittleFS, "/index.html", "text/html");
    else
      req->send(200, "text/plain", "Hello from EcoDrop3 AP");
  });

  server.on("/style.css", HTTP_GET, [fsOK](AsyncWebServerRequest *req){
    if (fsOK && LittleFS.exists("/styles.css"))
      req->send(LittleFS, "/styles.css", "text/css");
    else
      req->send(404, "text/plain", "style.css not found");
  });

  server.on("/script.js", HTTP_GET, [fsOK](AsyncWebServerRequest *req){
    if (fsOK && LittleFS.exists("/script.js"))
      req->send(LittleFS, "/script.js", "text/javascript");
    else
      req->send(404, "text/plain", "script.js not found");
  });

  server.begin();
  Serial.println("HTTP-Server gestartet");
}


void refreshWebinterface(){
  //TODO
}