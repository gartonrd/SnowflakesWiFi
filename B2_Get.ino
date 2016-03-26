//Snowflakes WiFi 
//  GetProfileRecords()
//  GetPatternRecord()
//  
//    11Mar2016  Dean Garton 
//      version 2 
//  
//    18Feb2016  Dean Garton
//      get records from the table

void GetProfileRecords(void)
{
  uint8_t EndLoop;
  uint8_t ProfileCount;
  uint32_t Length;
  uint8_t ProfileNumber;
  uint8_t ProfileSize;
  
  //loop to get records
  ProfileCount = 0;
  EndLoop = 0;
  while(EndLoop == 0)
  {
    //read from eeprom 
    ReadPatternRecord();
    
    //case per record ID
    switch(PatternRecord[0])
    {
       //profile record
      case 0x80:
        //EEPROM address
        Length = PROFILE_LENGTH;
        PatternAddress += Length;
  
        //get profile number
        ProfileNumber = PatternRecord[1];
  
        //if profile number is not in range
        ProfileSize = PROFILE_SIZE
        if(ProfileNumber >= ProfileSize)
        {
          //error
          ProfileNumberError(ProfileNumber);
        }
        else
        {
          //save profile
          SaveProfile(ProfileNumber);
        }
  
        //next
        ProfileCount += 1;
        if(ProfileCount >= 12)
        {
          EndLoop = 1;
        }
      break;

      //pattern record
      case 0x81:
        //bookkeeping
        RecordNumber += 1;

        //EEPROM address
        PatternStartAddress = PatternAddress;
        Length = PATTERN_LENGTH;
        PatternAddress += Length;
  
        //next
        NextPattern();
        EndLoop = 1;
      break;

      //fatal error
      default:
        RecordIDError();
      break;
    }
  }
}

void GetPatternRecord(void)
{
  uint8_t EndLoop;
  uint32_t Length;
  
  //loop to get record
  EndLoop = 0;
  while(EndLoop == 0)
  {
    //read from eeprom 
    ReadPatternRecord();
    
    //case per record ID
    switch(PatternRecord[0])
    {
      //start of pattern record
      case 0x90:
        //decrement reps
        if(PatternReps > 0)
        {
          PatternReps -= 1;
        }

        //if reps = 0
        if(PatternReps == 0)
        {
          //bookkeeping
          GetPatternName();
          RecordNumber = 0;
          PatternReps = PatternRecord[1];
          
          //EEPROM address
          Length = START_OF_PATTERN_LENGTH;
          PatternAddress += Length;
          
          //next
          PatternState = 4;
          EndLoop = 1;
        }
        else
        {
          //reset for another rep of the same pattern
          //bookkeeping
          RecordNumber = 0;

          //EEPROM address
          PatternAddress = PatternStartAddress;
        }

        //print heading
        PrintHeading();
      break;

      //end of table record
      case 0x91:
        //decrement reps
        if(PatternReps > 0)
        {
          PatternReps -= 1;
        }

        //if reps = 0
        if(PatternReps == 0)
        {
          //wrap to start of table
          //bookkeeping
          RecordNumber = 0;
          
          //EEPROM address
          PatternAddress = 0;

          //print break line
          PrintBreakLine();
        }
        else
        {
          //reset for another rep of the same pattern
          //bookkeeping
          RecordNumber = 0;

          //EEPROM address
          PatternAddress = PatternStartAddress;

          //print heading
          PrintHeading();
        }
      break;

      //pattern record
      case 0x81:
        //bookkeeping
        RecordNumber += 1;

        //EEPROM address
        Length = PATTERN_LENGTH;
        PatternAddress += Length;

        //next
        NextPattern();
        EndLoop = 1;
      break;

      //fatal error
      default:
        RecordIDError();
      break;
    }
  }
}
