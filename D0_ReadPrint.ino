//Snowflakes WiFi 
//  ReadPatternRecord()
//  GetPatternName()
//  
//  InitializeWebQueue()
//  WebPrint(String output)
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
//    Apr2016 Kevin Garton
//      Version 3
//        Changed print functions to string generators.
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

void ReadLogOnRecord(void)
{
  uint32_t Length;
  
  Length = sizeof(LogOnRecord);
  ReadEeprom(LogOnAddress, LogOnRecord, Length);
}

String GetPatternName(void)
{
  uint8_t Index;

  //loop to copy characters
  Index = 0;
  while(Index < 8)
  {
    PatternName[Index] = PatternRecord[Index+2];
    Index += 1;
  }

  PatternName[8] = '\0';
  return PatternName;
}

void InitializeWebQueue(){
  int j;
  for(j=0; j<MaxWebQueueSize; ++j){
    WebQueue[j] = "";
  }
}

void WebPrint(String output)
{
  Serial.print(output);

  if(WebQueueSize >= MaxWebQueueSize)
  {
    int j;
    for(j=MaxWebQueueSize-2; j>=0; --j)
    {
      WebQueue[j+1] = WebQueue[j];
    }
    WebQueue[0] = output;
  }
  else
  {
    WebQueue[WebQueueSize++] = output;
  }
}

String GetExecutionStartedDisplay()
{
  return "\nEXECUTION STARTED\n";
}

String GetStartExecutionOptions()
{
  return "Send any character to stop execution\n";
}

String GetExecutionStoppedDisplay()
{
  return "\nEXECUTION STOPPED\n";
}

String GetStopExecutionOptions(void)
{
  String returner = "Send L to write logon information \n";
  returner += "Send W to write flake test pattern table\n";
  returner += "Send any other character to start execution\n";
  return returner;
}

String GetWriteTestDataDone()
{
  return "\nFLAKE TEST PATTERN TABLE WRITTEN\n";
}

String GetSSIDPrompt(void)
{
  return "\nEnter SSID + Newline\n";
}

String GetPasswordDisplay()
{
  String returner = "\nPassword is: ";
  returner += password;
  returner += '\n';
  return returner;
}

String GetPasswordPrompt(void)
{
  return "Enter Password + Newline\n";
}

String GetSsidDisplay(void)
{
  String returner = "SSID is: ";
  returner += String(ssid);
  returner += '\n';
  return returner;
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

void LogOnStateError(void)
{
  //print error message
  Serial.println("");
  Serial.print("BAD LOGON STATE: ");
  Serial.println(LogOnState, HEX);
  Serial.println("");
  
  //exit
  LogOnState = 7;
}

void LogOnLengthError(void)
{
  uint8_t Length;
    
  //print error message
  Length = LOGON_LENGTH;
  Serial.println("");
  Serial.print("BAD ENTRY, MAXIMUM IS: ");
  Serial.println(Length);
  Serial.println("TRY AGAIN");
 }

void PatternStateError(void)
{
  Serial.println("");
  Serial.print("BAD PATTERN STATE: ");
  Serial.println(PatternState, HEX);
  Serial.println("");
  
  StopExecution();
<<<<<<< HEAD
  GetStopExecutionOptions();
=======
  //quit
>>>>>>> Removed comments that appeared redundant.
  PatternState = 7;
}

void RecordIDError(void)
{
  uint32_t Length;
    
  Serial.println("");
  Serial.print("BAD RECORD ID: ");

  Length = sizeof(PatternRecord);
  Serial.print(GetRecordContents(Length) + "\n");
  

  StopExecution();
<<<<<<< HEAD
  GetStopExecutionOptions();
=======
  //quit
>>>>>>> Removed comments that appeared redundant.
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
  GetStopExecutionOptions();
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
  GetStopExecutionOptions();
  //quit
  PatternState = 7;
}

