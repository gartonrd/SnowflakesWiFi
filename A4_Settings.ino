/********************************************************
Snowflakes WiFi 
  A4_Settings
    routines to manipulate settings
********************************************************/

String InitializeAllSettings()
{
  String Result;

  InitializeSetting("NETWORKID", Ssid);
  InitializeSetting("PASSWORD", Password);
  InitializeSetting("ACBOARD", AcBoard);
  InitializeSetting("TESTPATTERN", TestPattern);
  InitializeSetting("INTERNET", Internet);

  Result += '\n';
  Result += ShowSetting("SSID = " , Ssid);
  Result += ShowSetting("PASSWORD = ", Password);
  Result += ShowSetting("AC BOARD = " , AcBoard);
  Result += ShowSetting("TEST PATTERN = " , TestPattern);
  Result += ShowSetting("INTERNET CONNECT= " , Internet);
  Result += '\n';
  
  return Result;
}

String RunMenu()
{
  String Result;

  Result += '\n';
  Result += "---SET LINE ENDING TO NEW LINE---\n";
  Result += "X to stop EXECUTION\n";
  Result += '\n';
  
  return Result;
}

void RunActions(char CharacterReceived)
{
  switch(CharacterReceived)
  {
    case 'x':
      StopExecution();
      //menu will be displayed in main loop
    break;

    default:
      //menu will be displayed in main loop
    break;
  }
}

String StoppedMenu(void)
{
  String Result;

  Result += '\n';
  Result += "---SET LINE ENDING TO NEW LINE---\n";
  Result += ShowSetting("S to change SSID = " , Ssid);
  Result += ShowSetting("P to change PASSWORD = ", Password);
  Result += ShowSetting("A to specify AC BOARD (Y/N) = " , AcBoard);
  Result += ShowSetting("T to run TEST PATTERN (Y/N)= " , TestPattern);
  Result += ShowSetting("I to connect to INTERNET = " , Internet);
  Result += "CYCLE POWER to make any changes above take effect\n";
  Result += "W to WRITE test pattern\n";
  Result += "X to start EXECUTION\n";
  Result += '\n';

  return Result;
}

void StoppedActions(char CharacterReceived)
{

  switch(CharacterReceived)
  {
    case 's':
      SetSetting("NETWORKID", Ssid);
      //menu will be displayed in main loop
    break;

    case 'p':
      SetSetting("PASSWORD", Password);
      //menu will be displayed in main loop
    break;

    case 'a':
      SetSetting("ACBOARD", AcBoard);
      //menu will be displayed in main loop
    break;

    case 't':
      SetSetting("TESTPATTERN", TestPattern);
      //menu will be displayed in main loop
    break;

    case 'i':
      SetSetting("INTERNET", Internet);
      //menu will be displayed in main loop
    break;

    case 'w':
      WriteTestData();
      //menu will be displayed in main loop
    break;

    case 'x':
      StartExecution();
      //menu will be displayed in main loop
    break;

    default:
      //menu will be displayed in main loop
    break;
  }

}

void InitializeSetting(char *FileName, char* VariableName)
{
  int Result;

  Result = GetSetting(FileName, VariableName);
  if(Result != 0)
  {
    SetSetting(FileName, VariableName);
  }
}

void SetSetting(char *FileName, char* VariableName)
{

  //stub
  Ssid = "gartonrd";
  Password = "silverturkey";
  AcBoard = "y";
  TestPattern = "n";
  Internet = "y";

}

int GetSetting(char *FileName, char* VariableName)
{
  int Result;

  Result = 0;
  
  //stub
  if(Result == 0)
  {
    Ssid = "gartonrd";
    Password = "silverturkey";
    AcBoard = "n";
    TestPattern = "n";
    Internet = "y";
  }

  return Result;
}

String ShowSetting(char *ShowText, char* VariableName)
{ 
  String Result;

  Result += ShowText;
  Result += VariableName;
  Result += '\n';
  
  return Result;
}
