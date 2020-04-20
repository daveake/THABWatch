void ShowGPSScreen(void)
{
}


void UpdateGPSScreen(int Always)
{
  static unsigned long LastPositionAt=0;
  
  if ((GPS.LastPositionAt > 0) && (Always || (GPS.LastPositionAt != LastPositionAt)))
  {
    char Line[32];
    int Row, Height, Width, Column;
    
    ttgo->eTFT->setTextColor(TFT_WHITE, Background(ScreenNumber));

    // GPS Time
    ttgo->eTFT->setTextDatum(TC_DATUM);
    sprintf(Line, "%02d:%02d:%02d", GPS.Hours, GPS.Minutes, GPS.Seconds);
    ttgo->eTFT->drawString(Line, 119, 2, 7);

    Row = 58;
    Height = 93;
    
    Column = 1;
    Width = 70;

    // Latitude and Longitude
    Column = 1;
    Width = 240 - Column - 1;
    ttgo->eTFT->drawRoundRect(Column, Row, Width, Height, 5, TFT_WHITE);
    sprintf(Line, "%.5lf", GPS.Longitude);
    ttgo->eTFT->setTextDatum(TC_DATUM);
    ttgo->eTFT->drawString(Line, Column+Width/2, Row+5, 6);
    ttgo->eTFT->setTextDatum(BC_DATUM);
    sprintf(Line, "%.5lf", GPS.Latitude);
    ttgo->eTFT->drawString(Line, Column+Width/2, Row+Height-1, 6);

    Row += Height + 6;
    Height = 60;
    
    // Satellite count
    Column = 1;
    Width = 65;
    ttgo->eTFT->drawRoundRect(Column, Row, Width, Height, 5, TFT_WHITE);
    ttgo->eTFT->setTextDatum(MC_DATUM);
    ttgo->eTFT->drawNumber(GPS.Satellites, Column+Width/2, Row+Height/2-1, 6);
    ttgo->eTFT->setTextDatum(BC_DATUM);
    ttgo->eTFT->drawString("Sats", Column+Width/2, Row+Height-3, 1);

    // Altitude
    Column += Width + 5;
    Width = 240 - Column - 1;
    ttgo->eTFT->drawRoundRect(Column, Row, Width, Height, 5, TFT_WHITE);
    ttgo->eTFT->setTextDatum(MC_DATUM);
    sprintf(Line, "%ld m", GPS.Altitude);
    ttgo->eTFT->drawString(Line, Column+Width/2, Row+Height/2, 6);

    LastPositionAt = GPS.LastPositionAt;
  }
}
