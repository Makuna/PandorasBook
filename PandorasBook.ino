#include <ESP8266WiFi.h>
#include "PandoraCommunications.h"

const char* ssid     = "AP001";
const char* password = "ItIsABadDay";

uint32_t lastTime;
PandoraClient chest;

void setup() 
{
    Serial.begin(115200);
    while (!Serial); // wait for serial attach
    Serial.println();

    chest.begin(ssid, password);
    
    lastTime = millis();
}

void loop() {

  uint32_t time = millis();
  chest.loop();
  
  if (time - lastTime > 10000)
  {
    lastTime = time;

    BookCommand command = {random(12), random(128) + 127};
    chest.sendCommand(command);
  }
}

