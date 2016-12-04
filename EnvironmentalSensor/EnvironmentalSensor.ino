/**
 * @file EnvironmentalSensor.ino
 * @version 1.0
 * @author András Kovács
 * @title Environmental Sensor Firmware
 */

#include <Wire.h>

#include <BME280.h>



#include <ESP8266WebServer.h>
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
#include <ESP8266mDNS.h>

//General comments
//

//JSON Interface Example
/*
{
  "Type": "Environmental Sensor",
  "MAC": "mac ide",
  "Air": {
    "temp": "22.5",
    "pres": "1.01",
    "hum": "45"
  },
  "RGB": {
    "R": "200",
    "G": "186",
    "B": "211",
    "W": "652"
  },
  "UV": {
    "A": "0",
    "B": "0"
  }
}
 */

//Images encoded in Base64
//

//Temperature symbol 64x64
const char* img_temp = "data:image/png;base64,"
"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";

//Pressure symbol 64x64
const char* img_pres = "data:image/png;base64,"
"iVBORw0KGgoAAAANSUhEUgAAAEAAAABACAMAAACdt4HsAAAAA3NCSVQICAjb4U/gAAAACXBIWXMA"
"AAG7AAABuwE67OPiAAAAGXRFWHRTb2Z0d2FyZQB3d3cuaW5rc2NhcGUub3Jnm+48GgAAAlJQTFRF"
"////AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAtrfCOAAAAMV0Uk5TAAECAwQFBgcICQoLDA0ODxASExUW"
"FxgZGhscHR4fICMkJSYnKissLS4vMTQ1Nzk6Ozw9Pj9AQUJDREVGR0hKS0xOT1BRU1RWV1haW1xf"
"YGNmaWpsbm9wcXJzdXZ3eXp7fX5/gIOEhoeIjI2Oj5CRlJaXmJmanZ6foKGkpaeqq6yur7KztLW2"
"t7i5ury+v8DBw8TFxsfJysvMzs/Q0tPU1dbX2Nvd3t/g4eLj5OXm5+jp6uvs7e7v8PHy9PX29/j5"
"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";

//Humidity symbol 64x64
const char* img_hum = "data:image/png;base64,"
"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";


//RGB symbol 64x64
const char* img_rgb = "data:image/png;base64,"
"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"
"3usAXnb9C5treY/3Dhn/AAAAAElFTkSuQmCC";


