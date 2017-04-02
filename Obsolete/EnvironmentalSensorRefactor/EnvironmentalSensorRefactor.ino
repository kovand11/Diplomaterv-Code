#include <Wire.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


#include <DNSServer.h>           
#include <WiFiManager.h>         


#include <BME280.h>
#include <BME280I2C.h>
#include <BME280Spi.h>
#include <BME280SpiSw.h>

#include <veml6040.h>

#define MY_SSID "AndroidAP"
#define MY_PASSWORD "ifez0433"

//Board
#define LED_BLUE    12
#define LED_AMBER   13
#define PIN_SDA     2
#define PIN_SCL     14

//Global declarations
//
String htmlLog;
ESP8266WebServer server(80);
MDNSResponder mdns;
BME280I2C bme280;
VEML6040 RGBWSensor;


//Images encoded in Base64
//

//Temperature symbol 32x32
const char* img_temp = "data:image/png;base64,"
"iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAAA3NCSVQICAjb4U/gAAAACXBIWXMA"
"AAEKAAABCgEWpLzLAAAAGXRFWHRTb2Z0d2FyZQB3d3cuaW5rc2NhcGUub3Jnm+48GgAAARdQTFRF"
"////gP//gL+/bdvbjMzMec7OdcrKcsrKd8zMhNbWeM/PdMvRd87Oh9vViNfXd87Odc7OiNnWds3N"
"dc3NdszOd83Nh9fXgNTSidnXitnZitnYitnYds3Pds3Nds3Ods3OjNrZdszOds3OjdvZds3NdszO"
"ds3Ods7Ods3Oj9vads3Okd3bds3Okt7ck97dlN/dds3Oe8jHfNHRfcbDh722ibuzlN/dld/eluDe"
"m6uZm6uaos7Ios/IpaGLpqCJp5+IrJuBrMG2rZqAsJd7sJd8trGhua6fupd5u6ycvYtpwJF0wYhk"
"x5qFyJeB228/3m075HBB5HFR52xK521L6GxK7WRA710j8Vsf8Vsg81kc81kd9Fgb9FgcD9hGYQAA"
"ADB0Uk5TAAIEBxQVGB0eHyAsLzE6PllrcHFzdnmJjKivsLO0tre6z9LU2N7h6Ozt7/D4+vz8N0Dq"
"GwAAAOBJREFUOE/t0F03QlEUheEdopBUQomiT4qmviyipCg6ZZei2v7/73A4jNHF2rpujN7LNZ+r"
"JcR0zmg2G3UKbbY0maVtWuChnzxaEKByrVamgBYEyRgODQouwByAvdIM4M11/gc+vAxMcKDb1xJ4"
"lgODLrb43R5D9V3J3hOdu1gQxt2nUkr2W3TK7cuZwkR9J99ur1YY4EZFWclH2mWAH+1foJqlCAO2"
"Uf8Djdw+A9ZR/LD2UREOBogQHixwj0NuF6spXHfHk9cbJO0sEBsnQP4SON7kd/MVvqOzeHhnafr2"
"BVNsRi5y8k/dAAAAAElFTkSuQmCC";


//Pressure symbol 32x32
const char* img_pres = "data:image/png;base64,"
"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";

