#include<Arduino.h>

#define RELAY 0

bool relayOn = false;

void setup()
{
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW);

}

void loop()
{
  delay(5000);
  if (relayOn)
    digitalWrite(RELAY, HIGH);
  else
    digitalWrite(RELAY, LOW);
  relayOn = !relayOn;


}
