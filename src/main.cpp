/*
  This a simple example of the aREST Library for the ESP32 WiFi chip.
  See the README file for more details.

  Written in 2017 by Marco Schwartz under a GPL license.
  
*/

// Import required libraries
#include <WiFi.h>
#include <aREST.h>

// Import local files
#include "endpoints.h"
#include "ota.h"

void connectWifi();
void wifiConnectPending();

aREST rest = aREST();
WiFiServer server(80);

int led;
int req = 0;

// Client variables
char linebuf[80];
int charcount = 0;

void setup()
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);

    // Init variables and expose them to REST API
    led = 0;
    rest.variable("Led", &led);

    // Function to be exposed
    rest.function("led", ledControl);
    rest.function("test", testFunction);

    // Give name & ID to the device (ID should be 6 characters long)
    rest.set_id("Leet LED");
    rest.set_name("esp32");

    connectWifi();

    // Start the server
    server.begin();
    Serial.println("Server started");

    setupOta();

    // Print the IP address
    Serial.println(WiFi.localIP());
}

void loop()
{

    ArduinoOTA.handle();

    // Handle REST calls
    WiFiClient client = server.available();
    if (client)
    {
        while (!client.available())
        {
            delay(5);
        }
        rest.handle(client);
        delay(1000);
    }
}
