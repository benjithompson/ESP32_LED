/*
  This a simple example of the aREST Library for the ESP32 WiFi chip.
  See the README file for more details.

  Written in 2017 by Marco Schwartz under a GPL license.
  
*/

// Import required libraries
#include <WiFi.h>
#include <aREST.h>

//=========Function Declarations==============
void connectToWifi();
void wifiConnectedStatusLED();
void blinkOne(void* p);
void blinkTwo(void* p);
int ledControl(String command);
//============================================

//=========Global Variables===================
int led;

// Client variables
char linebuf[80];
int charcount = 0;
//============================================

aREST rest = aREST();
WiFiServer server(80);

// WiFi parameters
char *ssid = "pluviophile2.4";
char *password = "Youshallnotpassword!";

void setup()
{

  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  // Init variables and expose them to REST API
  led = 0;
  rest.variable("Led", &led);

  // Function to be exposed
  rest.function("led", ledControl);

  // Give name & ID to the device (ID should be 6 characters long)
  rest.set_id("Leet LED");
  rest.set_name("esp32");

  // Connect to WiFi
  void connectToWifi();
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  wifiConnectedStatusLED();
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop()
{

  WiFiClient client = server.available();
  if(!client){
    return;
  }
  while(!client.available()){
    delay(1);
  }

  rest.handle(client);

  while(!client.available()){
    delay(1);
  }
}

void connectToWifi()
{

}

// Custom function accessible by the API
int ledControl(String command)
{

   BaseType_t xReturned;
   TaskHandle_t xHandle = NULL;
  Serial.println(command);
  // Get state from command
  int state = command.toInt();
  Serial.println(state);

  switch(state){
    case 1:
      Serial.println("Case1");
      xReturned = xTaskCreate(&blinkOne, "blinkOne", 1024, NULL, 10, NULL);
      break;
    case 2:
      Serial.println("Case2");
      xReturned = xTaskCreate(&blinkTwo, "blinkTwo", 1024, NULL, 10, NULL);
      break;
  }
  while (true)
  {
    if (xReturned == pdPASS)
    {
      vTaskDelete(xHandle);
      return state;
    }
  }
 
}

//Status functions
void wifiConnectedStatusLED()
{
  for (int i = 0; i < 4; i++)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(75);
    digitalWrite(LED_BUILTIN, LOW);
    delay(75);
  }
}

void blinkOne(void* p){

    for (int i = 0; i < 1; i++)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
      vTaskDelay(1000 / portTICK_RATE_MS);
    }
    

}

void blinkTwo(void* p){

    for (int i = 0; i < 2; i++)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
      vTaskDelay(1000 / portTICK_RATE_MS);
    }
}


