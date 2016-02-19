//Snowflakes WiFi 
//  GetProfileRecords()
//  
//    18Feb2016  Dean Garton
//      get profile records
//      scale data and save

void GetProfileRecords(uint8_t StartIndex, uint8_t EndIndex)
{
  uint8_t Index;
  uint8_t ProfileNumber;
  uint8_t State;
  uint8_t StartIntensity;
  uint16_t StartIntensity16;
  uint8_t EndIntensity;
  uint16_t EndIntensity16;
  uint8_t RampTime;
  uint16_t RampTime16;
  uint32_t RampTime32;
  uint8_t BlinkOnTime;
  uint8_t BlinkOffTime;

  //loop thru all profiles
  Index = StartIndex;
  while(Index <= EndIndex)
  {
    //get profile number
    ProfileNumber = PatternRecord[Index+1];

    //if new profile requested
    if(ProfileNumber != 0xFF)
    {      
      //read from eeprom
      ReadProfileRecord(ProfileNumber);

      //initialize state and data
      StartIntensity = ProfileRecord[1];
      StartIntensity16 = ScaleIntensity(StartIntensity);
      ProfileData[ProfileNextIndex].StartIntensity[Index] = StartIntensity16;
      RampTime = ProfileRecord[3];
      if(RampTime == 0)
      {
        State = 0x08;
        BlinkOffTime = ProfileRecord[5];
        ProfileData[ProfileNextIndex].BlinkOffTime[Index] = ScaleTime(BlinkOffTime);
        if(BlinkOffTime == 0)
        {
          State |= 0x10;
        }
        BlinkOnTime = ProfileRecord[4];
        ProfileData[ProfileNextIndex].BlinkOnTime[Index] = ScaleTime(BlinkOnTime);
        if(BlinkOnTime == 0)
        {
          State |= 0x20;
        }
      }
      else
      {
        RampTime16 = ScaleTime(RampTime);
        ProfileData[ProfileNextIndex].RampTime[Index] = RampTime16;
        RampTime32 = RampTime16;
        ProfileData[ProfileNextIndex].Denominator[Index] = (RampTime32) * (RampTime32);
        EndIntensity = ProfileRecord[2];
        EndIntensity16 = ScaleIntensity(EndIntensity);
        ProfileData[ProfileNextIndex].EndIntensity[Index] = EndIntensity16;
        if(StartIntensity < EndIntensity)
        {
          State = 0x10;
          ProfileData[ProfileNextIndex].Numerator[Index] = EndIntensity16 - StartIntensity16;
        }
        else
        {
          State = 0x20;
          ProfileData[ProfileNextIndex].Numerator[Index] = StartIntensity16 - EndIntensity16;
        }
      }
      ProfileData[ProfileNextIndex].State[Index] = State;
    }
    
    //next
    Index += 1;
  }

  //next
  PatternState += 1;
}

