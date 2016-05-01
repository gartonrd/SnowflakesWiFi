/********************************************************
Snowflakes WiFi 
  B1_Check
    check cases for the pattern state machine
********************************************************/

void CheckFirstRecord(void)
{
  uint32_t Length;

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

    //end of table record
    case 0x91:
    //bookkeeping
      strcpy(PatternName, "EndTable");
      RecordNumber = 0;

      //EEPROM address
      Length = END_OF_TABLE_LENGTH;
      PatternAddress = StartTableAddress;

      //print
      Serial.print(GetCheckHeading());
      Serial.print(GetCheckRecord(Length));

      //next
      Serial.print(GetBreakLine());
      PatternState = 7;
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

      Serial.print(GetCheckRecord(Length));

      ProfileSize = PROFILE_SIZE
      if(ProfileNumber >= ProfileSize)
      {
        ProfileNumberError(ProfileNumber);
      }

    break;

    //pattern record
    case 0x81:
      Serial.print('\n');
      
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
  
  ReadPatternRecord();

  //case per record ID
  switch(PatternRecord[0])
  {
    //pattern record
    case 0x81:

      RecordNumber += 1;

      //EEPROM address
      Length = PATTERN_LENGTH;
      PatternAddress += Length;
      
      Serial.print(GetCheckRecord(Length));
    break;

    //start of pattern record
    case 0x90:

      GetPatternName();
      RecordNumber = 0;

      //EEPROM address
      Length = START_OF_PATTERN_LENGTH;
      PatternAddress += Length;

      Serial.print(GetCheckHeading());
      Serial.print(GetCheckRecord(Length));
      Serial.print('\n');

      //next
      PatternState = 2;
    break;

    //end of table record
    case 0x91:

      strcpy(PatternName, "EndTable");
      RecordNumber = 0;

      //EEPROM address
      Length = END_OF_TABLE_LENGTH;
      PatternAddress = StartTableAddress;

      Serial.print(GetCheckHeading());
      Serial.print(GetCheckRecord(Length));

      Serial.print(GetBreakLine());
      //next
      PatternState = 5;
    break;

    //fatal error
    default:
      RecordIDError();
    break;
  }
}

