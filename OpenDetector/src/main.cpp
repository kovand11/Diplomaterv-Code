#include<Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

#include <EEPROM.h>




#define EEPROM_USER 0
#define EEPROM_PASSWORD 64
#define EEPROM_SERVERADDRESS 128

#define LED 5
#define SW 14
#define IRST 16


ESP8266WebServer server(80);

void writeToEEPROM(int address,String value)
{
  int addr = address;
  for (int i=0; i < value.length();i++)
  {
    EEPROM.write(addr, (byte)value[i]);
    addr++;
  }
  EEPROM.write(addr, 0);
}

String readFromEEPROM(int address,int maxlen)
{
  String str = "";
  int addr = address;
  while (true)
  {
    if (addr - address >=maxlen)
      break;
    byte ch = EEPROM.read(addr);
    addr++;
    if (ch == 0)
      break;
    else
      str += (char)ch;
  }
  return str;

}

void performSetup()
{
  Serial.println("Setup started");
  WiFi.mode(WIFI_AP);
  WiFi.begin("OpenDetector-Setup");
  server.on("/", [](){

    EEPROM.begin(192);
    String user = readFromEEPROM(EEPROM_USER,64);
    String password = readFromEEPROM(EEPROM_PASSWORD,64);
    String serverAddress = readFromEEPROM(EEPROM_SERVERADDRESS,64);
    EEPROM.end();

    String form = "";
    form += "<!DOCTYPE html>\n";
    form += "<html>\n";
    form += "<body>\n";
    form += "<form action=\"/process\">\n";
    form += "SSID:<br>\n";
    form += "<input type=\"text\" name=\"user\" value=\"" + user + "\">\n";
    form += "<br>\n";
    form += "Password:<br>\n";
    form += "<input type=\"text\" name=\"password\" value=\"" + password + "\">\n";
    form += "<br><br>\n";
    form += "Server address:<br>\n";
    form += "<input type=\"text\" name=\"serveraddress\" value=\"" + serverAddress + "\">\n";
    form += "<br><br>\n";
    form += "<input type=\"submit\" value=\"Submit\">\n";
    form += "</form>\n";
    form += "</body>\n";
    form += "</html>\n";

    server.send(200, "text/html", form);
  });

  server.on("/process", [](){
    String user = server.arg("user");
    String password = server.arg("password");
    String serverAddress = server.arg("serveraddress");
    Serial.println("Form " +user+ " " +password+ " "+serverAddress);
    EEPROM.begin(192);
    writeToEEPROM(EEPROM_USER, user);
    writeToEEPROM(EEPROM_PASSWORD, password);
    writeToEEPROM(EEPROM_SERVERADDRESS, serverAddress);
    EEPROM.end();
    server.send(200, "text/html", "Rebooting...");
    ESP.reset();
  });

  server.begin();
}




void setup()
{
  Serial.begin(115200);
  EEPROM.begin(192);

  pinMode(LED, OUTPUT);
  pinMode(SW, INPUT);
  digitalWrite(LED, HIGH);

  String user = readFromEEPROM(EEPROM_USER,64);
  String password = readFromEEPROM(EEPROM_PASSWORD,64);
  String serverAddress = readFromEEPROM(EEPROM_SERVERADDRESS,64);
  EEPROM.end();

  Serial.println("Awake");
  Serial.println("User: " + user);
  Serial.println("Password: " + password);
  Serial.println("ServerAddress" + serverAddress);

  WiFi.mode(WIFI_STA);
  WiFi.begin(user.c_str(), password.c_str());
  delay(500);
  for  (int i = 0; WiFi.status() != WL_CONNECTED; i++)
  {
    delay(500);
    if (i == 5)
    {
      performSetup();
      return;
    }
  }
  Serial.println("Here notifies the server");
  HTTPClient http;
  http.begin(serverAddress + "/opendetector-notify?state=" + (true ? "open" : "close"));
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK)
  {
    String payload = http.getString();
    if (payload == "forcesetup")
      performSetup();
  }
  else
  {
    performSetup();
    return;
  }
  http.end();


  Serial.println("Goes deep sleep");

  //ESP.deepSleep(100000000);
}

void loop()
{
  server.handleClient();
  Serial.printf("Stations connected = %d\n", WiFi.softAPgetStationNum());
  delay(500);
  bool sw = digitalRead(SW);
  digitalWrite(LED,sw);
}