//Humidity symbol 32x32
const char* img_hum = "data:image/png;base64,"
"iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAAA3NCSVQICAjb4U/gAAAACXBIWXMA"
"AAGrAAABqwFz+PMoAAAAGXRFWHRTb2Z0d2FyZQB3d3cuaW5rc2NhcGUub3Jnm+48GgAAAkxQTFRF"
"////AAAAAAAAVQBVQEBAMzMzKysrJCRJICBAMxozKytALR48MyZAKyBAKR89MR07LiRALCM+KyI8"
"LR48KyNAMCI+LiFBLSBALB8+LiNALx8+Lh89LCBALyBBLiA+LCE+LiFALiA+LCE+LCFALCI/LiE/"
"LCFALiA/LSA+LSJALSI/LiE+LSE/LSE/LCE/LCA/LCE/LSE/LSI/LSE/LCE/LiE/LSI/LSE/LCE+"
"LSE/UVhwLSE/SU1mSk1mLSE/SEpka4CSa4GULiE+LSE/e5ipepmpfZuqfJysLSI/LiE/LSE/NS1K"
"LSE/LSI/MytHjbXDjrfDMShFREdfj7jFLCE/MShFkLnGUltybIWWbYaXkLrHLSA/cIycLSE/fZ2r"
"LSE/QD9aSk9nfaCtk77KLSE/LiE/RUhhREVfeZqoLyVDOjRQQ0NdLyRCLSE/Pj1Ygqe0PTtWPjxW"
"LSE/LSE/LiNBOjZROzdSibK9ODNOibK9ibO+LSE/LSI/LiJBirO+OTNPi7XANS5LNjBNodPdotXd"
"LSE/jrvEjrvFNC1JLSE/LSFAkb7Jo9bhMCRDMytIkLzHLSE/kL7HkL7Ikb/JptvjnM3WLSE/kb/J"
"LSE/mcnSLyRCodXek8LLLSE/LiNBlMPMlcTNLSE/LiJBk8LLk8PMlMTNLSE/k8LLLSE/LSJAksHK"
"lMTNlcXOmMnRmMnSmsvUmszVm83Vm83WnM7Xn9Lan9Lbotbeotffo9fgpNnhpdripdvjptzkp93l"
"p97mqN7mqN/nqd/nqeDoquHpl2iDhQAAAKh0Uk5TAAECAwQFBgcICgwRFBgZGhwdHiIkJScoKSwx"
"MkBHX2Nkb3N0eXqEhoeIiYuNkpWWm6qusbKztre4u8DBwcHCwsPDxMbJysrLzc/Q0tPV1tfY2dnZ"
"2tra29vb29zc3d3e3t7e3t/g4OHh4uLi4+Xl5ebm5+jo6Ojq6+vs7e3t7e7u7+/v8PLy8vP09PT0"
"9fX19vb29vb3+Pj5+fr6+/z8/Pz9/f39/f7+U5RwYAAAAdlJREFUOMuVkVdTU1EUhXeCGCsKAtZg"
"xYgoYouiYsGGGrBhw67YxRJ7r6hYsYVIU7Gw2LF37LD+mA/3OkDuYUbX057zfTPnnLVF/iEdxgR6"
"OE8T5o/raE3tJwBbEqJ597VAlkdEJHYsTi7A6m6teVwB8g9ifKyIZOLU0utLEPC05J4A8i9uPoNM"
"ES+uLiQfbIOvpeDD4gvktSvwih/ZJD8eR6m7mbtLqwtJvtkEvwRRSPKDHkVys5CMZST5+ll9UIJY"
"TvKVSfgVeVgfFD+mNX19p8eirni8qLHhpa6DX7y4cUS1fE/0I3feVT17E16RDODcYSC39TdzgUPn"
"gQwRkZRioCTd5r4cq3VPeglQnPK3tkSXPY0EiuKt0ZUY51zQCIQ2oKBrm2tNQ2ju/Y3I69IGH4rw"
"HDY92op5nY18IMKzSX5+ugs5MQaeVFY9iyQZebIfg53cNRWTSJJ8q7dqy5IcQh/svmcJn1RXIs0h"
"9McUi/O36mVMdAijMN0WGlXrcMAgzLCFBrMwANkW//nCfEVf7Kj89v3Hl/cRVV2F4Q7BPRPb1c7t"
"2lBPZxG9QhV7LX5nH4YYq36OFaer6i6tqcHkdsZl9CsCACCcGiPmdBo0ev2JrGG95X/yB/1K4IBN"
"a70aAAAAAElFTkSuQmCC";


//RGB symbol 32x32
const char* img_rgb = "data:image/png;base64,"
"iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAAA3NCSVQICAjb4U/gAAAACXBIWXMA"
"AAhbAAAIWwHon3XQAAAAGXRFWHRTb2Z0d2FyZQB3d3cuaW5rc2NhcGUub3Jnm+48GgAAAQhQTFRF"
"////gICA//+A379A5sxN6tVA7chJj49g789Q8dVH89FG6spK7NBM8MxJ7sxK785M79BJ8M5I7c1K"
"7s5J7s9L789J8M1Km5Nn8M1J7s5L789K7s5K7s9L7s1J785L781K785K785J8M9K8M1J789K781K"
"8M1L7s9K781K8M5L7s5K7s9J8M5K785K789J781K785K785K789K785J785K785K785K785J785K"
"bXF3785KaW95VWCA6stM785K785L789O789P8NBS8NFV8NJY8NJa8dRh8dVj8dVk8tZo8tdp8thv"
"89ly89p09N2A9N6C9N6E9d+I9uOV9uSZ9uSb9+We9+Wf9+ahadqkTwAAADx0Uk5TAAICCAoMDhAQ"
"EhYYGyMtLzFDSElLUFZXV1hgaGpsbXFyc3V2f4CFiZCWmJmlra6vsNnd5ejp6uz09vj5yTwzoQAA"
"ASJJREFUGBl9wYV6gmAYBtDXBFvE2OwOZmMi+1y47vT+72SAeyY/DM+BRT6PwyQJh0kSDovH8Z9c"
"AjaJHPYKiggdL9RqAg+dqBRgUh4DSI3IMEoBGJfB8CPQJc35mjTdAPywCMpEm5f37R3p5CCserR+"
"/Hq939BODxbHRE8ft7R3BNaErr6vyWQCRoTo7ZkYEZiliT5viJHGr2yjHkKLLrcXxGghVG9kAXd7"
"OIihQ2cPxOogNhi23dgpkk0RZgLZCDDzzcli7gOjShZVsLwyMWQvLKIzMplFYROd0p9pFBZiH+Cb"
"CzIsmjzQF2GSVErQhMkQhqakJLF3UoHOc2rwQFc5gQ2nGjg44VQDByecauDghFMNHJy4MitVXWVc"
"cLZU1SUYP+vjWEc6WvG4AAAAAElFTkSuQmCC";







//
//HTML helpers
//

