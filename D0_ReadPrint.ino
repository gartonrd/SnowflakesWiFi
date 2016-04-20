//Snowflakes WiFi 
//  ReadPatternRecord()
//  GetPatternName()
//  
//  WebPrint()
//
//  GetCheckHeading()
//  GetCheckRecord()
//  
//  GetHeading()
//  GetRecord()
//
//  GetRecordNumber()
//  GetRecordContents()
//
//  GetNewLine()
//  GetBreakLine()
//
//  PatternStateError()
//  RecordIDError()
//  ProfileNumberError()
//  ProfileStateError()
// 
//    11Mar2016  Dean Garton
//      version 2
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

String GetPatternName(void)
{
  uint8_t Index;

  //loop to copy characters
  Index = 0;
  while(Index < 8)
  {
    //copy one character
    PatternName[Index] = PatternRecord[Index+2];
    
    //next
    Index += 1;
  }

  //add end of string
  PatternName[8] = '\0';
  return PatternName;
}

void WebPrint(String output)
{
  WebOutput = output;
  Serial.print(output);  
}

String GetCheckHeading(void)
{
  return "\nPatternName  Record# : RecordContents\n";
}

String  GetCheckRecord(uint32_t Length)
{ 
  String check_record;
  check_record += PatternName;
  check_record += "     ";
  check_record += GetRecordNumber();
  check_record += "       : ";
  check_record += GetRecordContents(Length);
  return check_record;
}

String GetHeading(void)
{
  return "\nPatternName  Reps  Record# : RecordContents\n";
}

String  GetRecord(uint32_t Length)
{ 
  String record;
  record += PatternName;
  record += "     ";
  record += PatternReps;
  record += "     ";
  record += GetRecordNumber();
  record += "       : ";
  record += GetRecordContents(Length);
  return record;
}

String GetRecordNumber()
{
  switch(PatternRecord[0])
  {
     //profile record
    case 0x80:
      return String(PatternRecord[1]);
    break;

    //pattern record
    case 0x81:
      return String(RecordNumber);
    break;

    //space
    default:
      return " ";
    break;
  }
}

String GetRecordContents(uint32_t Length)
{
  uint16_t Index;
  String contents;
  //loop to print record contents
  Index = 0;
  while(Index < Length)
  {
    contents += String(PatternRecord[Index], HEX);
    contents += " ";
    Index += 1;
  }
  contents += '\n';
  return contents;
}

String GetBreakLine(void)
{
  return "\n==================================\n";
}

void PatternStateError(void)
{
  //print error message
  Serial.println("");
  Serial.print("BAD PATTERN STATE: ");
  Serial.println(PatternState, HEX);
  Serial.println("");
  
  //quit
  StopExecution();
  PatternState = 7;
}

void RecordIDError(void)
{
  uint32_t Length;
    
  //print error message
  Serial.println("");
  Serial.print("BAD RECORD ID: ");

  //print record contents
  Length = sizeof(PatternRecord);
  Serial.print(GetRecordContents(Length) + "\n");
  
  //quit
  StopExecution();
  PatternState = 7;
}

void ProfileNumberError(uint16_t ProfileNumber)
{
  uint16_t ProfileSize;
    
  //print error message
  ProfileSize = PROFILE_SIZE;
  Serial.println("");
  Serial.print("BAD PROFILE NUMBER: ");
  Serial.print(ProfileNumber);
  Serial.print(" (MAXIMUM IS: ");
  Serial.print(ProfileSize);
  Serial.println(")");
  Serial.println("");
  
  //quit
  StopExecution();
  PatternState = 7;
}

void ProfileStateError(uint8_t Index)
{
  //print error message
  Serial.println("");
  Serial.print("BAD PROFILE STATE: ");
  Serial.print(ProfileState[Index], HEX);
  Serial.print(" (PROFILE: ");
  Serial.print(Index);
  Serial.println(")");
  Serial.println("");
  
  //quit
  StopExecution();
  PatternState = 7;
}


