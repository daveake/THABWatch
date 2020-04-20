void ShowDirectionScreen(void)
{
  ttgo->eTFT->fillCircle(119, 109, 109, TFT_YELLOW);
  ttgo->eTFT->fillCircle(119, 109, 80, Background(ScreenNumber));
  // ttgo->eTFT->drawRect(60, 95, 110, 50, TFT_YELLOW);
}

void UpdateDirectionScreen(int Always)
{
  if (LoRa.DistanceAndDirectionAreValid && (Always || LoRa.GotDistanceAndDirection))
  {
  	static int LastDirection;
    char Distance[8];
	
    if ((LastDirection != LoRa.Position.Direction) && !Always)
    {
    	DrawPointer(LastDirection, TFT_YELLOW);
    }
    
    if ((LastDirection != LoRa.Position.Direction) || Always)
    {	
    	DrawPointer(LoRa.Position.Direction, TFT_BLACK);
    	
    	ttgo->eTFT->setTextDatum(MC_DATUM);
    }
    
    LastDirection = LoRa.Position.Direction;
    
    ttgo->eTFT->drawRect(60, 105, 120, 30, Background(ScreenNumber));
    ttgo->eTFT->setTextDatum(MC_DATUM);
    ttgo->eTFT->setTextColor(TFT_WHITE, Background(ScreenNumber));
    sprintf(Distance, "%.0lf m", LoRa.Distance);
    ttgo->eTFT->drawString(Distance, 120, 120, 4);
    
    LoRa.GotDistanceAndDirection = 0;
  }
}

void DrawPointer(int Angle, unsigned int Colour)
{
  int x1, y1, x2, y2, x3, y3;

  GetPointFromAngle(Angle-8, 82, &x1, &y1);
  GetPointFromAngle(Angle, 109, &x2, &y2);
  GetPointFromAngle(Angle+8, 82, &x3, &y3);

  ttgo->eTFT->fillTriangle(x1, y1, x2, y2, x3, y3, Colour);	
}

void GetPointFromAngle(int Angle, int Length, int *x, int *y)
{
  *x = 120 + (int)(Length * cos(Angle * 0.0174532925));
  *y = 110 + (int)(Length * sin(Angle * 0.0174532925));
}
