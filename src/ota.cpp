
#include "ota.h"

void wifiConnectPending();



void connectWifi()
{
    // Connect to WiFi
    WiFi.begin(SSID, PW);
    wifiConnectPending();
    Serial.println("");
    Serial.println("WiFi connected");
}

void wifiConnectPending()
{
    while (WiFi.status() != WL_CONNECTED)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(500);
        digitalWrite(LED_BUILTIN, LOW);
        delay(500);
        Serial.print(".");
    }

    //Blink LED when connected
    for (int i = 0; i < 4; i++)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(75);
        digitalWrite(LED_BUILTIN, LOW);
        delay(75);
    }
}

void setupOta()
{

    // Port defaults to 3232
    ArduinoOTA.setPort(3232);

    // Hostname defaults to esp3232-[MAC]
    ArduinoOTA.setHostname("myesp32");

    // No authentication by default
    // ArduinoOTA.setPassword("admin");

    // Password can be set with it's md5 value as well
    // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
    // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

    ArduinoOTA.begin();

    Serial.printf("OTA Ready at: %s\n", ArduinoOTA.getHostname());
}
