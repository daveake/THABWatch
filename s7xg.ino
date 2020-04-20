const double Pi = 3.1415926;

void SetupS7xg(void)
{
  HostPort.print("Powering up S7xG Board ...");
  
  Wire1.begin(SOC_GPIO_PIN_TWATCH_SEN_SDA , SOC_GPIO_PIN_TWATCH_SEN_SCL);
  axp.begin(Wire1, AXP202_SLAVE_ADDRESS);
  axp.setLDO3Mode(1);
  axp.setPowerOutPut(AXP202_LDO3, AXP202_ON); // S76G (MCU + LoRa)
  axp.setLDO4Voltage(AXP202_LDO4_1800MV);
  axp.setPowerOutPut(AXP202_LDO4, AXP202_ON); // S76G (Sony GNSS)
  
  HostPort.print("\nConnecting to S7xG Booard ...");
  s7xgPort.begin(115200, SERIAL_8E1, SOC_GPIO_PIN_TWATCH_RX, SOC_GPIO_PIN_TWATCH_TX);
}

void ProcessLine(char *Line, int Length)
{
  char *Equals, *Parameters;

  HostPort.print(Line);
  
  Equals = strchr(Line, '=');

  if (Equals != NULL)
  {
    Parameters = Equals + 1;
    *Equals = '\0';
    
    if (strcmp(Line, "CurrentRSSI") == 0)
    {
      LoRa.CurrentRSSI = atoi(Parameters);

      HostPort.printf("Decoded CurrentRSSI=%d\n", LoRa.CurrentRSSI);
    }
    else if (strcmp(Line, "GPS") == 0)
    {
      // GPS=17:53:03,51.95022,-2.54436,151,0,117,9

      if (sscanf(Parameters, "%d:%d:%d,%lf,%lf,%ld,%d,%d,%d", &GPS.Hours, &GPS.Minutes, &GPS.Seconds, &GPS.Latitude, &GPS.Longitude, &GPS.Altitude, &GPS.Speed, &GPS.Direction, &GPS.Satellites) >= 6)
      {
        HostPort.printf("Decoded GPS=%02d:%02d:%02d,%.5f,%.5f,%ld,%d,%d,%d\r\n", GPS.Hours, GPS.Minutes, GPS.Seconds,
                                                                                 GPS.Latitude, GPS.Longitude, GPS.Altitude,
                                                                                 GPS.Speed, GPS.Direction, GPS.Satellites);
        GPS.LastPositionAt = millis();
        CalculateDistanceAndDirection();
      }
      else
      {
        HostPort.print("*** ");
        HostPort.print(Line);
        HostPort.print(": ");
        HostPort.print(Parameters);
      }
    }
    else if (strcmp(Line, "PacketRSSI") == 0)
    {
      LoRa.PacketRSSI = atoi(Parameters);

      HostPort.printf("Decoded PacketRSSI=%d\n", LoRa.PacketRSSI);
    }
    else if (strcmp(Line, "PacketSNR") == 0)
    {
      LoRa.PacketSNR = atoi(Parameters);

      HostPort.printf("Decoded PacketSNR=%d\n", LoRa.PacketSNR);
    }
    else if (strcmp(Line, "FreqErr") == 0)
    {
      LoRa.FreqErr = (int)(atof(Parameters) * 1000);

      HostPort.printf("Decoded FreqErr=%d\n", LoRa.FreqErr);
    }
    else if (strcmp(Line, "Message") == 0)
    {
      if (sscanf(Parameters+2, "%31[^,],%u,%d:%d:%d,%lf,%lf,%ld", LoRa.Payload,
                                                                  &LoRa.Counter,
                                                                  &LoRa.Position.Hours,
                                                                  &LoRa.Position.Minutes,
                                                                  &LoRa.Position.Seconds,
                                                                  &LoRa.Position.Latitude,
                                                                  &LoRa.Position.Longitude,
                                                                  &LoRa.Position.Altitude) >= 8)
      {
        HostPort.printf("Decoded Message=%s,%lu,%02d:%02d:%02d,%.5f,%.5f,%05ld\r\n", 
                                                                 LoRa.Payload,
                                                                 LoRa.Counter,
                                                                 LoRa.Position.Hours,
                                                                 LoRa.Position.Minutes,
                                                                 LoRa.Position.Seconds,
                                                                 LoRa.Position.Latitude,
                                                                 LoRa.Position.Longitude,
                                                                 LoRa.Position.Altitude);
        LoRa.Position.LastPositionAt = millis();
        CalculateDistanceAndDirection();
      }
      else
      {
        HostPort.print("*** ");
        HostPort.print(Line);
        HostPort.print(": ");
        HostPort.print(Parameters);
      }
    }
    else
    {
      HostPort.print("--- ");
      HostPort.print(Line);
      HostPort.print("=");
      HostPort.print(Parameters);
    }
  }
  else if (Length > 1)
  {
    HostPort.print(Line);
  }
}


