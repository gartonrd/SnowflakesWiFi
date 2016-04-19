//Snowflakes WiFi 
//  CheckFirstRecord()
//  CheckProfileRecords()
//  CheckPatternRecords()
//
//    11Mar2016  Dean Garton 
//      version 2 
//  
//    18Feb2016  Dean Garton 
//      Check records in the table

void CheckFirstRecord(void)
{

  uint32_t Length;

  //read from eeprom
  ReadPatternRecord();

  //case per record ID
  switch(PatternRecord[0])
  {
    //start of pattern record
    case 0x90:
      //print break line
      Serial.print(GetBreakLine());
        
      //next
      PatternState = 3;
    break;

    //fatal error
    default:
      RecordIDError();
    break;
  }
}

void CheckProfileRecords(void)
{
  uint32_t Length;
  uint8_t ProfileNumber;
  uint8_t ProfileSize;
  
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

      //print
      Serial.print(GetCheckRecord(Length));

      //check profile number
      ProfileSize = PROFILE_SIZE
      if(ProfileNumber >= ProfileSize)
      {
        ProfileNumberError(ProfileNumber);
      }

      //next
    break;

    //pattern record
    case 0x81:
      //print blank line
      Serial.print("\n");
      
      //next
      PatternState = 3;
    break;

    //fatal error
    default:
      RecordIDError();
    break;
  }
}

void CheckPatternRecords(void)
{
  uint32_t Length;
  
  //read from eeprom
  ReadPatternRecord();

  //case per record ID
  switch(PatternRecord[0])
  {
    //pattern record
    case 0x81:
      //bookkeeping
      RecordNumber += 1;

      //EEPROM address
      Length = PATTERN_LENGTH;
      PatternAddress += Length;
      
      //print
      Serial.print(GetCheckRecord(Length));
    break;

    //start of pattern record
    case 0x90:
      //bookkeeping
      GetPatternName();
      RecordNumber = 0;

      //EEPROM address
      Length = START_OF_PATTERN_LENGTH;
      PatternAddress += Length;

      //print
      Serial.print(GetCheckHeading());
      Serial.print(GetCheckRecord(Length));
      Serial.print('\n');

      //next
      PatternState = 2;
    break;

    //end of table record
    case 0x91:
    //bookkeeping
      strcpy(PatternName, "EndTable");
      RecordNumber = 0;

      //EEPROM address
      Length = END_OF_TABLE_LENGTH;
      PatternAddress = 0;

      //print
      Serial.print(GetCheckHeading());
      Serial.print(GetCheckRecord(Length));

      //next
      Serial.print(GetBreakLine());
      PatternState = 5;
    break;

    //fatal error
    default:
      RecordIDError();
    break;
  }
}

