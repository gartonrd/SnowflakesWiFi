/********************************************************
Snowflakes WiFi 
  C1_Intensity
    intensity utilities
********************************************************/

uint16_t ScaleIntensity(uint8_t Intensity)
{
uint16_t Intensity16;
  if(Intensity == 0)
  {
    Intensity16 = 0x0000;
  }
  else
  {
    if(Intensity >= 0xFF)
    {
      Intensity16 = 0x1000;
    }
    else
    {
      if(tolower(AcBoard[0]) == 'y')
      {
        Intensity16 = ((0xD9 * Intensity)/0x10) + 0x0245;    
      }
      else
      {
        Intensity16 = (0x0A * Intensity) + 0;     
      }
    }
  }
  return Intensity16;
}

void WriteTimerIntensityState(uint8_t Index, uint32_t Timer, uint16_t Intensity, uint8_t  State)
{
  ProfileTimer[Index] = Timer;
  WriteIntensity(Index, Intensity);
  ProfileState[Index] = State;
}

void WriteRampIntensity(uint8_t Index, uint16_t ElapsedTime, uint16_t IntensityMinimum)
{
  uint64_t ElapsedTime64;
  uint64_t IntensityMinimum64;
  uint64_t Numerator64;
  uint64_t Denominator64;
  uint64_t Intensity64;
  uint16_t Intensity16;

  ElapsedTime64 = ElapsedTime;
  IntensityMinimum64 = IntensityMinimum;
  Numerator64 = Numerator[ProfileIndex[Index]];
  Denominator64 = Denominator[ProfileIndex[Index]];

  Intensity64 = ((Numerator64 * ElapsedTime64 * ElapsedTime64) / Denominator64) + IntensityMinimum64;
  Intensity16 = Intensity64;

  WriteIntensity(Index, Intensity16);
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