//UV symbol 64x64
const char* img_uv = "data:image/png;base64,"
"iVBORw0KGgoAAAANSUhEUgAAAEAAAABACAMAAACdt4HsAAAAA3NCSVQICAjb4U/gAAAACXBIWXMA"
"AAK5AAACuQF2vISvAAAAGXRFWHRTb2Z0d2FyZQB3d3cuaW5rc2NhcGUub3Jnm+48GgAAAnZQTFRF"
"////AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAAAEAAAE"
"AAAEAAAEAAADAAADAAADAAADAAADAAADAAADAAADAAADAAADAAADAAADAAADAAADAAADAAADAAAD"
"AAADAAADAAADAAADAAADAAADAAACAAACAAACAAACAAACAAACAAACAAACAAACAAACAAACAAACAAAC"
"AAACAAACAAACAAACAAACAAACAAACAgACAgACAgACAgACAgACAgACAgACAgACAgACAgACAgACAgAC"
"AgACAgACAgACAgACAgACAgACAgACAgACAgACAgACAgACAgACAgACAgACAgACAgACAgACAgACAgAC"
"AgACAgACAgACAgACAgACAgACAQABAQABAQABAQABAQABAQABAQABAQABAQABAQABAQABAQABAQAB"
"AQABAQABAQADAQADAQADAQADAQADAQADAQADAQADAQADAQADAQADAQADAQACAQACAQACAQACAQAC"
"AQACAQACAQACAQACAQACAQACAQACAQACAQACAQACAQACAQACAQACAQACAQACAQACAQACAQACAQAC"
"AQACAQACAQACAQACAQACAQACAQACAQACAQACAQACAQACAQACAQACAQACAQACAQACAQACAQACAQAC"
"AQAC9wrllgAAANF0Uk5TAAECAwQFBgcICQoMDQ4PEBESExQVFhcZGhscHR8gISIjJSYnKSorLC0u"
"LzEyNjc4OTo7PD0+QkNERUdMTU5PUVNUVVZXWFlaW1xdX2FiY2RlZmhpamtscHFyc3R1d3h5ent8"
"fX5/gIGDhIaHiImKi42Oj5CRkpOUlZaXmJmam5ydnqCio6Smp6ipqqusra6wsbS1tre4ubu9vsHC"
"w8TFxsfIycrLzM7P0NLT1NbX2Nnb3N3e4OHi4+Tl5ufo6evs7e7w8fLz9PX29/j5+vv8/f4fywFA"
"AAAEP0lEQVQYGc3B+V8UBQDG4Xc5JFzMIzQBwyyx8ADvTCjtgDQoI4/SzEiBNNNKLQ8MryyiBCsU"
"o0TtkNIyLUQjCkUBXZHd73/U7OzMssDsLh9+6nkklWNLVxFRFSkdW7kMGT3YkrWaqFYrGVtPhqQM"
"gtK1nKiWK52gDBk2YZurxUS1WHOxbZIpp5mAfM0iqlnKJ6A5RxZ36RX81iqhkyg6E7QWvyulbvWK"
"zak5viU/RaomimopJX/L8ZqcWDkqJIpCRTbWS0TesYriABEdUDTJ14ngerKiWkEEKxRdTANhNcQo"
"msIkJZ0gjG+SlFSoSEYfpS5ewz7HUXWC4us4OlphTW4GDrsUsxsH+2LlOgw0T1Y4FfjtiJXW3KCf"
"zmIpdgd+FQojsR1Tfao0enMnIbq2Jkup9ZjaE+UsD0tbrqQxSw61Ymo9tGSMpNw2LHlyVoWtQSbX"
"hBk5OTMmuGQ6ha1Kzi5i6ZkqB9O9WC7KkduHZZtMs8uLl86bt7S4fLZMu7D43HKSRoBnt1umMixl"
"Mt279w4BaXISdxVD17bxstRgqZElZXsXhqtxcjSz9tTOXLeCmrA0Kcidu/NU7UwNTgOWBg1NAZYC"
"DU1iO6b2RA3RdkzbNVSjWjG0jtJgpZ38o1ih8jDkqY/UM60vy1n8BfBlKdRGr3ej+voKeFqOXsWQ"
"JVNWVdVYGcaNk9/CIx8MU0At0JQoB8ntwF/D5XdfC5xU0CQPHFTASgwb5OAjDM/LtAxoUdB8oCdF"
"prjzQFeqBpjmBU67ZKoG6hX0gA9Yr4AFGD7RAN8CvhkyDbsJrFOvOuBPlwJqMMxWP89gOKiAxzBk"
"qtezGBYo4EEP8FOM+ki8DHTcr4D3gKsKkdAGVMqyGcOL6qMUwzpZLgAVCrUN8IxRgLsFaB2hEGm3"
"gEsJCpiIYZFCPYLhFVkKMLyrEJ9ieFKWVUB3kvo4A5yTxfU90D1JQVkY6mSrBU6or2UYsmTJ9gHV"
"CloD3J0iy/DbwGvqy90BVMh2CGhTUAGwQ7YcDBnqZx/Q4ZZl/E34zaWgTb9+GC/bLqBJ/WVjWCrb"
"nNP1UxTGPmCPBjgPrNRg5HvpekgDPOHjWqYGZUHJo3IwZ91k/e9N/IFjIxT09u2Wp6Q3uzs/HqGg"
"e/a2e7cqjC1AoWwpXvhdOgh8FyOL62vgM4VRCZTJlg14pMxuYJUsK4FbGQqjEiiTLRvwSNoAdKTJ"
"9PAtoEjhVAJlsmUDHklxZ4Ev5Rf3I/CFwqoEymTLBjwyZHYDeTK8BVweqbAqgTLZsgGP/EqA1pHS"
"tLvQM0fh7Qe2yjYf+Fd+8Y3ATsWdBUoUwTLgF9neAY7INLUbvNNfB+pjFcFEH7AhRqaF7cBLCigF"
"zt2Ga2mK6AUfcKmqZMkbhxu9wB6XAuIbMS1SFM9dotff6xWUeQPD+4pu7q5jP/9zp+Vs7Z7HYxVi"
"WmXjiSIN8B/ZQGqWVW1cwwAAAABJRU5ErkJggg==";


