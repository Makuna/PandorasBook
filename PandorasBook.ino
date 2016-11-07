#include <ESP8266WiFi.h>
#include <PandoraCommunications.h>
#include <AnalogKeypad.h>

const char* ssid     = "AP001";
const char* password = "ItIsABadDay";

PandoraClient chest;
AnalogKeypad keypad(A0, 500);

void setup() 
{
    Serial.begin(115200);
    while (!Serial); // wait for serial attach
    Serial.println();

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
    Serial.print(".");
    break;
    
  case PandoraClientState_WifiConnected:
    Serial.println("Wifi Connected");
    break;
    
  case PandoraClientState_ClientConnecting:
    Serial.print("+");
    break;
    
  case PandoraClientState_ClientConnected:
    Serial.println("Client connected");
    break;

  case PandoraClientState_DeviceReset:    
    Serial.println("RESETTING");
    Serial.flush();
    break;
  }
}

void loop() {
  chest.loop(onClientStatusChange);
  keypad.loop(onKeyEvent);
}

