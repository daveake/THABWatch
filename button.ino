unsigned long ButtonPressedAt = 0;

void SetupButton(void)
{
  ttgo->button->setPressedHandler(pressed);
  ttgo->button->setReleasedHandler(released);
}

void CheckButton(void)
{
  ttgo->button->loop();
}


void pressed()
{
  HostPort.println("Button Pressed");

  ButtonPressedAt = millis();
}

void released()
{
  if (ButtonPressedAt > 0)
  {
    HostPort.println("Button Released");
    
    if (millis() > (ButtonPressedAt + 500))
    {
      LongButtonPress();
    }
    else
    {
      ShortButtonPress();
    }
  }
}
