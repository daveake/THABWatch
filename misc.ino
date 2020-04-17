int GPSStatus(void)
{
  if (GPS.LastPositionAt == 0)
  {
    return 0;
  }
  else if (millis() < (GPS.LastPositionAt + 5000))
  {
    return 1;
  }
  else
  {
    return 2;
  }
}

int LoRaStatus(void)
{
  if (LoRa.Position.LastPositionAt == 0)
  {
    return 0;
  }
  else if (millis() < (LoRa.Position.LastPositionAt + 45000))
  {
    return 1;
  }
  else
  {
    return 2;
  }
}

unsigned int ColourFromStatus(int Status)
{
  static unsigned int Colours[3] = {TFT_LIGHTGREY, TFT_GREEN, TFT_RED};

  if ((Status >= 0) && (Status <= 2))
  {
    return Colours[Status];
  }
  else
  {
    return TFT_BLUE;
  }
}
