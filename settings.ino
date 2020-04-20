struct TButton
{
  int           x;
  int           y;
  int           w;
  int           h;
  char          *Caption;
  void          (*Pressed)(void); 
};

void ShowSettingsScreen(void)
{
  ttgo->eTFT->drawRoundRect(2, 2, 236, 50, 5, TFT_WHITE);
  ttgo->eTFT->drawRoundRect(2, 60, 236, 100, 5, TFT_WHITE);
  ttgo->eTFT->drawRoundRect(2, 168, 236, 50, 5, TFT_WHITE);

  ttgo->eTFT->setTextDatum(ML_DATUM); 
  ttgo->eTFT->setTextColor(TFT_WHITE, Background(ScreenNumber));
  ttgo->eTFT->drawString("Freq. Error:", 20, 190, 2);

  DrawButtons();

  DrawMode();
  DrawFrequency();
}

void ModeDownClicked(void)
{
  if (--Settings.Mode < 0)
  {
      Settings.Mode = 7;
  }
  
  DrawMode();
  SendLoRaMode(Settings.Mode);
  SaveSettings();
}

void ModeUpClicked(void)
{
  if (++Settings.Mode > 7)
  {
      Settings.Mode = 0;
  }

  DrawMode();
  SendLoRaMode(Settings.Mode);
  SaveSettings();
}

void FrequencyUpClicked(void)
{
  Settings.Frequency = Settings.Frequency + 0.001;
  
  if (Settings.Frequency > 434.790)
  {
    Settings.Frequency = 434.040;
  }

  DrawFrequency();
  SendLoRaFrequency(Settings.Frequency);
  SaveSettings();
}

void FrequencyUpUpClicked(void)
{
  Settings.Frequency = Settings.Frequency + 0.025;
  
  if (Settings.Frequency > 434.790)
  {
    Settings.Frequency = 434.040;
  }

  DrawFrequency();
  SendLoRaFrequency(Settings.Frequency);
  SaveSettings();
}

void FrequencyDownClicked(void)
{
  Settings.Frequency = Settings.Frequency - 0.001;
  
  if (Settings.Frequency < 434.040)
  {
    Settings.Frequency = 434.790;
  }

  DrawFrequency();
  SendLoRaFrequency(Settings.Frequency);
  SaveSettings();
}

void FrequencyDownDownClicked(void)
{
  Settings.Frequency = Settings.Frequency - 0.025;
  
  if (Settings.Frequency < 434.040)
  {
    Settings.Frequency = 434.790;
  }

  DrawFrequency();
  SendLoRaFrequency(Settings.Frequency);
  SaveSettings();
}

const TButton Buttons[] =
{
  {  5,   5, 50, 40, "<",  &ModeDownClicked},
  {184,   5, 50, 40, ">",  &ModeUpClicked},
  { 60, 115, 50, 40, "<",  &FrequencyDownClicked},
  {130, 115, 50, 40, ">",  &FrequencyUpClicked},
  {  5, 115, 50, 40, "<<", &FrequencyDownDownClicked},
  {185, 115, 50, 40, ">>", &FrequencyUpUpClicked}
};

void DrawButtons(void)
{
  int i;

  ttgo->eTFT->setTextColor(TFT_BLACK, TFT_YELLOW);
  ttgo->eTFT->setTextDatum(MC_DATUM); 
  
  for (i=0; i<6; i++)
  {
    ttgo->eTFT->fillRoundRect(Buttons[i].x, Buttons[i].y, Buttons[i].w, Buttons[i].h, 3, TFT_YELLOW);
    ttgo->eTFT->drawString(Buttons[i].Caption, Buttons[i].x + Buttons[i].w / 2, Buttons[i].y + Buttons[i].h / 2, 4);
  }
}

void DrawMode(void)
{
  char Line[16];
  
  ttgo->eTFT->setTextDatum(MC_DATUM); 
  ttgo->eTFT->setTextColor(TFT_WHITE, Background(ScreenNumber));

  ttgo->eTFT->fillRect(60, 15, 120, 25, Background(ScreenNumber));
  sprintf(Line, "Mode %d", Settings.Mode);
  ttgo->eTFT->drawString(Line, 120, 30, 4);
}

void DrawFrequency(void)
{
  char Line[32];
  
  ttgo->eTFT->setTextDatum(TC_DATUM); 
  ttgo->eTFT->setTextColor(TFT_WHITE, Background(ScreenNumber));

  ttgo->eTFT->fillRect(7, 65, 226, 30, Background(ScreenNumber));
  sprintf(Line, "Freq. %.3f MHz", Settings.Frequency);
  ttgo->eTFT->drawString(Line, 125, 80, 4);
}

void UpdateSettingsScreen(int Always)
{
  static int LastFreqErr=9999;
  char Line[16];
  
  if (Always || (LoRa.FreqErr != LastFreqErr))
  {
    LastFreqErr = LoRa.FreqErr;
    ttgo->eTFT->fillRect(105, 180, 130, 30, Background(ScreenNumber));
    sprintf(Line, "%dHz", LoRa.FreqErr);
    ttgo->eTFT->setTextDatum(MC_DATUM); 
    ttgo->eTFT->setTextColor(TFT_WHITE, Background(ScreenNumber));
    ttgo->eTFT->drawString(Line, 165, 195, 4);
  }
}

void SettingsScreenPress(int x, int y)
{
  // Did this hit one of the buttons ?
  int i;
 
  for (i=0; i<6; i++)
  {
    if (ClickIsWithinBounds(x, y, Buttons[i].x, Buttons[i].y, Buttons[i].w, Buttons[i].h))
    {
      (*Buttons[i].Pressed)();
      return;
    }
  }
}
