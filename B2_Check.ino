//Snowflakes WiFi 
//  CheckFirstRecord()
//  CheckProfileRecords()
//  CheckPatternRecords()
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
    //profile record
    case 0x80:
      //increment
      Length = 6;
      PatternAddress += Length;

      //print
      PrintCheckStartOfPattern();
      PrintCheckRecord(Length);
      
      //next
      RecordNumber += 1;
      PatternState += 1;
    break;

    //fatal error
    default:
      FatalTableError();
    break;
  }
}

void CheckProfileRecords(void)
{
  uint32_t Length;
  
  //read from eeprom
  ReadPatternRecord();

  //case per record ID
  switch(PatternRecord[0])
  {
    //profile record
    case 0x80:
      //increment
      Length = 6;
      PatternAddress += Length;

      //print
      PrintCheckRecord(Length);

      //next
      RecordNumber += 1;
    break;

    //start of pattern record
    case 0x90:
      //save start address
      PatternStartTableAddress = PatternAddress;
      
      //increment
      PatternNumber += 1;
      RecordNumber = 0;
      GetPatternName();
      Length = 11;
      PatternAddress += Length;
      
      //print
      PrintCheckStartOfPattern();
      PrintCheckRecord(Length);
      
      //next
      PatternState += 1;
    break;

    //fatal error
    default:
      FatalTableError();
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
      //increment
      RecordNumber += 1;
      Length = 18;
      PatternAddress += Length;
      
      //print
      PrintCheckRecord(Length);
    break;

    //start of pattern record
    case 0x90:
      //increment
      PatternNumber += 1;
      RecordNumber = 0;
      GetPatternName();
      Length = 11;
      PatternAddress += Length;

      //print
      PrintCheckStartOfPattern();
      PrintCheckRecord(Length);
    break;

    //end of table record
    case 0x91:
      //increment
      PatternNumber += 1;
      RecordNumber = 0;
      strcpy(PatternName, "EndTable");
      Length = 1;
      PatternAddress = PatternStartTableAddress;

      //print
      PrintCheckStartOfPattern();
      PrintCheckRecord(Length);

      //next
      PatternNumber = 0;
      EndOfTable = 1;
      StartOfPattern = 1;
      PatternState += 1;
    break;

    //fatal error
    default:
      FatalTableError();
    break;
  }
}

