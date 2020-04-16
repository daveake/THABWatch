#include <TTGO.h>
#include <Arduino.h>
#include <Wire.h>
// #include "axp20x.h"

#define SOC_GPIO_PIN_TWATCH_SEN_SDA     21
#define SOC_GPIO_PIN_TWATCH_SEN_SCL     22
#define SOC_GPIO_PIN_TWATCH_RX          34
#define SOC_GPIO_PIN_TWATCH_TX          33

AXP20X_Class axp;

#define HostPort  Serial
#define s7xgPort  Serial1

int ScreenNumber;   // 0=GPS, 1=LoRa, 2=Dir, 3=Settings

struct TPosition
{
  int           Hours, Minutes, Seconds;
  double        Longitude, Latitude;
  long          Altitude;
  int           PositionIsValid;
  int           GotNewPosition;
} GPS;


struct TLoRa
{
  int               CurrentRSSI, GotNewCurrentRSSI, CurrentRSSIIsValid;
  int               PacketRSSI;
  int               PacketSNR;
  int               FreqErr;
  char              Payload[32];
  unsigned long     Counter;
  struct TPosition  Position;
  double            Direction;
  double            Distance;
  int               GotDistanceAndDirection, DistanceAndDirectionAreValid;
} LoRa;

TTGOClass *ttgo;


void setup()
{
  HostPort.begin(115200);
  HostPort.println("T-Watch HAB Chase Firmware V1.00");

  SetupWatch();
  
  SetupButton();

  ShowScreen(ScreenNumber = 0);    // 0=GPS, 1=LoRa, 2=Dir, 3=Settings
  
  SetupS7xg();
   
  HostPort.println("");
  HostPort.println("Ready\n");
}

void loop()
{
  CheckButton();
  
  CheckS7xg();

  UpdateScreen(ScreenNumber, 0);
}

void SetupWatch(void)
{
  ttgo = TTGOClass::getWatch();
  ttgo->begin();
  ttgo->openBL();
}

void ShortButtonPress(void)
{
  ShowScreen(ScreenNumber = (ScreenNumber + 1) % 4);
}

void LongButtonPress(void)
{
}

void ShowScreen(int ScreenNumber)    // 0=GPS, 1=LoRa, 2=Dir, 3=Settings
{
  if (ScreenNumber == 0)
  {
    ShowGPSScreen();
  }
  else if (ScreenNumber == 1)
  {
    ShowLoRaScreen();
  }
  else if (ScreenNumber == 2)
  {
    ShowDirectionScreen();
  }
  else if (ScreenNumber == 3)
  {
    ShowSettingsScreen();
  }  

  UpdateScreen(ScreenNumber, 1);
}
 
void UpdateScreen(int ScreenNumber, int Always)    // 0=GPS, 1=LoRa, 2=Dir, 3=Settings
{
  if (ScreenNumber == 0)
  {
    UpdateGPSScreen(Always);
  }
  else if (ScreenNumber == 1)
  {
    UpdateLoRaScreen(Always);
  }
  else if (ScreenNumber == 2)
  {
    UpdateDirectionScreen(Always);
  }
  else if (ScreenNumber == 3)
  {
    UpdateSettingsScreen(Always);
  }  
 }
