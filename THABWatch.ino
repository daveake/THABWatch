#include <TTGO.h>
#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>

#define SOC_GPIO_PIN_TWATCH_SEN_SDA     21
#define SOC_GPIO_PIN_TWATCH_SEN_SCL     22
#define SOC_GPIO_PIN_TWATCH_RX          34
#define SOC_GPIO_PIN_TWATCH_TX          33

AXP20X_Class axp;

#define HostPort  Serial
#define s7xgPort  Serial1

int ScreenNumber;   // 0=Logo, 1=GPS, 2=LoRa, 3=Dir, 4=Settings

struct TPosition
{
  int           Hours, Minutes, Seconds;
  double        Longitude, Latitude;
  long          Altitude;
  int           Speed;
  int           Direction;
  unsigned long LastPositionAt;
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
  double            Distance;
  int               GotDistanceAndDirection, DistanceAndDirectionAreValid;
} LoRa;

struct TSettings
{
  float   Frequency;
  int     Mode;
//  int     ImplicitOrExplicit;
//  int     ErrorCoding;
//  int     Bandwidth;
//  int     SpreadingFactor;
//  int     LowDataRateOptimize;
} Settings;

#define EEPROM_SIZE (sizeof(Settings) + 2)

TTGOClass *ttgo;


void setup()
{
  EEPROM.begin(EEPROM_SIZE);  

  HostPort.begin(115200);
  HostPort.println("\nT-Watch HAB Chase Firmware V1.00\n");

  if ((EEPROM.read(0) == 'D') && (EEPROM.read(1) == 'A'))
  {
    // Load settings from EEPROM
    LoadSettings();
    HostPort.println("Settings loaded from EEPROM");
  }
  else
  {
    LoadDefaults();
    HostPort.println("Loaded default settings");
    SaveSettings();
  }

  SetupS7xg();      // Do this BEFORE SetupWatch otherwise it stops touchscreen presses from working (possibly i2c issue ?)

  SetupWatch();
 
  SetupButton();

  ShowScreen(ScreenNumber = 0);    // Logo screen

  SendLoRaMode(Settings.Mode);
  SendLoRaFrequency(Settings.Frequency);
    
  HostPort.println("");
  HostPort.println("Ready\n");
}

void loop()
{
  CheckButton();

  CheckS7xg();

  CheckTouch();

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
  ShowScreen(ScreenNumber = (ScreenNumber + 1) % 5);
}

void LongButtonPress(void)
{
}

void ShowScreen(int ScreenNumber)    // 0=GPS, 1=LoRa, 2=Dir, 3=Settings
{
  if (ScreenNumber == 0)
  {
    ShowLogocreen();
  }
  else if (ScreenNumber == 1)
  {
    ShowGPSScreen();
  }
  else if (ScreenNumber == 2)
  {
    ShowLoRaScreen();
  }
  else if (ScreenNumber == 3)
  {
    ShowDirectionScreen();
  }
  else if (ScreenNumber == 4)
  {
    ShowSettingsScreen();
  }  

  UpdateScreen(ScreenNumber, 1);
}
 
void UpdateScreen(int ScreenNumber, int Always)
{
  if (ScreenNumber == 0)
  {
    UpdateLogoScreen(Always);
  }
  else if (ScreenNumber == 1)
  {
    UpdateGPSScreen(Always);
  }
  else if (ScreenNumber == 2)
  {
    UpdateLoRaScreen(Always);
  }
  else if (ScreenNumber == 3)
  {
    UpdateDirectionScreen(Always);
  }
  else if (ScreenNumber == 4)
  {
    UpdateSettingsScreen(Always);
  }  
}

void ScreenPress(int x, int y)
{
  if (ScreenNumber == 0)
  {
    // LogoScreenPress(x, y);
  }
  else if (ScreenNumber == 1)
  {
    // GPSScreenPress(x, y);
  }
  else if (ScreenNumber == 2)
  {
    // LoRaScreenPress(x, y);
  }
  else if (ScreenNumber == 3)
  {
    // DirectionScreenPress(x, y);
  }
  else if (ScreenNumber == 4)
  {
    SettingsScreenPress(x, y);
  }  
}


void LoadDefaults()
{
  Settings.Mode = 1;
  Settings.Frequency = 434.275;

//  Settings.PPM = 0.0;
//  Settings.Rx.ImplicitOrExplicit = 1;
//  Settings.Rx.ErrorCoding = 5;
//  Settings.Rx.Bandwidth = 3;
//  Settings.Rx.SpreadingFactor = 6;

}

void LoadSettings(void)
{
  int i;
  unsigned char *ptr;

  ptr = (unsigned char *)(&Settings);
  for (i=0; i<sizeof(Settings); i++, ptr++)
  {
    *ptr = EEPROM.read(i+2);
  }
}

void SaveSettings(void)
{
  int i;
  unsigned char *ptr;
  
  // Signature
  EEPROM.write(0, 'D');
  EEPROM.write(1, 'A');

  // Settings
  ptr = (unsigned char *)(&Settings);
  for (i=0; i<sizeof(Settings); i++, ptr++)
  {
    EEPROM.write(i+2, *ptr);
  }

  EEPROM.commit();
}
