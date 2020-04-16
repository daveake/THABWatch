void ShowSettingsScreen(void)
{
  ttgo->eTFT->fillScreen(TFT_BLACK);
  ttgo->eTFT->setTextColor(TFT_WHITE, TFT_BLACK);
  ttgo->eTFT->setTextFont(4);
  ttgo->eTFT->drawString("Settings",  32, 0, 4);
}


void UpdateSettingsScreen(int Always)
{
}
