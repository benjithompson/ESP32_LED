#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Arduino.h>

// WiFi parameters
#define SSID "pluviophile2.4"
#define PW "Youshallnotpassword!"

void setupOta();
void connectWifi();

