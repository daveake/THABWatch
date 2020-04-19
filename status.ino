void ShowStatusBar(void)
{
  ttgo->eTFT->setTextDatum(MC_DATUM);

  // GPS Status - nothing yet - it's all done during updates

  // Battery Status - all done in updates

  // LoRa Signal - draw container
  ttgo->eTFT->drawRect(128, 221, 52, 18, TFT_WHITE);

  // loRa Status - all done in updates

  ttgo->eTFT->setTextDatum(TL_DATUM); 
}

void UpdateStatusBar(void)
{
  UpdateGPSStatus();
  UpdateBatteryStatus();
  UpdateLoRaRSSIStatus();
  UpdateLoRaStatus();
}


void UpdateGPSStatus(void)
{
  static int GPS_Status=-1;
  
  if (GPS_Status != GPSStatus())
  {
    GPS_Status = GPSStatus();

    ttgo->eTFT->setTextDatum(MC_DATUM);
    ttgo->eTFT->fillRoundRect(0, 221, 52, 18, 3, ColourFromStatus(GPS_Status));
    ttgo->eTFT->setTextColor(TFT_BLACK, ColourFromStatus(GPS_Status));
    ttgo->eTFT->drawString("GPS", 26, 230, 2);
  }
}

void UpdateBatteryStatus(void)
{
  static unsigned int BatteryVoltage=999;

  if (BatteryVoltage != SystemInfo.BatteryVoltage)
  {
    int Width, Colour, CellColour;
    
    BatteryVoltage = SystemInfo.BatteryVoltage;
    if (BatteryVoltage < 3000)
    {
      Width = 0;
    }
    else if (BatteryVoltage > 4200)
    {
      Width = (BatteryVoltage - 3000) / 30;
    }
    else
    {
      Width = 40;
    }
    
    if (Width < 10)
    {
      Colour = TFT_RED;
      CellColour = TFT_RED;
    }
    else if (Width < 20)
    {
      Colour = TFT_YELLOW;
      CellColour = TFT_WHITE;
    }
    else
    {
      Colour = TFT_GREEN;
      CellColour = TFT_WHITE;
    }

    ttgo->eTFT->drawRect(61, 221, 52, 18, CellColour);
    ttgo->eTFT->fillRect(113, 226, 4, 8, CellColour);
    ttgo->eTFT->fillRect(62, 222, 40, 16, TFT_BLACK);
    ttgo->eTFT->fillRect(62, 222, Width, 16, Colour);
  }
}

void UpdateLoRaRSSIStatus(void)
{
  static int LoRa_RSSI=-999;

  if (LoRa.CurrentRSSI != LoRa_RSSI)
  {
    static int LastWidth=0;
    int Width;
    
    LoRa_RSSI = LoRa.CurrentRSSI;

    Width = ((LoRa_RSSI + 150) * 4) / 5;
    if (Width < 1) Width = 1;
    if (Width > 40) Width = 40;

    if (Width > LastWidth)
    {
      ttgo->eTFT->fillRect(129+LastWidth, 222, Width-LastWidth, 16, TFT_GREEN);
    }
    else if (Width < LastWidth)
    {
      ttgo->eTFT->fillRect(129+Width, 222, LastWidth-Width, 16, TFT_BLACK);
    }
  }
}

void UpdateLoRaStatus(void)
{
  static int LoRa_Status=-1;

  if (LoRa_Status != LoRaStatus())
  {
    LoRa_Status = LoRaStatus();

    ttgo->eTFT->setTextDatum(MC_DATUM);
    ttgo->eTFT->fillRoundRect(188, 221, 52, 18, 3, ColourFromStatus(LoRa_Status));
    ttgo->eTFT->setTextColor(TFT_BLACK, ColourFromStatus(LoRa_Status));
    ttgo->eTFT->drawString("LoRa", 214, 230, 2);
  }
  
  ttgo->eTFT->setTextDatum(TL_DATUM); 
}
