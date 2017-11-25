
#include <Arduino.h>
#include "endpoints.h"

bool lightStatus = HIGH;

int lightsToggle(String command)
{
    // Get state from command
    int state = command.toInt();
    //Serial.println(state);
    //Serial.println("Toggled Lights");
    if(lightStatus){
        digitalWrite(LED_BUILTIN, HIGH);
    }else{
        digitalWrite(LED_BUILTIN, LOW);
    }
    lightStatus = !lightStatus;
}
// Custom function accessible by the API
int ledControl(String command)
{
    // Get state from command
    int state = command.toInt();
    //Serial.println(state);

    switch (state)
    {
    case 1:
        //Serial.println("Case1");
        xTaskCreate(&taskBlinkOne, "blinkOne", 512, NULL, 10, NULL);
        break;
    case 2:
        //Serial.println("Case2");
        xTaskCreate(&taskBlinkTwo, "blinkTwo", 512, NULL, 10, NULL);
        break;
    default:
        //Serial.println("default");
        break;
    }
    return state;
}

void taskBlinkOne(void *p)
{
    blinkOne();
    vTaskDelete(NULL);
}

void blinkOne()
{

    for (int i = 0; i < 10; i++)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
        delay(100);
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

void taskBlinkTwo(void *p)
{
    blinkTwo();
    vTaskDelete(NULL);
}

void blinkTwo()
{

    for (int i = 0; i < 10; i++)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
        delay(100);
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}