void CheckS7xg(void)
{
  static char Line[300];
  static int Length=0;
  char Character;

  // Host to S7Xg - remove later, but handy for testing
//  while (HostPort.available())
//  {
//    Character = HostPort.read();
//    
//    if (Character == '1')
//    {
//      NextScreen();
//    }
//    else if (Character == '2')
//    {
//      PreviousScreen();
//    }
//    
//    s7xgPort.write(Character);
//  }

  // Receive from S7XG serial port
  while (s7xgPort.available())
  {
    Character = s7xgPort.read();    
    
    if (Length >= 299)
    {
      Length = 0;
    }
    else if (Character != '\r')
    {
      Line[Length++] = Character;
      
      if (Character == '\n')
      {
        Line[Length] = '\0';

        ProcessLine(Line, Length);

        Length = 0;
      }
    }
  }
}

void CalculateDistanceAndDirection(void)
{
  if ((GPS.Latitude != 0) || (GPS.Longitude != 0))
  {
    if ((LoRa.Position.Latitude != 0) || (LoRa.Position.Longitude != 0))
    {
      LoRa.Distance = CalculateDistance(LoRa.Position.Latitude, LoRa.Position.Longitude, GPS.Latitude, GPS.Longitude);
      LoRa.Position.Direction = (int)CalculateDirection(LoRa.Position.Latitude, LoRa.Position.Longitude, GPS.Latitude, GPS.Longitude);

      HostPort.printf("Distance=%.0lf, Direction=%d\n", LoRa.Distance, LoRa.Position.Direction);

      LoRa.GotDistanceAndDirection = 1;
      LoRa.DistanceAndDirectionAreValid = 1;
    }
  }
}

double CalculateDistance(double flat1, double flon1, double flat2, double flon2)
{
  double dist_calc=0;
  double dist_calc2=0;
  double diflat=0;
  double diflon=0;

  //I've to spplit all the calculation in several steps. If i try to do it in a single line the arduino will explode.
  diflat=radians(flat2-flat1);
  flat1=radians(flat1);
  flat2=radians(flat2);
  diflon=radians((flon2)-(flon1));

  dist_calc = (sin(diflat/2.0)*sin(diflat/2.0));
  dist_calc2= cos(flat1);
  dist_calc2*=cos(flat2);
  dist_calc2*=sin(diflon/2.0);
  dist_calc2*=sin(diflon/2.0);
  dist_calc +=dist_calc2;

  dist_calc=(2*atan2(sqrt(dist_calc),sqrt(1.0-dist_calc)));

  dist_calc*=6371000.0; //Converting to meters
  //Serial.println(dist_calc);
  return dist_calc;  
}

double CalculateDirection(double HABLatitude, double HABLongitude, double CarLatitude, double CarLongitude)
{
    double x, y;

    HABLatitude = HABLatitude * Pi / 180;
    HABLongitude = HABLongitude * Pi / 180;
    CarLatitude = CarLatitude * Pi / 180;
    CarLongitude = CarLongitude * Pi / 180;

    y = sin(HABLongitude - CarLongitude) * cos(HABLatitude);
    x = cos(CarLatitude) * sin(HABLatitude) - sin(CarLatitude) * cos(HABLatitude) * cos(HABLongitude - CarLongitude);

    return atan2(y, x) * 180 / Pi;
}

void SendLoRaMode(int Mode)
{
  HostPort.printf("Set Mode %d\r\n", Mode);
  s7xgPort.printf("~M%d\r\n", Mode);
}

void SendLoRaFrequency(float Frequency)
{
  HostPort.printf("Set Frequency %.4f\r\n", Frequency);
  s7xgPort.printf("~F%.4f\r", Frequency);
}
