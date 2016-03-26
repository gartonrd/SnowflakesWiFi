//Snowflakes WiFi 
//  ScaleIntensity()
//    convert 8 bit intensity (0x00-0xFF) to 
//    16 bit intensity (0x0000-0x1000)
//  WriteTimerIntensityState()
//  WriteFixedIntensity()
//  WriteRampIntensity()
//  WriteIntensity()
//  
//    11Mar2016  Dean Garton 
//      version 2 
//  
//    18Feb2016  Dean Garton
//      intensity utilities    

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

void WriteTimerIntensityState(uint8_t Index, uint32_t Timer, uint16_t Intensity, uint8_t  State)
{
  //set timer
  ProfileTimer[Index] = Timer;

  //set intensity
  WriteIntensity(Index, Intensity);
  
  //set state
  ProfileState[Index] = State;
}

void WriteRampIntensity(uint8_t Index, uint16_t ElapsedTime, uint16_t IntensityMinimum)
{
  uint64_t ElapsedTime64;
  uint64_t IntensityMinimum64;
  uint64_t Numerator64;
  uint64_t Denominator64;
  uint64_t Intensity64;
  uint16_t Intensity8;

  //get parameters
  ElapsedTime64 = ElapsedTime;
  IntensityMinimum64 = IntensityMinimum;
  Numerator64 = Numerator[ProfileIndex[Index]];
  Denominator64 = Denominator[ProfileIndex[Index]];

  //perform calculation
  Intensity64 = ((Numerator64 * ElapsedTime64 * ElapsedTime64) / Denominator64) + IntensityMinimum64;
  Intensity8 = Intensity64;

  //write intensity to output port
  WriteIntensity(Index, Intensity8);
}

void WriteIntensity(uint8_t Index, uint16_t Intensity)
{
    uint8_t BufferIndex;

  BufferIndex = Index*4;
  if(Intensity == 0)
  {
    PWMBuffer[BufferIndex+0] = 0x00;
    PWMBuffer[BufferIndex+1] = 0x00;
    PWMBuffer[BufferIndex+2] = 0x00;
    PWMBuffer[BufferIndex+3] = 0x10;
  }
  else
  {
    if(Intensity < 0x1000)
    {
      PWMBuffer[BufferIndex+0] = 0x00;
      PWMBuffer[BufferIndex+1] = 0x00;
      PWMBuffer[BufferIndex+2] = U16ToU80(Intensity);
      PWMBuffer[BufferIndex+3] = U16ToU81(Intensity);
    }
    else
    {
      PWMBuffer[BufferIndex+0] = 0x00;
      PWMBuffer[BufferIndex+1] = 0x10;
      PWMBuffer[BufferIndex+2] = 0x00;
      PWMBuffer[BufferIndex+3] = 0x00;
    }
  }
}