//Constants
//

//Network of my phone
#define MY_SSID "AndroidAP"
#define MY_PASSWORD "ifez0433"

//Board
#define LED_BLUE    12
#define LED_AMBER   13
#define PIN_SDA     2
#define PIN_SCL     14

//BME280
#define BME280_ADDRESS 0x76

//VEML6040
#define  VEML6040_ADDRESS 0x10
#define  VEML6040_CONF      0x00
#define  VEML6040_R_DATA    0x08  
#define  VEML6040_G_DATA    0x09 
#define  VEML6040_B_DATA    0x0A
#define  VEML6040_W_DATA    0x0B



enum IT {
  IT_40 = 0, //   40 ms
  IT_80,     //   80 ms
  IT_160,    //  160 ms
  IT_320,    //  320 ms
  IT_640,    //  640 ms
  IT_1280   // 1280 ms
};


// Specify VEML6070 Integration time
uint8_t IT = IT_160;
uint8_t ITime = 160;  // milliseconds
uint16_t RGBWData[4] = {0, 0, 0, 0};
float GSensitivity = 0.25168/((float) (IT + 1)); // ambient light sensitivity increases with integration time
float redLight, greenLight, blueLight, ambientLight;



struct BME280Result
{
  float Temperature;
  float Pressure;
  float Humidity;
};


//Global declarations
//
ESP8266WebServer server(80);
MDNSResponder mdns;
BME280 bme;


/**
 * @brief: Gets the BME280 measurements on demand
 */
BME280Result getBME280Data()
{
  return {0.0f,0.0f,0.0f};
}


/**
 * @brief: Creates a HTML table with key value parts
 */
String createKeyValueTable(char* keys[], String values[],int rowcount)
{
  String src = "";
  src += "<table>\n";
  for (int i=0; i < rowcount; i++)
  {
    src += "<tr>";
    src += String("<td>") + String(keys[i]) + String("</td>");
    src += String("<td>") + values[i] + String("</td>");
    src += "</tr>";    
  }
  src += "</table>\n";
  return src;
}

/**
 * @brief Creates and icon(Base64) and a text line partial HTML source.
 */
String createIconTextLine(String icon, String text)
{
  String src;
  src+="<div id=\"temp_container\" style=\"white-space:nowrap\">";  
  src+=" <div id=\"image\" style=\"display:inline;\">";
  src += "<img alt=\"Embedded Image\" src=\"";
  src += icon;
  src += "\" />";
  src += " </div>";  
  src += " <div id=\"texts\" style=\"display:inline; white-space:nowrap; font-size:200%;\">";
  src += text;
  src = " </div>";  
  src += "</div>";
  return src;
}



/**
 * @brief Creates the HTML source of the main page, in a human readable form. Displayed: Temp., Press., Hum., RGBW, UVAB
 */
String createMainPage(float temp,float pres, float hum)
{
  String src = "";
  src += "<!DOCTYPE html><html><body>";  
  createIconTextLine(img_temp, String(temp) + " &#8451;");
  createIconTextLine(img_pres, String(pres) + " Bar");
  createIconTextLine(img_hum, String(hum) + " %");  
  src +="</body></html>";  
  return src;
}

/**
 * @brief At first I am going to implement the LED controll
 */
String createControlPage()
{
  return "Control will be implemented later";
}

/**
 * @brief Displays the measured data in JSON format.  Displayed: Temp., Press., Hum., RGBW, UVAB
 */
