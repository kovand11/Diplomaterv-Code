




/**
 * @file EnvironmentalSensor.ino
 * @version 1.0
 * @author András Kovács
 * @title Open Detector Firmware
 */

#include <limits>

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>


ESP8266WebServer server(80);
MDNSResponder mdns;




/**
 * @brief The initializer callback. Runs once at device startup.
 */
void setup()
{
  //try to connect
  bool isConnected;
  if (isConnected)
  {
    int result;
    result = doit();
    if (result)
      getSetup();
  }
  else
  {
    getSetup();
    doit();
  }

  ESP.deepSleep(std::numeric_limits<int>::max());
}

/**
 * @brief The main loop. 
 */
void loop()
{
}


