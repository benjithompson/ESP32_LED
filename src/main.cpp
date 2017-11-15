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
void taskBlinkOne(void* p);
void blinkOne();
void taskBlinkTwo(void* p);
void blinkTwo();
int ledControl(String command);
//============================================

//=========Global Variables===================
int led;
int req = 0;
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

void loop() {
  
  // Handle REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while(!client.available()){
    delay(1);
  }
  rest.handle(client);
  delay(100);
}

// Custom function accessible by the API
int ledControl(String command)
{
  TaskHandle_t xHandle = NULL;

  // Get state from command
  int state = command.toInt();
  Serial.println(state);

  switch(state){
    case 1:
      Serial.println("Case1");
      xTaskCreate(&taskBlinkOne, "blinkOne", 1024, NULL, 10, NULL);
      break;
    case 2:
      Serial.println("Case2");
      xTaskCreate(&taskBlinkTwo, "blinkTwo", 1024, NULL, 10, NULL);
      break;
    default:
      Serial.println("default");
      break;
  }

  return state;
 
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

void taskBlinkOne(void* p){
  blinkOne();
  vTaskDelete(NULL);
}

void blinkOne(){

    for (int i = 0; i < 10; i++)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
      vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

void taskBlinkTwo(void* p){
  blinkTwo();
  vTaskDelete(NULL);
}

void blinkTwo(){

    for (int i = 0; i < 10; i++)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
      vTaskDelay(1000 / portTICK_RATE_MS);
    }
}