String createJSONPage(float temp,float pres, float hum,float r,float g,float b, float w,float uva,float uvb)
{
  String src = "";
  src += "{\n";
  src += "  \"Type\": \"Environmental Sensor\",\n";
  src += "  \"MAC\": \"" + String(WiFi.macAddress()) + "\",\n";
  src += "  \"Air\": {";
  src += "    \"temp\": \"" + String(temp) + "\",\n";
  src += "    \"pres\": \"" + String(pres) + "\",\n";
  src += "    \"hum\": \"" + String(hum) + "\",\n";
  src += "  },\n";
  src += "  \"RGB\": {";
  src += "    \"R\": \"" + String(r) + "\",\n";
  src += "    \"G\": \"" + String(g) + "\",\n";
  src += "    \"B\": \"" + String(b) + "\",\n";
  src += "    \"W\": \"" + String(w) + "\",\n";
  src += "  },";
  src += "  \"UV\": {";
  src += "    \"A\": \"" + String(uva) + "\",\n";
  src += "    \"B\": \"" + String(uvb) + "\",\n";
  src += "  }\n";
  src += "}\n";

  return src;
}

/**
 * @brief Scans all the I2C addresses, and (now) displays the result on the serial port.
 */
void I2Cscan() 
{
    // scan for i2c devices
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    Serial.print(address,HEX);
    Serial.print(" ");
    Serial.print(error);
    Serial.print("\n");
  }
}



/**
 * @brief: Server initialization
 */
void serverSetup()
{
  WiFi.begin(MY_SSID, MY_PASSWORD);
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
    getRGBWdata(RGBWData);
    server.send(200, "text/html", createMainPage((float)RGBWData[0]/96.0f,(float)RGBWData[1]/74.0f,(float)RGBWData[2]/56.0f));
  });

  server.on("/control", []() {
    server.send(200, "text/html", "Control will be implemented later");
  });

  server.on("/json", []() {
    server.send(200, "text/html", createJSONPage(1.1f, 2.2f, 3.3f, RGBWData[0],RGBWData[1],RGBWData[2],RGBWData[3] , 8.8f, 9.9f));
  });
  server.begin();
}

//TMP
void enableVEML6040()
{
  Wire.beginTransmission(VEML6040_ADDRESS);
  Wire.write(VEML6040_CONF); // Command code for configuration register
  Wire.write(IT << 4); // Bit 3 must be 0, bit 0 is 0 for run and 1 for shutdown, LS Byte
  Wire.write(0x00); // MS Byte
  Wire.endTransmission();
}

//TMP
uint16_t getRGBWdata(uint16_t * destination)
{
    for (int j = 0; j < 4; j++)
    {
    uint8_t rawData[2] = {0, 0};
    Wire.beginTransmission(VEML6040_ADDRESS);
    Wire.write(VEML6040_R_DATA + j);        // Command code for reading rgbw data channels in sequence
    Wire.endTransmission(false);  // Send the Tx buffer, but send a restart to keep connection alive

    Wire.requestFrom(VEML6040_ADDRESS, 2);  // Read two bytes from slave register address 
    uint8_t i = 0;
    while (Wire.available()) 
    {
        rawData[i++] = Wire.read();       // Put read results in the Rx buffer
    }     
    Wire.endTransmission();
    destination[j] = ((uint16_t) rawData[1] << 8) | rawData[0];
    } 
}




/**
 * @brief The initializer callback. Runs once at device startup.
 */
void setup()
{
  pinMode(LED_BLUE, OUTPUT);
  digitalWrite(LED_BLUE, LOW);
  pinMode(LED_AMBER, OUTPUT);
  digitalWrite(LED_AMBER, LOW);
  Serial.begin(115200);
  serverSetup();

  Wire.begin(PIN_SDA,PIN_SCL);
  Wire.setClock(200000);
  I2Cscan();

  enableVEML6040(); // initalize sensor
  delay(150);
}

/**
 * @brief The main loop. 
 */
void loop() {
  server.handleClient();
}


