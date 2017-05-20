#include<Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266mDNS.h>

#include <SPI.h>

#define LED 13  //mosi
#define RELAY 4
#define BUTTON 2

#define SCL 14 //clock //clk
#define SDA 12 //data  //miso
#define SCS 15 // chip select //SS
#define SYN 5 //read/wite


ESP8266WebServer server(80);
MDNSResponder mdns;

bool relayOn = false;
bool ledOn = false;

uint32_t reg_data[8];  // array to hold read data, max is 8


void read_regs(uint32_t data[], uint8_t len){  // an array to hold register data is passed
  digitalWrite(SYN, LOW);  // latching operation
  digitalWrite(SCS, LOW);
  digitalWrite(SYN, HIGH);

  uint32_t val = 0;
  for (int i = 0; i < len; i++){  // read the 8 32-bit registers into the array
    val = SPI.transfer(0);  // your 32 serial clocks
    val |= (uint32_t)SPI.transfer(0) << 8;
    val |= (uint32_t)SPI.transfer(0) << 16;
    val |= (uint32_t)SPI.transfer(0) << 24;
    data[i] = val;
  }
  digitalWrite(SCS, HIGH);  // deassert SCS
}


void write_reg(byte addr, byte val){
  val = (val << 7) | (addr << 1);  // 6-bit addresses!
  SPI.end();  // disable SPI
  pinMode(MISO, OUTPUT);
  digitalWrite(SYN, HIGH);
  delayMicroseconds(10);

  digitalWrite(SCS, LOW);  // assert SCS and SYN
  digitalWrite(SYN, LOW);
  for (int i = 0; i < 8; i++){  // shift out a byte
    delayMicroseconds(10);
    digitalWrite(SCK, LOW);  // max clock is 100kHz
    digitalWrite(MISO, (val & 0x80) ? HIGH : LOW);
    delayMicroseconds(10);
    digitalWrite(SCK, HIGH);
    val <<= 1;
  }

  digitalWrite(SYN, HIGH); // deassert SCS and SYN
  digitalWrite(SCS, HIGH);

  SPI.beginTransaction(SPISettings(32000000, MSBFIRST, SPI_MODE3)); //restart SPI
}


/**
 * @brief Creates the HTML source of the main page, in a human readable form. Displayed: Temp., Press., Hum., RGBW, UVAB
 */
String createMainPage()
{
  String src = "";
  src += "<!DOCTYPE html>\n";
  src += "<html><head><style>\n";
  src += ".switch {\n";
  src += "  position: relative;\n";
  src += "  display: inline-block;\n";
  src += "  width: 80px;\n";
  src += "  height: 44px;\n";
  src += "}\n";
  src += ".switch input {display:none;}\n";
  src += ".slider {\n";
  src += "  position: absolute;\n";
  src += "  cursor: pointer;\n";
  src += "  top: 0;\n";
  src += "  left: 0;\n";
  src += "  right: 0;\n";
  src += "  bottom: 0;\n";
  src += "  background-color: #ccc;\n";
  src += "  -webkit-transition: .4s;\n";
  src += "  transition: .4s;\n";
  src += "}\n";
  src += ".slider:before {\n";
  src += "  position: absolute;\n";
  src += "  content: \"\";\n";
  src += "  height: 36px;\n";
  src += "  width: 36px;\n";
  src += "  left: 4px;\n";
  src += "  bottom: 4px;\n";
  src += "  background-color: white;\n";
  src += "  -webkit-transition: .4s;\n";
  src += "  transition: .4s;\n";
  src += "}\n";
  src += "input:checked + .slider {\n";
  src += "  background-color: #2196F3;\n";
  src += "}\n";
  src += "input:focus + .slider {\n";
  src += "  box-shadow: 0 0 1px #2196F3;\n";
  src += "}\n";
  src += "input:checked + .slider:before {\n";
  src += "  -webkit-transform: translateX(36px);\n";
  src += "  -ms-transform: translateX(36px);\n";
  src += "  transform: translateX(36px);\n";
  src += "}\n";
  src += "</style></head><body onload=\"setInterval(refreshData, 500)\">\n";
  src += "<h2>Power: <span id=\"power\"></span> W</h2>\n";
  src += "<h3>Voltage: <span id=\"voltage\"></span> V</h3>\n";
  src += "<h3>Current: <span id=\"current\"></span> A</h3>\n";
  src += "<h3>Power factor: <span id=\"powerfactor\"></span></h3>\n";
  src += "<label class=\"switch\">\n";
  src += "  <input id=\"sw\" type=\"checkbox\" onclick=\"myFunction(this)\">\n";
  src += "  <div class=\"slider\"></div>\n";
  src += "</label>\n";
  src += "<script>\n";
  src += "function myFunction(sender)\n";
  src += "{\n";
  src += "  var http = new XMLHttpRequest();\n";
  src += "  var url = \"/control\";\n";
  src += "  var params = \"relay=\" + (sender.checked?\"1\":\"0\");\n";
  src += "  http.open(\"GET\", url+\"?\"+params, true);\n";
  src += "  http.send(null);\n";
  src += "}\n";
  src += "function refreshData()\n";
  src += "{\n";
  src += "  var http = new XMLHttpRequest();\n";
  src += "  var url = \"/json\";\n";
  src += "  http.onreadystatechange = function() {";
  src += "    if (this.readyState == 4 && this.status == 200) {";
  src += "      var arr = JSON.parse(this.responseText);";
  src += "      document.getElementById(\"power\").innerHTML = arr.power;";
  src += "      document.getElementById(\"voltage\").innerHTML = arr.voltage;";
  src += "      document.getElementById(\"current\").innerHTML = arr.current;";
  src += "      document.getElementById(\"powerfactor\").innerHTML = arr.powerfactor;";
  src += "      document.getElementById(\"sw\").checked = (arr.relay == \"1\");";
  src += "    }\n";
  src += "  }\n";
  src += "  http.open(\"GET\", url, true);\n";
  src += "  http.send(null);\n";
  src += "}\n";
  src += "</script></body></html>\n";
  return src;
}

