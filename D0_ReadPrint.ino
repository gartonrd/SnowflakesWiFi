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
//  PrintHeading()
//  PrintRecord()
//
//  PrintRecordNumber()
//  PrintRecordContents()
//
//  PrintNewLine()
//  PrintBreakLine()
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

void ReadLogOnRecord(void)
{
  uint32_t Length;
  
  //read from eeprom
  Length = sizeof(LogOnRecord);
  ReadEeprom(LogOnAddress, LogOnRecord, Length);
}

void GetPatternName(void)
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

void PrintExecutionStarted()
{
  Serial.println("");
  Serial.println("EXECUTION STARTED");
}

void PrintStartExecutionOptions(void)
{
  Serial.println("Send any character to stop execution");
}

void PrintExecutionStopped(void)
{
  Serial.println("");
  Serial.println("EXECUTION STOPPED");
}

void PrintStopExecutionOptions(void)
{
  Serial.println("Send L to write logon information ");
  Serial.println("Send W to write flake test pattern table");
  Serial.println("Send any other character to start execution");
}

void PrintWriteTestDataDone()
{
  Serial.println("");
  Serial.println("FLAKE TEST PATTERN TABLE WRITTEN");
}

void PrintSSIDPrompt(void)
{
  Serial.println("");
  Serial.println("Enter SSID + Newline");
}

void PrintPasswordPrompt(void)
{
  Serial.println("");
  Serial.println("Enter Password + Newline");
}

void PrintSSID(void)
{
  Serial.print("SSID is: ");
  Serial.println(ssid);
}

void PrintPassword(void)
{
  Serial.print("Password is: ");
  Serial.println(password);
}

void PrintCheckHeading(void)
{
  Serial.println("");
  Serial.println("PatternName  Record# : RecordContents");
}

void  PrintCheckRecord(uint32_t Length)
{ 
  //print info
  Serial.print(PatternName);
  Serial.print("     ");
  PrintRecordNumber();
  Serial.print("       : ");
  PrintRecordContents(Length);
}

void PrintHeading(void)
{
  Serial.println("");
  Serial.println("PatternName  Reps  Record# : RecordContents");
}

void  PrintRecord(uint32_t Length)
{ 
  //print info
  Serial.print(PatternName);
  Serial.print("     ");
  Serial.print(PatternReps);
  Serial.print("     ");
  PrintRecordNumber();
  Serial.print("       : ");
  PrintRecordContents(Length);
}

void PrintRecordNumber()
{
  switch(PatternRecord[0])
  {
     //profile record
    case 0x80:
      Serial.print(PatternRecord[1]);
    break;

    //pattern record
    case 0x81:
      Serial.print(RecordNumber);
    break;

    //space
    default:
      Serial.print(" ");
    break;
  }
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

void PrintNewLine(void)
{
  Serial.println("");
}

void PrintBreakLine(void)
{
  Serial.println("");
  Serial.println("==================================");
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
  //print error message
  Serial.println("");
  Serial.print("BAD PATTERN STATE: ");
  Serial.println(PatternState, HEX);
  Serial.println("");
  
  //quit
  StopExecution();
  PrintStopExecutionOptions();
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
  PrintRecordContents(Length);
  Serial.println("");
  
  //quit
  StopExecution();
  PrintStopExecutionOptions();
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
  PrintStopExecutionOptions();
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
  PrintStopExecutionOptions();
  PatternState = 7;
}


