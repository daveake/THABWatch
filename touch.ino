int LastX=-1;
int LastY=-1;

void CheckTouch()
{
  if (ttgo->touch->touched())
  {
    TP_Point p =  ttgo->touch->getPoint();

    if (LastX <= 0)
    {
      HostPort.printf("*** PRESS x:%03d  y:%03d ***\n", p.x, p.y);
      ScreenPress((p.x * 3) / 4, (p.y * 3) / 4);      // scale for pixel display size
    }
  
    if ((p.x != LastX) || (p.y != LastY))
    {
      // HostPort.printf("*** x:%03d  y:%03d ***\n", p.x, p.y);
      LastX = p.x;
      LastY = p.y;
    }
  }
  else
  {
    LastX = -1;
  }
}
