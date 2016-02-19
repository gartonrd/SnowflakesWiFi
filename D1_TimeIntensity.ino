//Snowflakes WiFi 
//  
//  ScaleTime()
//    multiply 8 bit time by scale factor
//  ScaleIntensity()
//    convert 8 bit intensity (0x00-0xFF) to 
//    16 bit intensity (0x0000-0x1000)
//  WriteIntensity()
//  WriteRampIntensity()
//  
//    18Feb2016  Dean Garton
//      time and intensity utilities    

uint16_t ScaleTime(uint8_t UnscaledTime)
{
  uint16_t UnScaledTime16;
  uint16_t ScaleFactor16;
  uint16_t ScaledTime;

  UnScaledTime16 = UnscaledTime;
  ScaleFactor16 = ScaleFactor;
  ScaledTime = (UnScaledTime16 * ScaleFactor16)/10;
  if((UnscaledTime > 0) && (ScaledTime == 0))
  {
    ScaledTime = 1;
  }

  return ScaledTime;
}

uint16_t ScaleIntensity(uint8_t Intensity)
{
uint16_t Intensity16;

if(Intensity >= 0xFF)
{
  Intensity16 = 0x1000;
}
else
{
  if(Intensity <= 16)
  {
    Intensity16 = Intensity;
  }
  else
  {
    Intensity16 = (Intensity * Intensity) / 16;
  }
}

return Intensity16;
}

void WriteIntensity(uint8_t Index, uint16_t Intensity)
{
    uint16_t TimeOn;
    uint16_t TimeOff;

  if(Intensity == 0)
  {
    TimeOn = 0;
    TimeOff = 0x1000;
  }
  else
  {
    if(Intensity < 0x1000)
    {
      TimeOn = 0;
      TimeOff = Intensity;
    }
    else
    {
      TimeOn = 0x1000;
      TimeOff = 0;
    }
  }

  //write intensity to output port
  WritePWMChannel(Index, TimeOn, TimeOff); 
}

void WriteRampIntensity(uint8_t Index, uint16_t ElapsedTime, uint16_t IntensityMinimum)
{
  uint64_t ElapsedTime64;
  uint64_t IntensityMinimum64;
  uint64_t Numerator;
  uint64_t Denominator;
  uint64_t Intensity64;
  uint16_t Intensity;

  //get parameters
  ElapsedTime64 = ElapsedTime;
  IntensityMinimum64 = IntensityMinimum;
  Numerator = ProfileData[ProfileIndex].Numerator[Index];
  Denominator = ProfileData[ProfileIndex].Denominator[Index];

  //perform calculation
  Intensity64 = ((Numerator * ElapsedTime64 * ElapsedTime64) / Denominator) + IntensityMinimum64;
  Intensity = Intensity64;

  //write intensity to output port
  WriteIntensity(Index, Intensity);
}
