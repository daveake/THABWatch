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
  ttgo->eTFT->fillScreen(TFT_BLACK);
  ttgo->eTFT->setTextColor(TFT_WHITE, TFT_BLACK);
  ttgo->eTFT->setTextFont(4);
  ttgo->eTFT->drawString("Settings",  32, 0, 4);

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
  { 60,  40, 50, 40, "<",  &ModeDownClicked},
  {130,  40, 50, 40, ">",  &ModeUpClicked},
  { 60, 199, 50, 40, "<",  &FrequencyDownClicked},
  {130, 199, 50, 40, ">",  &FrequencyUpClicked},
  {  0, 199, 50, 40, "<<", &FrequencyDownDownClicked},
  {190, 199, 50, 40, ">>", &FrequencyUpUpClicked}
};

void DrawButtons(void)
{
  int i;

  ttgo->eTFT->setTextColor(TFT_BLACK, TFT_YELLOW);
  ttgo->eTFT->setTextDatum(MC_DATUM); 
  
  for (i=0; i<6; i++)
  {
    ttgo->eTFT->fillRect(Buttons[i].x, Buttons[i].y, Buttons[i].w, Buttons[i].h, TFT_YELLOW);
    ttgo->eTFT->drawString(Buttons[i].Caption, Buttons[i].x + Buttons[i].w / 2, Buttons[i].y + Buttons[i].h / 2, 4);
  }
}

void DrawMode(void)
{
  char Line[16];
  
  ttgo->eTFT->setTextDatum(MC_DATUM); 
  ttgo->eTFT->setTextColor(TFT_WHITE, TFT_BLACK);

  ttgo->eTFT->fillRect(0, 90, 240, 50, TFT_BLACK);
  sprintf(Line, "Mode %d", Settings.Mode);
  ttgo->eTFT->drawString(Line, 120, 115, 4);
  
  ttgo->eTFT->setTextDatum(TL_DATUM); 
}

void DrawFrequency(void)
{
  char Line[32];
  
  ttgo->eTFT->setTextDatum(MC_DATUM); 
  ttgo->eTFT->setTextColor(TFT_WHITE, TFT_BLACK);

  ttgo->eTFT->fillRect(0, 140, 240, 50, TFT_BLACK);
  sprintf(Line, "Freq. %.3f MHz", Settings.Frequency);
  ttgo->eTFT->drawString(Line, 120, 165, 4);
  
  ttgo->eTFT->setTextDatum(TL_DATUM); 
}

void UpdateSettingsScreen(int Always)
{
}

void SettingsScreenPress(int x, int y)
{
  // Did this hit one of the buttons ?
  int i;
 
  for (i=0; i<6; i++)
  {
    if (ClickIsWithinBounds(x, y, Buttons[i].x, Buttons[i].y, Buttons[i].w, Buttons[i].h))
    {
      HostPort.printf("*** BUTTON %d PRESSED ***\n", i);
      (*Buttons[i].Pressed)();
      return;
    }
  }

  HostPort.println("*** NO BUTTON PRESSED ***");
}
