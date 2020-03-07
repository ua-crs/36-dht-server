/*
 *  Project 36-dht-server - maim.cpp
 *      Asynchronous server
 *      for humidity and temperature
 */

/*
 *  Includes
 */

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>

#else
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif

#include <ESPAsyncWebServer.h>

#include "wifi_ruts.h"

/*
 *  Definitions
 */

//#define DHTPIN  13                  // Digital pin connected to the DHT sensor
//#define DHTTYPE DHT11               // select: DHT22 (AM2302) or DHT11 or DHT21 (AM2301)
const long interval = 4000;        // Updates DHT readings every 10 seconds

/*
 *  Objects invocation
 */

DHT dht(DHTPIN, DHTTYPE);
AsyncWebServer server(80);          // Create AsyncWebServer object on port 80

/*
 *  Global variables
 */

float t, h;                         // current temperature & humidity, updated in loop()
unsigned long previousMillis = 0;   // will store last time DHT was updated

#include "html_page.h"              // page in HTML

/*
 *  Auxiliary functions
 */

static String
processor(const String &var)
{
    return var == "TEMPERATURE" ? String(t): String(h);
}

static int
get_temp_hum( float *pt, float *ph )
{
    *pt = dht.readTemperature();
    *ph = dht.readHumidity();
    return !isnan(*pt) && !isnan(*ph);
}

/*
 *  Public functions
 */

void
setup( void )
{
    Serial.begin(115200);
    dht.begin();

    // Connect to Wi-Fi
    connect_wifi(MY_SSID, MY_PASS);

    // Route for root / web page
    server.on("/", HTTP_GET,
            [](AsyncWebServerRequest *request) { request->send_P(200, "text/html", index_html, processor); });
    server.on("/temperature", HTTP_GET,
            [](AsyncWebServerRequest *request) { request->send_P(200, "text/plain", String(t).c_str()); });
    server.on("/humidity", HTTP_GET,
            [](AsyncWebServerRequest *request) { request->send_P(200, "text/plain", String(h).c_str()); });

    // Start server
    server.begin();
}

void
loop( void )
{
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval)
    {
        previousMillis = currentMillis;         // save the last time you updated the DHT values
        if( get_temp_hum( &t, &h ) )    
        {
            Serial.print( "Temperature = " );
            Serial.print( t, 1 );
            Serial.print( "\tHumidity = " );
            Serial.println( h, 1 );
        }
        else
            Serial.println("Failed to read from DHT sensor!");
    }
}
