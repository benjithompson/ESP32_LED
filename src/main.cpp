/*
  This a simple example of the aREST Library for the ESP32 WiFi chip.
  See the README file for more details.

  Written in 2017 by Marco Schwartz under a GPL license.
  
*/

// Import required libraries
#include <WiFi.h>
#include <aREST.h>

// Create aREST instance
aREST rest = aREST();

// WiFi parameters
char *ssid = "pluviophile2.4";
char *password = "Youshallnotpassword!";

// Create an instance of the server
WiFiServer server(80);

const int led1 = 16; // the number of the LED pin
const int led2 = 17; // the number of the LED pin

// Client variables
char linebuf[80];
int charcount = 0;

// Variables to be exposed to the API
int temperature;
int humidity;

// Declare functions to be exposed to the API
int ledControl(String command);
void wifiConnectedStatusLED();

void setup()
{

  // Start Serial
  Serial.begin(115200);

  // set pin as output
  pinMode(LED_BUILTIN, OUTPUT);

  // Init variables and expose them to REST API
  temperature = 24;
  humidity = 40;
  rest.variable("temperature", &temperature);
  rest.variable("humidity", &humidity);

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

  // // Handle REST calls
  WiFiClient client = server.available();
  // if (!client) {
  //   return;
  // }
  // while(!client.available()){
  //   delay(1);
  // }
  // rest.handle(client);

  if (client)
  {
    Serial.println("New client");
    memset(linebuf, 0, sizeof(linebuf));
    charcount = 0;
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        Serial.write(c);
        //read char by char HTTP request
        linebuf[charcount] = c;
        if (charcount < sizeof(linebuf) - 1)
          charcount++;
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank)
        {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close"); // the connection will be closed after completion of the response
          client.println();
          client.println("<!DOCTYPE HTML><html><head>");
          client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head>");
          client.println("<h1>ESP32 - Web Server</h1>");
          client.println("<p>LED #1 <a href=\"on1\"><button>ON</button></a>&nbsp;<a href=\"off1\"><button>OFF</button></a></p>");
          client.println("<p>LED #2 <a href=\"on2\"><button>ON</button></a>&nbsp;<a href=\"off2\"><button>OFF</button></a></p>");
          client.println("</html>");
          break;
        }
        if (c == '\n')
        {
          // you're starting a new line
          currentLineIsBlank = true;
          if (strstr(linebuf, "GET /on1") > 0)
          {
            Serial.println("LED 1 ON");
            digitalWrite(led1, HIGH);
          }
          else if (strstr(linebuf, "GET /off1") > 0)
          {
            Serial.println("LED 1 OFF");
            digitalWrite(led1, LOW);
          }
          else if (strstr(linebuf, "GET /on2") > 0)
          {
            Serial.println("LED 2 ON");
            digitalWrite(led2, HIGH);
          }
          else if (strstr(linebuf, "GET /off2") > 0)
          {
            Serial.println("LED 2 OFF");
            digitalWrite(led2, LOW);
          }
          // you're starting a new line
          currentLineIsBlank = true;
          memset(linebuf, 0, sizeof(linebuf));
          charcount = 0;
        }
        else if (c != '\r')
        {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);

    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
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