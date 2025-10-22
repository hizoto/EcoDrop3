#pragma once
void logToWebinterface(String log);
void startWebinterface();
void startFilesystem();
void sendDataToWebinterface(String name, int value);
void sendDataToWebinterface(String name, float value);
void onWsMessage(AsyncWebSocket *server, AsyncWebSocketClient *client, 
                 AwsEventType type, void *arg, uint8_t *data, size_t len);