#include <Arduino.h>
#include <Wire.h>
#include "axp20x.h"

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
} GPS;


struct TLoRa
{
  int               CurrentRSSI;
  int               PacketRSSI;
  int               PacketSNR;
  int               FreqErr;
  char              Payload[32];
  unsigned long     Counter;
  struct TPosition  Position;
  double            Direction;
  double            Distance;
} LoRa;

void setup()
{
  HostPort.begin(115200);
  HostPort.println("T-Watch HAB Chase Firmware V1.00");

  SetupS7xg();
   
  HostPort.println("");
  HostPort.println("Ready\n");
}

void loop()
{
  CheckS7xg();
}
