//Snowflakes WiFi 
//  NextPattern()
//
//    Apr2016 Kevin Garton
//      Version 3
//        Changed calls to print statements to calls to string
//        builders, now that the print statements are instead
//        string builders.
//  
//    11Mar2016  Dean Garton 
//      version 2 
//  
//    18Feb2016  Dean Garton
//      switch to the next pattern

void NextPattern(void)
{
  uint32_t Length;
  uint8_t Index;
  uint8_t ProfileNumber;

  //print data from eeprom
  Length = sizeof(PatternRecord);
  WebPrint(GetRecord(Length));

  //reset timer
  PatternTimer = U8ToU16(PatternRecord[17], PatternRecord[18]);

  //loop to switch to next pattern
  Index = 0;
  while(Index < 16)
  {
    //get profile number
    ProfileNumber = PatternRecord[Index+1];

    //if profile change requested
    if(ProfileNumber != 0xFF)
    {
      ProfileState[Index] = State[ProfileNumber];
      ProfileIndex[Index] = ProfileNumber;
    }

    Index += 1;
  }

  //next
  PatternState = 6;
}



