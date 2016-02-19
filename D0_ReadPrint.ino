//Snowflakes WiFi 
//  ReadPatternRecord()
//  ReadProfileRecord()
//  GetPatternName()
//  
//  PrintCheckStartOfPattern()
//  PrintCheckRecord()
//  PrintEndOfTable()
//  
//  PrintStartOfPattern()
//  PrintRecord()
//  
//  PrintRecordContents()
//  RecordNumberToColumnNumber()
//  FatalTableError()
//  
//    18Feb2016  Dean Garton
//      get and print records
//      

void ReadPatternRecord(void)
{
  uint32_t Length;
  
  //read from eeprom
  Length = sizeof(PatternRecord);
  ReadEeprom(PatternAddress, PatternRecord, Length);
}

void ReadProfileRecord(uint8_t ProfileNumber)
{
  uint32_t ProfileNumber32;
  uint32_t Length;
  uint32_t MemoryAddress;
  
  //read from eeprom
  ProfileNumber32 = ProfileNumber;
  Length = sizeof(ProfileRecord);
  MemoryAddress = ProfileNumber32 * Length;
  ReadEeprom(MemoryAddress, ProfileRecord, Length);
}

void GetPatternName(void)
{
  uint8_t Index;

  //loop to copy characters
  Index = 0;
  while(Index < 8)
  {
    //copy one character
    PatternName[Index] = PatternRecord[Index+3];
    
    //next
    Index += 1;
  }

  //add end of string
  PatternName[8] = '\0';
}

void PrintCheckStartOfPattern(void)
{
  Serial.println("PatternName   Record# : RecordContents");
}

void  PrintCheckRecord(uint32_t Length)
{
  char ColumnNumber[3];
  
  //print header
  Serial.print(PatternName);
  Serial.print("      ");
  if(PatternRecord[0] == 0x80)
  {
    Serial.print(RecordNumber, HEX);
    Serial.print("       : ");
  }
  else
  {
    RecordNumberToColumnNumber(ColumnNumber);
    Serial.print(ColumnNumber);
    Serial.print("      : ");
  }
  
  //print record contents
  PrintRecordContents(Length);
}

void PrintEndOfTable(void)
{
  Serial.println("==================================");
}

void PrintStartOfPattern(void)
{
  Serial.println("PatternName  Reps  Scale  Record# : RecordContents");
}

void  PrintRecord(uint32_t Length)
{
  char ColumnNumber[3];
  
  //print header
  Serial.print(PatternName);
  Serial.print("     ");
  Serial.print(PatternReps);
  Serial.print("     ");
  Serial.print(ScaleFactor);
  Serial.print("    ");
  RecordNumberToColumnNumber(ColumnNumber);
  Serial.print(ColumnNumber);
  Serial.print("      : ");

  //print record contents
  PrintRecordContents(Length);
}

void PrintRecordContents(uint32_t Length)
{
  uint16_t Index;
  
  //loop to print record contents
  Index = 0;
  while(Index < Length)
  {
    Serial.print(PatternRecord[Index], HEX);
    Serial.print(" ");
    Index += 1;
  }
  Serial.println("");
}

void RecordNumberToColumnNumber(char *ColumnNumber)
{
  //calculate MSD
  if(RecordNumber <= 26)
  {
    
    ColumnNumber[0] = ' ';
  }
  else
  {
    ColumnNumber[0] = (((RecordNumber-1)/26)-1) + 'A';
  }

  //calculate LSD
  if(RecordNumber == 0)
  {
    //case per record ID
    switch(PatternRecord[0])
    {
      //start of pattern record
      case 0x90:
        ColumnNumber[0] = 'S';
        ColumnNumber[1] = 'P';
      break;
  
      //end of table record
      case 0x91:
        ColumnNumber[0] = 'E';
        ColumnNumber[1] = 'T';
      break;
  
      //other record
      default:
        ColumnNumber[0] = '0';
        ColumnNumber[1] = '0';
      break;
    }
  }
  else
  {
    ColumnNumber[1] = ((RecordNumber-1)%26) + 'A';
  }
  
  //add end of string
  ColumnNumber[2] = '\0';
}

void FatalTableError()
{
  uint32_t Length;
    
  //print record data
  Length = sizeof(PatternRecord);
  PrintRecord(Length);
      
  //print error message
  Serial.println("Bad record ID");
  
  //quit
  StopExecution();
  PatternState = 10;
}

