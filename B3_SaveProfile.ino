/********************************************************
Snowflakes WiFi 
  B3_SaveProfile
    save profile record in RAM
********************************************************/
  
void SaveProfile(uint8_t ProfileNumber8)
{
  uint8_t State8;
  uint8_t StartIntensity8;
  uint16_t StartIntensity16;
  uint8_t EndIntensity8;
  uint16_t EndIntensity16;
  uint16_t RampTime16;
  uint32_t RampTime32;
  uint16_t BlinkDelayTime16;
  uint16_t BlinkOnTime16;
  uint16_t BlinkOffTime16;

  //initialize data
  StartIntensity8 = PatternRecord[2];
  StartIntensity16 = ScaleIntensity(StartIntensity8);
  StartIntensity[ProfileNumber8] = StartIntensity16;

  EndIntensity8 = PatternRecord[3];
  EndIntensity16 = ScaleIntensity(EndIntensity8);
  EndIntensity[ProfileNumber8] = EndIntensity16;
  
  RampTime16 = U8ToU16(PatternRecord[4], PatternRecord[5]);
  RampTime[ProfileNumber8] = RampTime16;
  RampTime32 = RampTime16;
  Denominator[ProfileNumber8] = (RampTime32) * (RampTime32);

  BlinkDelayTime16 = U8ToU16(PatternRecord[6], PatternRecord[7]);
  BlinkDelayTime[ProfileNumber8] = BlinkDelayTime16;  

  BlinkOnTime16 = U8ToU16(PatternRecord[8], PatternRecord[9]);
  BlinkOnTime[ProfileNumber8] = BlinkOnTime16;

  BlinkOffTime16 = U8ToU16(PatternRecord[10], PatternRecord[11]);
  BlinkOffTime[ProfileNumber8] = BlinkOffTime16;

  //initialize state
  State8 = 0x00;
  if(BlinkOffTime16 != 0)
  {
    State8 |= 0x10;
  }
  if(BlinkOnTime16 != 0)
  {
    State8 |= 0x20;
    if(BlinkDelayTime16 != 0)
    {
      State8 |= 0x80;
    }
  }

  if(RampTime16 != 0)
  {
    if(StartIntensity8 < EndIntensity8)
    {
      State8 = 0x40;
      Numerator[ProfileNumber8] = EndIntensity16 - StartIntensity16;
    }
    else
    {
      State8 = 0x50;
      Numerator[ProfileNumber8] = StartIntensity16 - EndIntensity16;
    }
  }
  
  State[ProfileNumber8] = State8;
}

