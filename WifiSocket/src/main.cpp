#include<Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266mDNS.h>

#define LED 13
#define RELAY 4

ESP8266WebServer server(80);
MDNSResponder mdns;

bool relayOn = false;

void setup()
{
  Serial.begin(115200);

  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  WiFi.begin("kovand-NET", "ingyennet1111");
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
  digitalWrite(LED, HIGH);
  server.on("/", []() {
    server.send(200, "text/html", "Bazzinga");
  });

  server.on("/control", []() {
    String relayArg = server.arg("relay");
    if (relayArg == "0")
    {
      digitalWrite(RELAY, LOW);
    }
    else if (relayArg == "1")
    {
      digitalWrite(RELAY, HIGH);
    }



    server.send(200, "text/html", "Done");
  });




  server.begin();

}

void loop()
{
      server.handleClient();
}
