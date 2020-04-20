TP_Point Pressed, LastPosition;

void SetupTouch(void)
{
  Pressed.x = -1;
}

void CheckTouch()
{
  if (ttgo->touch->touched())
  {
    TP_Point p =  ttgo->touch->getPoint();

    if ((p.x > 0) || (p.y > 0))
    {
      if (Pressed.x < 0)
      {
        Pressed = p;
      }

      LastPosition = p;
    }
  }
  else if (Pressed.x >= 0)
  {
    int XDistance, YDistance;

    XDistance = LastPosition.x - Pressed.x;
    YDistance = LastPosition.y - Pressed.y;

    if ((abs(XDistance) + abs(YDistance)) < 20)
    {
      // Process as a press
      ScreenPress((Pressed.x * 3) / 4, (Pressed.y * 3) / 4);      // scale for pixel display size
    }
    else if ((XDistance > 50) || (YDistance > 50))
    {
      // Swipe up or right
      PreviousScreen();
    }
    else if ((XDistance < -50) || (YDistance < -50))
    {
      // Swipe up or right
      NextScreen();
    }

    Pressed.x = -1;
  }
}
