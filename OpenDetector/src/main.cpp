#include<Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

#include <EEPROM.h>

#define EEPROM_USER 0
#define EEPROM_PASSWORD 64
#define EEPROM_SERVERADDRESS 128

ESP8266WebServer server(80);

void writeToEEPROM(int address,String value)
{
  int addr = address;
  for (int i=0; i < value.length();i++)
  {
    EEPROM.write(addr, value[i]);
    addr++;
  }
  EEPROM.write(addr, 0);
}

String readFromEEPROM(int address)
{
  String str = "";
  int addr = address;
  while (true)
  {
    byte ch = EEPROM.read(addr);
    addr++;
    if (ch == 0)
      break;
    else
      str += ch;
  }

}

void performSetup()
{
  Serial.println("Setup started");
  WiFi.softAP("OpenDetector-Setup");

  server.on("/", [](){
    server.send(200, "text/html", "Form here");
  });

  server.on("/process", [](){
    String user = server.arg("user");
    String password = server.arg("password");
    String serverAddress = server.arg("serveraddress");
    server.send(200, "text/html", "Rebooting...");
    ESP.reset();
  });

  server.begin();
}




void setup()
{
  Serial.begin(115200);
  EEPROM.begin(192);
  String user = readFromEEPROM(EEPROM_USER);
  String password = readFromEEPROM(EEPROM_PASSWORD);
  String serverAddress = readFromEEPROM(EEPROM_SERVERADDRESS);

  Serial.println("Awake");
  Serial.println("User: " + user);
  Serial.println("Password: " + password);
  Serial.println("ServerAddress" + serverAddress);


  if (user == "" || password == "" || serverAddress == "")
    performSetup();

  WiFi.begin(user.c_str(), password.c_str());
  delay(500);
  for  (int i = 0; WiFi.status() != WL_CONNECTED; i++)
  {
    delay(500);
    if (i == 5)
    {
      performSetup();
      break;
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
  }
  http.end();


  Serial.println("Goes deep sleep");

  ESP.deepSleep(100000000);
}

void loop()
{
    server.handleClient();
}
