
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
    
    uint16_t color = random(0xFFFF);
    ttgo->eTFT->fillScreen(color);
    ttgo->eTFT->setTextColor(color, TFT_WHITE);
    ttgo->eTFT->drawString("User Button pressed",  5, 100, 4);
}
void released()
{
    HostPort.println("Button Released");
    
    uint16_t color = random(0xFFFF);
    ttgo->eTFT->fillScreen(color);
    ttgo->eTFT->setTextColor(color, TFT_WHITE);
    ttgo->eTFT->drawString("User Button released",  5, 100, 4);
}
