void ShowGPSScreen(void)
{
  ttgo->eTFT->fillScreen(TFT_BLACK);
  ttgo->eTFT->setTextColor(TFT_WHITE, TFT_BLACK);
  ttgo->eTFT->setTextFont(4);
  ttgo->eTFT->drawString("GPS", 90, 0, 4);
  ttgo->eTFT->drawRect(40, 60, 160, 135, TFT_YELLOW);
  ttgo->eTFT->drawString("Waiting", 75, 70, 4);
}


void UpdateGPSScreen(int Always)
{
  static unsigned long LastPositionAt=0;
  
  if ((GPS.LastPositionAt > 0) && (Always || (GPS.LastPositionAt != LastPositionAt)))
  {
    char Line[32];
    
    ttgo->eTFT->setTextColor(TFT_WHITE, TFT_BLACK);

    sprintf(Line, "%02d:%02d:%02d", GPS.Hours, GPS.Minutes, GPS.Seconds);
    ttgo->eTFT->drawString(Line, 75, 70, 4);

    sprintf(Line, "%.5lf", GPS.Latitude);
    ttgo->eTFT->drawString(Line, 70, 100, 4);
    
    sprintf(Line, "%.5lf", GPS.Longitude);
    ttgo->eTFT->drawString(Line, 70, 130, 4);

    sprintf(Line, "%ld m", GPS.Altitude);
    ttgo->eTFT->drawString(Line, 100, 160, 4);
    
    LastPositionAt = GPS.LastPositionAt;
  }
}
