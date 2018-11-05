/********************************************************
Snowflakes WiFi 
  D0_ReadPrint
    utilities to read records from EEPROM
    utilities to print messages to the serial port
********************************************************/ 

/********************************************************
    utilities to read records from EEPROM
********************************************************/
void ReadPatternRecord(void)
{
  uint32_t Length;
  
  //read from eeprom
  Length = sizeof(PatternRecord);
  ReadEeprom(PatternAddress, PatternRecord, Length);
}

/********************************************************
    informational messages
********************************************************/

String ExecutionStartedMessage()
{
  return "\nEXECUTION STARTED\n";
}

String ExecutionStoppedMessage()
{
  return "\nEXECUTION STOPPED\n";
}

String WriteTestDataDoneMessage()
{
  return "\nFLAKE TEST PATTERN TABLE WRITTEN\n";
}

/********************************************************
    run time messages
********************************************************/

String CheckHeadingMessage(void)
{
  return "\nPatternName  Record# : RecordContents\n";
}

String  CheckRecordMessage(uint32_t Length)
{ 
  String check_record;
  check_record += PatternName;
  check_record += "     ";
  check_record += RecordNumberMessage();
  check_record += "       : ";
  check_record += RecordContentsMessage(Length);
  return check_record;
}

String HeadingMessage(void)
{
  return "\nPatternName  Reps  Record# : RecordContents\n";
}

String  RecordMessage(uint32_t Length)
{ 
  String record;
  record += PatternName;
  record += "     ";
  record += PatternReps;
  record += "     ";
  record += RecordNumberMessage();
  record += "       : ";
  record += RecordContentsMessage(Length);
  return record;
}

String RecordNumberMessage()
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

String RecordContentsMessage(uint32_t Length)
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

String BreakLineMessage(void)
{
  return "\n==================================\n";
}

/********************************************************
    error messages
********************************************************/

void PatternStateError(void)
{
  Serial.println("");
  Serial.print("BAD PATTERN STATE: ");
  Serial.println(PatternState, HEX);
  Serial.println("");
  
  StopExecution();
  Serial.print(StoppedMenu());
  
  //quit
  PatternState = 7;
}

void RecordIDError(void)
{
  uint32_t Length;
    
  Serial.println("");
  Serial.print("BAD RECORD ID: ");

  Length = sizeof(PatternRecord);
  Serial.print(RecordContentsMessage(Length) + "\n");
  
  StopExecution();
  Serial.print(StoppedMenu());
  
  //quit
  PatternState = 7;
}

void ProfileNumberError(uint16_t ProfileNumber)
{
  uint16_t ProfileSize;

  ProfileSize = PROFILE_SIZE;
  Serial.println("");
  Serial.print("BAD PROFILE NUMBER: ");
  Serial.print(ProfileNumber);
  Serial.print(" (MAXIMUM IS: ");
  Serial.print(ProfileSize);
  Serial.println(")");
  Serial.println("");
  
  StopExecution();
  Serial.print(StoppedMenu());
  
  //quit
  PatternState = 7;
}

void ProfileStateError(uint8_t Index)
{
  Serial.println("");
  Serial.print("BAD PROFILE STATE: ");
  Serial.print(ProfileState[Index], HEX);
  Serial.print(" (PROFILE: ");
  Serial.print(Index);
  Serial.println(")");
  Serial.println("");
  
  StopExecution();
  Serial.print(StoppedMenu());
  
  //quit
  PatternState = 7;
}

