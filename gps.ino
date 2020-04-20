void ShowGPSScreen(void)
{
  int Row, Height, Width, Column;
  
  ttgo->eTFT->setTextColor(TFT_WHITE, Background(ScreenNumber));

  Row = 58;
  Height = 93;
  Column = 1;
  Width = 70;

  // Latitude and Longitude
  Column = 1;
  Width = 240 - Column - 1;
  ttgo->eTFT->drawRoundRect(Column, Row, Width, Height, 5, TFT_WHITE);

  Row += Height + 6;
  Height = 60;
  
  // Satellite count
  Column = 1;
  Width = 65;
  ttgo->eTFT->drawRoundRect(Column, Row, Width, Height, 5, TFT_WHITE);

  // Altitude
  Column += Width + 5;
  Width = 240 - Column - 1;
  ttgo->eTFT->drawRoundRect(Column, Row, Width, Height, 5, TFT_WHITE);
}


void UpdateGPSScreen(int Always)
{
  static unsigned long LastPositionAt=0;
  
  if ((GPS.LastPositionAt > 0) && (Always || (GPS.LastPositionAt != LastPositionAt)))
  {
    static double LastLatitude=-999;
    static double LastLongitude=-999;
    static long LastAltitude=-999;
    static int LastSatellites=-1;
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

    if (Always || (GPS.Latitude != LastLatitude) || (GPS.Longitude != LastLongitude))
    {
      LastLatitude = GPS.Latitude;
      LastLongitude = GPS.Longitude;
      
      ttgo->eTFT->fillRect(Column+2, Row+2, Width-4, Height-4, Background(ScreenNumber));
      sprintf(Line, "%.5lf", GPS.Latitude);
      ttgo->eTFT->setTextDatum(TC_DATUM);
      ttgo->eTFT->drawString(Line, Column+Width/2, Row+5, 6);
      ttgo->eTFT->setTextDatum(BC_DATUM);
      sprintf(Line, "%.5lf", GPS.Longitude);
      ttgo->eTFT->drawString(Line, Column+Width/2, Row+Height-1, 6);
    }

    Row += Height + 6;
    Height = 60;
    
    // Satellite count
    Column = 1;
    Width = 65;
    if (Always || (GPS.Satellites != LastSatellites))
    {
      LastSatellites = GPS.Satellites;
      
      ttgo->eTFT->fillRect(Column+2, Row+2, Width-4, Height-4, Background(ScreenNumber));
      ttgo->eTFT->setTextDatum(MC_DATUM);
      ttgo->eTFT->drawNumber(GPS.Satellites, Column+Width/2, Row+Height/2-1, 6);
      ttgo->eTFT->setTextDatum(BC_DATUM);
      ttgo->eTFT->drawString("Sats", Column+Width/2, Row+Height-3, 1);
    }
    
    // Altitude
    Column += Width + 5;
    Width = 240 - Column - 1;
    if (Always || (GPS.Altitude != LastAltitude))
    {
      LastAltitude = GPS.Altitude;
      ttgo->eTFT->fillRect(Column+2, Row+2, Width-4, Height-4, Background(ScreenNumber));
      ttgo->eTFT->setTextDatum(MC_DATUM);
      sprintf(Line, "%ld m", GPS.Altitude);
      ttgo->eTFT->drawString(Line, Column+Width/2, Row+Height/2, 6);
    }
    
    LastPositionAt = GPS.LastPositionAt;
  }
}
