/*
  This a simple example of the aREST Library for the ESP32 WiFi chip.
  See the README file for more details.

  Written in 2017 by Marco Schwartz under a GPL license.
  
*/

// Import required libraries
#include <WiFi.h>
#include <aREST.h>
#include <aREST_UI.h>



// Create aREST instance
aREST_UI rest = aREST_UI();


// WiFi parameters
char *ssid = "pluviophile2.4";
char *password = "Youshallnotpassword!";

// Create an instance of the server
WiFiServer server(80);

// Client variables
char linebuf[80];
int charcount = 0;

// Variables to be exposed to the API
int led;

// Declare functions to be exposed to the API
int ledControl(String command);
void wifiConnectedStatusLED();

void setup()
{

  // Start Serial
  Serial.begin(115200);

  // set pin as output
  pinMode(LED_BUILTIN, OUTPUT);
  // pinMode(32, OUTPUT);
  // pinMode(35, OUTPUT);
  // pinMode(32, OUTPUT);

  rest.title("aREST UI Demo");
  
  rest.slider(34);

  rest.button(2);

  // Init variables and expose them to REST API
  led = 0;
  rest.variable("Led", &led);

  // Labels
  rest.label("temperature");
  rest.label("humidity");


  // Function to be exposed
  rest.function("led", ledControl);

  // Give name & ID to the device (ID should be 6 characters long)
  rest.set_id("000001");
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
}

// Custom function accessible by the API
int ledControl(String command)
{
  // Get state from command
  int state = command.toInt();
  digitalWrite(6, state);
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