/**
 * @brief Creates and icon(Base64) and a text line partial HTML source.
 */
String createIconTextLine(String icon, String text)
{
  String src;
  src += "<div id=\"temp_container\" style=\"white-space:nowrap\">";  
  src += " <div id=\"image\" style=\"display:inline;\">";
  src += "<img alt=\"Embedded Image\" src=\"";
  src += icon;
  src += "\" />";
  src += " </div>";  
  src += " <div id=\"texts\" style=\"display:inline; white-space:nowrap; font-size:200%;\">";
  src += text;
  src += " </div>";  
  src += "</div>";
  return src;
}

///
///Page creators
///

String createRootPage(float temperature,float pressure, float humidity, float ambientLight,uint16_t red,uint16_t green, uint16_t blue,uint16_t white)
{
  String htmlSource = "";
  htmlSource += "<!DOCTYPE html><html> <head> <meta http-equiv=\"refresh\" content=\"5\" /> </head> <body>";  
  htmlSource += createIconTextLine(img_temp, String(temperature) + " &#8451;");
  htmlSource += createIconTextLine(img_pres, String(pressure) + " Bar");
  htmlSource += createIconTextLine(img_hum, String(humidity) + " %");
  String rgbwText = "Amb: " + String(ambientLight) + " lux  (R: " + String(red) + "  G: " + String(green) + "  B: " + String(blue) + "  W: " + String(white) + ")";
  htmlSource += createIconTextLine(img_rgb, rgbwText);
  htmlSource += "<br />";
  htmlSource += "<a href=\"/settings\">Settings</a> ";
  htmlSource += "<a href=\"/developer\">Developer</a> ";
  htmlSource += "<a href=\"/debug\">Log</a> ";
  htmlSource +="</body></html>";  
  return htmlSource;
}

String createSettingsPage(String network,String password,String id,String mac,bool bme280Detected, bool veml6040Detected)
{
  //Status page should be here as well
  String htmlSource = "";
  return htmlSource;
}

String createDeveloperPage()
{
  String htmlSource = "";
  return htmlSource;
}

String createJsonPage(float temperature,float pressure, float humidity,float ambientLight,uint16_t red,uint16_t green, uint16_t blue,uint16_t white)
{
  String htmlSource = "";
  return htmlSource;
}

String createLogPage() //Uses a global log structure
{
  String htmlSource = "";
  return htmlSource;
}


void setupPeripherals()
{
  //Serial port
  Serial.begin(115200);
  
  //LEDs
  pinMode(LED_BLUE, OUTPUT);
  digitalWrite(LED_BLUE, LOW);
  pinMode(LED_AMBER, OUTPUT);
  digitalWrite(LED_AMBER, LOW);

  //BME280 (just once)
  bme280.begin();

  //VEML6040
  
  
}


void setupWebserver()
{
  WiFiManager wifiManager;
  wifiManager.autoConnect("AutoConnectAP");
    
  delay(1000);
  Serial.print("Waiting for connection");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(MY_SSID);
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());

  
  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }

  server.on("/", []() {
    float temp(NAN), hum(NAN), pres(NAN);
    bool useCelsius = true;
    uint8_t pressureUnit = B001; //hPa
    bme280.read(pres, temp, hum, useCelsius, pressureUnit);
    float amb = RGBWSensor.getAmbientLight();
    uint16_t r = RGBWSensor.getRed();
    uint16_t g = RGBWSensor.getGreen();
    uint16_t b = RGBWSensor.getBlue();
    uint16_t w = RGBWSensor.getWhite();    
    server.send(200, "text/html", createRootPage(temp, hum, pres,amb,r,g,b,w));
  });




  /*server.on("/status", []() {    
    char * keys[] = {"Chip ID","Heap size","Compilation date","Compilation time"};
    String values[4];
    values[0] = String(ESP.getChipId());
    values[1] = String(ESP.getFreeHeap());
    values[2] = String(__DATE__ );
    values[3] = String(__TIME__);
    
    server.send(200, "text/html", createStatusPage(keys, values, 4));
  });*/

  server.on("/settings", []() {
    server.send(200, "text/html", "Settings will be implemented later. Content: ssid, pass,page autorefresh");
  });

  server.on("/developer", []() {
    server.send(200, "text/html", "Developer options will be implemented later. First leds");
  });


  server.on("/json", []() {
    float amb = RGBWSensor.getAmbientLight();
    uint16_t r = RGBWSensor.getRed();
    uint16_t g = RGBWSensor.getGreen();
    uint16_t b = RGBWSensor.getBlue();
    uint16_t w = RGBWSensor.getWhite();
    server.send(200, "text/html", createJsonPage(1.1f, 2.2f, 3.3f, amb, r, g, b, w));
  });
  
  //Temporary page to check stuff
  server.on("/log", []() {
    server.send(200, "text/html", String("UNIMPLEMENTED"));
  });
  
  server.begin();
  
}


/**
 * @brief The initializer callback. Runs once at device startup.
 */
void setup()
{
  setupPeripherals();
  setupWebserver();  
}

/**
 * @brief The main loop. 
 */
void loop() {
}


