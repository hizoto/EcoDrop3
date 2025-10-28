#pragma once
#include <Arduino.h>
#include <ESPAsyncWebServer.h>

void logToWebinterface(String log);
void startWebinterface();
void onWsMessage(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
void startAP();
