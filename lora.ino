void ShowLoRaScreen(void)
{
  ttgo->eTFT->setTextColor(TFT_WHITE, TFT_BLACK);
  ttgo->eTFT->setTextFont(4);
  ttgo->eTFT->drawString("LoRa", 80, 0, 4);
  ttgo->eTFT->drawRect(40, 40, 160, 160, TFT_YELLOW);
}


void UpdateLoRaScreen(int Always)
{
  static unsigned long LastPositionAt=0;
  
  if ((LoRa.Position.LastPositionAt > 0) && (Always || (LoRa.Position.LastPositionAt != LastPositionAt)))
  {
    char Line[32];
    
    ttgo->eTFT->setTextDatum(TL_DATUM);
    ttgo->eTFT->setTextColor(TFT_WHITE, TFT_BLACK);

    sprintf(Line, "%s", LoRa.Payload);
    ttgo->eTFT->drawString(Line, 90, 50, 4);

    sprintf(Line, "%02d:%02d:%02d", LoRa.Position.Hours, LoRa.Position.Minutes, LoRa.Position.Seconds);
    ttgo->eTFT->drawString(Line, 75, 80, 4);

    sprintf(Line, "%.5lf", LoRa.Position.Latitude);
    ttgo->eTFT->drawString(Line, 70, 110, 4);
    
    sprintf(Line, "%.5lf", LoRa.Position.Longitude);
    ttgo->eTFT->drawString(Line, 70, 140, 4);

    sprintf(Line, "%ld m", LoRa.Position.Altitude);
    ttgo->eTFT->drawString(Line, 85, 170, 4);
    
    LastPositionAt = LoRa.Position.LastPositionAt;
  }
}
