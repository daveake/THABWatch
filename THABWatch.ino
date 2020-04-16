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


struct TPosition
{
  int           Hours, Minutes, Seconds;
  double        Longitude, Latitude;
  long          Altitude;
  int           GotNewPosition;
} GPS;


struct TLoRa
{
  int               CurrentRSSI, GotNewCurrentRSSI;
  int               PacketRSSI;
  int               PacketSNR;
  int               FreqErr;
  char              Payload[32];
  unsigned long     Counter;
  struct TPosition  Position;
  double            Direction;
  double            Distance;
  int               GotDistanceAndDirection;
} LoRa;

TTGOClass *ttgo;


void setup()
{
  HostPort.begin(115200);
  HostPort.println("T-Watch HAB Chase Firmware V1.00");

  SetupWatch();
  
  SetupButton();
  
  SetupS7xg();
   
  HostPort.println("");
  HostPort.println("Ready\n");
}

void loop()
{
  char Line[32];
  
  CheckButton();
  
  CheckS7xg();

  if (GPS.GotNewPosition)
  {
    sprintf(Line, "%02d:%02d:%02d", GPS.Hours, GPS.Minutes, GPS.Seconds);
    ttgo->eTFT->setTextColor(TFT_WHITE, TFT_BLACK);
    ttgo->eTFT->drawString(Line,  5, 160, 4);
    GPS.GotNewPosition = 0;
  }


}

void SetupWatch(void)
{
  ttgo = TTGOClass::getWatch();
  ttgo->begin();
  ttgo->openBL();

  ttgo->eTFT->fillScreen(TFT_BLACK);
  ttgo->eTFT->setTextColor(TFT_WHITE, TFT_BLACK);
  ttgo->eTFT->setTextFont(4);
  ttgo->eTFT->drawString("User Button Test",  5, 100, 4);
}
