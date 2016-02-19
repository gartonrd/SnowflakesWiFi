//Snowflakes WiFi 
//  NextPattern()
//  
//    18Feb2016  Dean Garton
//      switch to the next pattern

void NextPattern(void)
{
  uint32_t Length;
  uint8_t Index;
  uint8_t ProfileNumber;

  //print data from eeprom
  if(EndOfTable == 1)
  {
    PrintEndOfTable();
  }
  if(StartOfPattern == 1)
  {
    PrintStartOfPattern();
  }
  Length = sizeof(PatternRecord);
  PrintRecord(Length);

  //reset timer
  PatternTimer = ScaleTime(PatternRecord[17]);

  //loop to overwrite state if no change requested
  Index = 0;
  while(Index < 16)
  {
    //get profile number
    ProfileNumber = PatternRecord[Index+1];

    //if no change requested
    if(ProfileNumber == 0xFF)
    {
      //if start of pattern found
      if(StartOfPattern == 1)
      {
        //set state to fixed intensity
        ProfileData[ProfileNextIndex].State[Index] = 0x38;
      
        //set intensity to off
        ProfileData[ProfileNextIndex].StartIntensity[Index] = 0x0000;
      }
      else
      {
        //copy current state to next state
        ProfileData[ProfileNextIndex].State[Index] = ProfileData[ProfileIndex].State[Index];
      }
    }

    //next
    Index += 1;
  }

  //switch to next pattern
  if(ProfileIndex == 0)
  {
    ProfileIndex = 1;
    ProfileNextIndex = 0;
  }
  else
  {
    ProfileIndex = 0;
    ProfileNextIndex = 1;
  }

  //next
  StartOfPattern = 0;
  EndOfTable = 0;
  PatternState = 4;
}



