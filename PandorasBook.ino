#include <ESP8266WiFi.h>
#include <PandoraCommunications.h>
#include <AnalogKeypad.h>
#include "StatusLed.h"

const char* ssid     = "AP001";
const char* password = "ItIsABadDay";

PandoraClient chest;
AnalogKeypad keypad(A0, 500);
StatusLed<LedInvertedPinLevelFeature> statusLed(LED_BUILTIN);

void setup() 
{

//    Serial.begin(115200);
//    while (!Serial); // wait for serial attach
//    Serial.println();

    statusLed.begin();
    statusLed.setBlinkBeat(0b1010101010101010, 250);
    
    chest.begin(ssid, password);
}

void onKeyEvent(const ButtonParam& param)
{
    BookCommand command;
    command.command = param.button;
    command.param = param.state;
    chest.sendCommand(command);
}

void onClientStatusChange(const PandoraClientStatus& status)
{
  switch (status.state)
  {
  case PandoraClientState_WifiConnecting:
    statusLed.setBlinkBeat(0b1010101010101010, 250);
    break;
    
  case PandoraClientState_WifiConnected:
    statusLed.setBlinkBeat(0b1010000010100000, 250);
    break;
    
  case PandoraClientState_ClientConnecting:
    statusLed.setBlinkBeat(0b1010100010101000, 250);
    break;
    
  case PandoraClientState_ClientConnected:
    statusLed.stop();
    break;

  case PandoraClientState_DeviceReset:    
    break;
  }
}

void loop() {
  chest.loop(onClientStatusChange);
  keypad.loop(onKeyEvent);
  statusLed.loop();
}

