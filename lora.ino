void ShowLoRaScreen(void)
{
  ttgo->eTFT->fillScreen(TFT_BLACK);
  ttgo->eTFT->setTextColor(TFT_WHITE, TFT_BLACK);
  ttgo->eTFT->setTextFont(4);
  ttgo->eTFT->drawString("LoRa", 80, 0, 4);
  ttgo->eTFT->drawRect(40, 50, 160, 160, TFT_YELLOW);
}


void UpdateLoRaScreen(int Always)
{
  if (LoRa.Position.PositionIsValid && (Always || LoRa.Position.GotNewPosition))
  {
    char Line[32];
    
    ttgo->eTFT->setTextColor(TFT_WHITE, TFT_BLACK);

    sprintf(Line, "%s", LoRa.Payload);
    ttgo->eTFT->drawString(Line, 90, 60, 4);

    sprintf(Line, "%02d:%02d:%02d", LoRa.Position.Hours, LoRa.Position.Minutes, LoRa.Position.Seconds);
    ttgo->eTFT->drawString(Line, 75, 90, 4);

    sprintf(Line, "%.5lf", LoRa.Position.Latitude);
    ttgo->eTFT->drawString(Line, 70, 120, 4);
    
    sprintf(Line, "%.5lf", LoRa.Position.Longitude);
    ttgo->eTFT->drawString(Line, 70, 150, 4);

    sprintf(Line, "%ld m", LoRa.Position.Altitude);
    ttgo->eTFT->drawString(Line, 85, 180, 4);
    
    LoRa.Position.GotNewPosition = 0;
  }
}
