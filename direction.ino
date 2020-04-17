void ShowDirectionScreen(void)
{
  ttgo->eTFT->fillScreen(TFT_BLACK);
  ttgo->eTFT->setTextColor(TFT_WHITE, TFT_BLACK);
  ttgo->eTFT->setTextFont(4);
  ttgo->eTFT->drawCircle(119, 119, 119, TFT_YELLOW);
  ttgo->eTFT->drawRect(60, 95, 120, 50, TFT_YELLOW);
}

void UpdateDirectionScreen(int Always)
{
  
  if (LoRa.DistanceAndDirectionAreValid && (Always || LoRa.GotDistanceAndDirection))
  {
    char Distance[8];

    sprintf(Distance, "%.0lf m", LoRa.Distance);
    ttgo->eTFT->drawString(Distance, 90, 110, 4);
    plotNeedle((int)LoRa.Position.Direction - GPS.Direction);
    LoRa.GotDistanceAndDirection = 0;
  }
}

uint16_t osx = 120, osy = 120; // Saved x & y coords

void plotNeedle(int value)
{
  // Calculate tip of needle coords
  float sx = 105 * cos(value * 0.0174532925);
  float sy = 105 * sin(value * 0.0174532925);
  
  // Erase old needle image
//  ttgo->eTFT->drawLine(120, 120, osx, osy + 120, TFT_BLACK);
  ttgo->eTFT->drawCircle(osx, osy, 10, TFT_BLACK);
  
  osx = sx + 120;
  osy = sy + 120;

  // Draw the needle in the new position
//  ttgo->eTFT->drawLine(120, 120, osx, osy + 120, TFT_YELLOW);
  ttgo->eTFT->drawCircle(osx, osy, 10, TFT_YELLOW);
}
