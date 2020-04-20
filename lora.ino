void ShowLoRaScreen(void)
{
  int Row, Height, Width, Column;
  
  ttgo->eTFT->setTextColor(TFT_WHITE, Background(ScreenNumber));

  // Latitude and Longitude etc.
  Row = 30;
  Height = 60;
  Column = 1;
  Width = 240 - Column - 1;
  ttgo->eTFT->drawRoundRect(Column, Row, Width, Height, 5, TFT_WHITE);

  // RSSI etc
  Row += Height + 4;
  Height = 218 - Row;
  Column = 1;
  Width = 115;
  ttgo->eTFT->drawRoundRect(Column, Row, Width, Height, 5, TFT_WHITE);
  Column += Width + 5;
  Width = 240 - Column - 2;
  ttgo->eTFT->drawRoundRect(Column, Row, Width, Height, 5, TFT_WHITE);
}


void UpdateLoRaScreen(int Always)
{
  static int LastSecondsSincePosition=0;
  static unsigned long LastPositionAt=0;
  int Row, Height, Width, Column;
  int SecondsSincePosition;
  char Line[32];

  Row = 1;
  Height = 28;
  Column = 1;
  Width = 160;

  // Time-since-position updates every second
  if (LoRa.Position.LastPositionAt > 0)
  {
    SecondsSincePosition = (millis() - LoRa.Position.LastPositionAt) / 1000;

    if (SecondsSincePosition > 999) SecondsSincePosition = 999;

    if (Always || (SecondsSincePosition != LastSecondsSincePosition))
    {
      Column += Width + 5;
      Width = 249 - Width - 5;
      
      LastSecondsSincePosition = SecondsSincePosition;
      ttgo->eTFT->fillRect(Column, Row, Width, Height, Background(ScreenNumber));
      ttgo->eTFT->setTextColor(TFT_WHITE, Background(ScreenNumber));
      ttgo->eTFT->setTextDatum(MC_DATUM);
      sprintf(Line, "%ds", SecondsSincePosition);
      ttgo->eTFT->drawString(Line, Column+Width/2-4, Row+Height/2, 4);
    }
  }

  // Now do the sections that only update when there's a new position
  if ((LoRa.Position.LastPositionAt > 0) && (Always || (LoRa.Position.LastPositionAt != LastPositionAt)))
  {
    static char OldPayload[32];
    char Line[32];
    
    Column = 1;
    Width = 150;
    Height = 28;
    
    ttgo->eTFT->setTextColor(TFT_WHITE, Background(ScreenNumber));

    // Payload ID
    if (Always || (strcmp(OldPayload, LoRa.Payload) != 0))
    {
      ttgo->eTFT->fillRect(Column, Row, Width, Height, Background(ScreenNumber));
      strcpy(OldPayload, LoRa.Payload);
      ttgo->eTFT->setTextDatum(MC_DATUM);
      ttgo->eTFT->drawString(LoRa.Payload, Column+Width/2, Row+Height/2, 4);
    }
   
    // Time and Position
    Row += Height + 5;
    Height = 60;
    ttgo->eTFT->fillRect(3, Row+2, 233, Height-10, Background(ScreenNumber));
    ttgo->eTFT->setTextDatum(TC_DATUM);
    sprintf(Line, "%02d:%02d:%02d", LoRa.Position.Hours, LoRa.Position.Minutes, LoRa.Position.Seconds);
    ttgo->eTFT->drawString(Line, 59, Row, 4);
    sprintf(Line, "%.5lf", LoRa.Position.Latitude);
    ttgo->eTFT->drawString(Line, 181, Row, 4);
    Row += 28;
    sprintf(Line, "%ld m", LoRa.Position.Altitude);
    ttgo->eTFT->drawString(Line, 59, Row, 4);
    sprintf(Line, "%.5lf", LoRa.Position.Longitude);
    ttgo->eTFT->drawString(Line, 181, Row, 4);

    // Miscellaneous stuff
    Row = 94;
    Height = 218 - Row;
    ttgo->eTFT->fillRect(3, Row+2, 111, Height-4, Background(ScreenNumber));
    ttgo->eTFT->fillRect(124, Row+2, 111, Height-4, Background(ScreenNumber));

    Row += 2;
    sprintf(Line, "%.1f", LoRa.AscentRate);
    ttgo->eTFT->drawString(Line, 59, Row, 4);
    ttgo->eTFT->drawString("Rate (m/s)", 59, Row+20, 2);
    sprintf(Line, "%lu", LoRa.Counter);
    ttgo->eTFT->drawString(Line, 179, Row, 4);
    ttgo->eTFT->drawString("Counter", 179, Row+20, 2);
    
    Row += 41;
    sprintf(Line, "%dHz", LoRa.FreqErr);
    ttgo->eTFT->drawString(Line, 59, Row, 4);
    ttgo->eTFT->drawString("Freq. Error", 59, Row+20, 2);
    sprintf(Line, "%d", LoRa.PacketSNR);
    ttgo->eTFT->drawString(Line, 179, Row, 4);
    ttgo->eTFT->drawString("Packet SNR", 179, Row+20, 2);
    
    Row += 41;
    sprintf(Line, "%d", LoRa.CurrentRSSI);
    ttgo->eTFT->drawString(Line, 59, Row, 4);
    ttgo->eTFT->drawString("CurrentRSSI", 59, Row+20, 2);
    sprintf(Line, "%d", LoRa.PacketRSSI);
    ttgo->eTFT->drawString(Line, 179, Row, 4);
    ttgo->eTFT->drawString("Packet RSSI", 179, Row+20, 2);
    
    LastPositionAt = LoRa.Position.LastPositionAt;
  }
}
