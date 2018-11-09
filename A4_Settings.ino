/********************************************************
Snowflakes WiFi 
  A4_Settings
    routines to manipulate settings
********************************************************/

void InitializeAllSettings()
{
  Serial.println("");
  Serial.println("------------------------------------------------------");
  InitializeSetting("SSID", "/Ssid.txt", Ssid, sizeof(Ssid)/sizeof(char));
  InitializeSetting("PASSWORD", "/Password.txt", Password, sizeof(Password)/sizeof(char));
  InitializeSetting("AC BOARD (Y/N)", "/AcBoard.txt", AcBoard, sizeof(AcBoard)/sizeof(char));
  InitializeSetting("TEST PATTERN (Y/N)", "/TestPattern.txt", TestPattern, sizeof(TestPattern)/sizeof(char));
  InitializeSetting("INTERNET CONNECTION (Y/N)", "/Internet.txt", Internet, sizeof(Internet)/sizeof(char));
  Serial.println("------------------------------------------------------");
  Serial.println("");
}

void RunMenu()
{
  Serial.println();
  Serial.println("------------------------------------------------------");
  Serial.println("X to stop EXECUTION");
  Serial.println("------------------------------------------------------");
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

void StoppedMenu(void)
{
  Serial.println();
  Serial.println("------------------------------------------------------");
  Serial.print("S to change ");
  FileToVariable("SSID", "/Ssid.txt", Ssid, sizeof(Ssid)/sizeof(char));
  Serial.print("P to change ");
  FileToVariable("PASSWORD", "/Password.txt", Password, sizeof(Password)/sizeof(char));
  Serial.print("A to specify ");
  FileToVariable("AC BOARD (Y/N)", "/AcBoard.txt", AcBoard, sizeof(AcBoard)/sizeof(char));
  Serial.print("T to run ");
  FileToVariable("TEST PATTERN (Y/N)", "/TestPattern.txt", TestPattern, sizeof(TestPattern)/sizeof(char));
  Serial.print("I to enable ");
  FileToVariable("INTERNET CONNECTION (Y/N)", "/Internet.txt", Internet, sizeof(Internet)/sizeof(char));
  Serial.println("CYCLE POWER to make any changes above take effect");
  Serial.print("FLASH MEMORY Size = ");
  Serial.print(ESP.getFlashChipRealSize());
  Serial.println(" Bytes");
  Serial.println("W to WRITE test pattern");
  Serial.println("X to start EXECUTION");
  Serial.println("------------------------------------------------------");
}

void StoppedActions(char CharacterReceived)
{

  switch(CharacterReceived)
  {
    case 's':
      ChangeSetting("SSID", "/Ssid.txt", Ssid, sizeof(Ssid)/sizeof(char));
      //menu will be displayed in main loop
    break;

    case 'p':
      ChangeSetting("PASSWORD", "/Password.txt", Password, sizeof(Password)/sizeof(char));
      //menu will be displayed in main loop
    break;

    case 'a':
      ChangeSetting("AC BOARD (Y/N)", "/AcBoard.txt", AcBoard, sizeof(AcBoard)/sizeof(char));
      //menu will be displayed in main loop
    break;

    case 't':
      ChangeSetting("TEST PATTERN (Y/N)", "/TestPattern.txt", TestPattern, sizeof(TestPattern)/sizeof(char));
      //menu will be displayed in main loop
    break;

    case 'i':
      ChangeSetting("INTERNET CONNECTION (Y/N)", "/Internet.txt", Internet, sizeof(Internet)/sizeof(char));
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

void InitializeSetting(char *Text, char *FilePath, char *VariableName, int Size)
{
  if(!SPIFFS.exists(FilePath))
  {
    SerialToFile(Text, FilePath);
  }
  FileToVariable(Text, FilePath, VariableName, Size);
}

void ChangeSetting(char *Text, char *FilePath, char *VariableName, int Size)
{
  SerialToFile(Text, FilePath);
  FileToVariable(Text, FilePath, VariableName, Size);
}

void SerialToFile(char *Text, char *FilePath)
{
  File FileHandle;
  char Character;
  int Exit;

  Serial.print("Enter new ");
  Serial.print(Text);
  Serial.println(" setting");
  
  while(Serial.available() == 0)
  {
  }
  
  FileHandle = SPIFFS.open(FilePath, "w");
  
  Exit = 0;
  while(Exit == 0)
  {
    Character = Serial.read();
    FileHandle.print(Character);
    delay(20);
    if(Serial.available()== 0)
    {
      Exit = 1;
    }
  }
  
  FileHandle.close();
}  

void FileToVariable(char *Text, char *FilePath, char *VariableName, int Size)
{
  File FileHandle;
  char Character;
  int Index;
  int Exit;
 
  FileHandle = SPIFFS.open(FilePath, "r");
  if(!FileHandle)
  {
    strcpy(VariableName, strcat("Unable to Open", FilePath));
  }
  else
  {
    Index = 0;
    Exit = 0;
    while(Exit == 0)
    {
      Character = FileHandle.read();
      switch(Character)
      {
        case '\n':
          VariableName[Index] = '\0';
          Exit = 1;
        break;

        case '\r':
          VariableName[Index] = '\0';
          Exit = 1;
        break;

        default:
          VariableName[Index] = Character;
          Index += 1;
          
          if(Index >= (Size-1))
          {
            VariableName[Index] = '\0';
            Exit = 1;
          }
          
          if(Index >= FileHandle.size())
          {
            VariableName[Index] = '\0';
            Exit = 1;
          }
        break;
      }
    }
    FileHandle.close();
  }
  Serial.print(Text);
  Serial.print(" = ");
  Serial.println(VariableName);
}

