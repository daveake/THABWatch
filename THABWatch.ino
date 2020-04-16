#include <Arduino.h>
#include <Wire.h>
#include "axp20x.h"

#define SOC_GPIO_PIN_TWATCH_SEN_SDA     21
#define SOC_GPIO_PIN_TWATCH_SEN_SCL     22
#define SOC_GPIO_PIN_TWATCH_RX          34
#define SOC_GPIO_PIN_TWATCH_TX          33

AXP20X_Class axp;

void setup()
{
    Serial.begin(115200);
    Serial.println("T-Watch HAB Chase Firmware V1.00");

    Serial.println("");
    Serial.print("Powering up S7xG Booard ...");

    Wire1.begin(SOC_GPIO_PIN_TWATCH_SEN_SDA , SOC_GPIO_PIN_TWATCH_SEN_SCL);
    axp.begin(Wire1, AXP202_SLAVE_ADDRESS);
    axp.setLDO3Mode(1);
    axp.setPowerOutPut(AXP202_LDO3, AXP202_ON); // S76G (MCU + LoRa)
    axp.setLDO4Voltage(AXP202_LDO4_1800MV);
    axp.setPowerOutPut(AXP202_LDO4, AXP202_ON); // S76G (Sony GNSS)

    Serial.println("");
    Serial.print("Connecting to S7xG Booard ...");
    Serial1.begin(115200, SERIAL_8E1, SOC_GPIO_PIN_TWATCH_RX, SOC_GPIO_PIN_TWATCH_TX);

    Serial.println("");
    Serial.println("Ready\n");
}

void loop()
{
  while (Serial.available() > 0)
  {
    Serial1.write(Serial.read());
  }
  
  while (Serial1.available() > 0)
  {
    Serial.write(Serial1.read());
  }
}
