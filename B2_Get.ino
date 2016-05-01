/********************************************************
Snowflakes WiFi 
  B2_Get
    get cases for the pattern state machine
********************************************************/

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
  
        ProfileSize = PROFILE_SIZE
        if(ProfileNumber >= ProfileSize)
        {
          ProfileNumberError(ProfileNumber);
        }
        else
        {
          SaveProfile(ProfileNumber);
        }
  
        ProfileCount += 1;
        if(ProfileCount >= 12)
        {
          EndLoop = 1;
        }
      break;

      //pattern record
      case 0x81:

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

    ReadPatternRecord();
    
    //case per record ID
    switch(PatternRecord[0])
    {
      //start of pattern record
      case 0x90:

        if(PatternReps > 0)
        {
          PatternReps -= 1;
        }

        if(PatternReps == 0)
        {
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
          RecordNumber = 0;

          //EEPROM address
          PatternAddress = PatternStartAddress;
        }

        Serial.print(GetHeading());
      break;

      //end of table record
      case 0x91:

        if(PatternReps > 0)
        {
          PatternReps -= 1;
        }


        if(PatternReps == 0)
        {
          //wrap to start of table
          RecordNumber = 0;
          
          //EEPROM address
          PatternAddress = StartTableAddress;

          Serial.print(GetBreakLine());
        }
        else
        {
          //reset for another rep of the same pattern
          RecordNumber = 0;

          //EEPROM address
          PatternAddress = PatternStartAddress;

          Serial.print(GetHeading());
        }
      break;

      //pattern record
      case 0x81:
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
