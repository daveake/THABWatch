void CheckPower(void)
{
  static unsigned long CheckPower=0;

  if (millis() > CheckPower)
  {
    SystemInfo.BatteryVoltage = (unsigned int)(ttgo->power->getBattVoltage());
    HostPort.printf("Battery = %u mV\n", SystemInfo.BatteryVoltage);
    CheckPower = millis() + 60000;
  }
}