/**
 * @brief JSON info page: type, id
 */
String createInfoPage(String id) //type hardcoded
{
  String src = "";
  src += "{\n";
  src += "  \"type\": \"soc\",\n";
  src += "  \"id\": \"" + id + "\"\n";
  src += "}\n";
  return src;
}

/**
 * @brief Displays the measured data in JSON format.  Displayed: Temp., Press., Hum., RGBW, UVAB
 */
String createJSONPage(float power,float volt, float curr,float powf) //led and relay from global
{
  String src = "";
  src += "{\n";
  src += "  \"relay\": \"" + String(relayOn?"1":"0") + "\",\n";
  src += "  \"led\": \"" + String(ledOn?"1":"0") + "\",\n";
  src += "  \"power\": \"" + String(power) + "\",\n";
  src += "  \"voltage\": \"" + String(volt) + "\",\n";
  src += "  \"current\": \"" + String(curr) + "\",\n";
  src += "  \"powerfactor\": \"" + String(powf) + "\"\n";
  src += "}\n";
  return src;
}


void onButtonPush()
{
  write_reg(47, 1);  // set config bit 47
  delay(1);
  read_regs(reg_data, 8);  // read all reg data into array
  for (int i = 0; i < 8; i++)
    Serial.println(reg_data[i], HEX);
}

void setup()
{
  Serial.begin(115200);

  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  ledOn = true;

  pinMode(BUTTON, INPUT);
  attachInterrupt(BUTTON, onButtonPush, FALLING);

  WiFi.begin("AndroidAP", "ifez0433");
  delay(1000);
  Serial.print("Waiting for connection");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println("TODO");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());


  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
  digitalWrite(LED, LOW);
  ledOn = false;
  server.on("/", []() {
    server.send(200, "text/html", createMainPage());
  });

  server.on("/control", []() {
    String relayArg = server.arg("relay");
    if (relayArg == "0")
    {
      digitalWrite(RELAY, LOW);
      relayOn = false;
    }
    else if (relayArg == "1")
    {
      digitalWrite(RELAY, HIGH);
      relayOn = true;
    }
    server.send(200, "text/html", "Done");
  });

  server.on("/info", []() {
    server.send(200, "text/html", createInfoPage(String(ESP.getChipId())));
  });

  server.on("/json", []() {
    server.send(200, "text/html", createJSONPage(1.1f, 2.2f, 3.3f, 4.4f));
  });
  server.begin();


  pinMode(SS, OUTPUT);  //needed to use the SPI
  pinMode(SCS, OUTPUT);  // set SCS and SYN as output
  pinMode(SYN, OUTPUT);
  digitalWrite(SCS, HIGH);  // idle state
  digitalWrite(SYN, HIGH);
  SPI.beginTransaction(SPISettings(32000000, MSBFIRST, SPI_MODE3));

}

void loop()
{
      server.handleClient();
}
