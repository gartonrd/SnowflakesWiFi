/********************************************************
Snowflakes WiFi 
  B4_Next
    switch to the next pattern
********************************************************/

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



