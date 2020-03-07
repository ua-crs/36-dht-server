/*
 *  wifi_ruts.cpp
 *      Routines for WiFi management
 */

#include <Arduino.h>

#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

#include "wifi_ruts.h"

void
connect_wifi( const char *ssid, const char *pass )
{
    WiFi.begin(MY_SSID, MY_PASS);

    while( WiFi.status() != WL_CONNECTED )
    {
        Serial.print(".");
        delay(500);
    }

    Serial.println();
    Serial.println("______________");
    Serial.print( "Baud Rate = " );
    Serial.println( SERIAL_BAUD );
    Serial.print( "Conectado a ");
    Serial.println( MY_SSID );
    Serial.print( "MAC = ");
    Serial.println( WiFi.macAddress() );
    Serial.print( "local IP = " );
    Serial.println( WiFi.localIP() );
    Serial.println("______________");
}
