/********************************************************
Snowflakes WiFi 
  A5_LogOn
    Logon cases
********************************************************/

void CheckSSID(void)
{
  uint32_t Length;

  //initialize bookkeeping
  LogOnAddress = 0;

  //print break line
  Serial.print(GetBreakLine());
  
  //read from eeprom
  ReadLogOnRecord();

  //case per eeprom data
  switch(LogOnRecord[0])
  {
    //ssid record
    case 0x82:
      //next
      LogOnIndex = 0;
      LogOnState = 5;
    break;

    //ssid not found in EEPROM
    default:
      LogOnState = 1;
    break;
  }
}

void PromptSSID(void)
{
  //initialize
  LogOnAddress = 0;
  LogOnIndex = 0;
  LogOnRecord[0] = 0x82;
  
  //prompt for ssid
  PrintSSIDPrompt();
  
  //next
  LogOnState = 2;
}

void SetSSID(void)
{
  char CharacterReceived;
  uint8_t Length;

  //if there is another character
  Length = LOGON_LENGTH;
  if(Serial.available() > 0)
  { 
    //get character
    CharacterReceived = Serial.read();

    //cases per character
    switch(CharacterReceived)
    {
      case '\n':
        //terminate ssid string
        ssid[LogOnIndex] = '\0';

        //terminate ssid string in record
        LogOnRecord[LogOnIndex+1] = '\0';
        
        //save ssid record in EEPROM
        WriteEeprom(LogOnAddress, LogOnRecord, Length+2);
        
        //print SSID
        PrintSSID();
        
        //next
        LogOnAddress += Length+2;
        LogOnState = 3;
      break;
      
      //ssid character
      default:
        //if buffer is not full
        if(LogOnIndex < Length)
        {
          //save character in ssid string
          ssid[LogOnIndex] = CharacterReceived;
          
          //save character in ssid record
          LogOnRecord[LogOnIndex+1] = CharacterReceived;

          //next
          LogOnIndex += 1;
        }
        else
        {
          //dump any other characters typed
          while(Serial.available() > 0)
          { 
            //get character
            Serial.read();
          }
          
          //print error message
          LogOnLengthError();

          //try again
          LogOnState = 1;
        }
      break;
    }
  }
}

void PromptPassword(void)
{
  //initialize
  LogOnIndex = 0;
  LogOnRecord[0] = 0x83;
  
  //prompt for password
  PrintPasswordPrompt();
  
  //next
  LogOnState = 4;
}

void SetPassword(void)
{
  char CharacterReceived;
  uint8_t Length;

  //if there is another character
  Length = LOGON_LENGTH;
  if(Serial.available() > 0)
  { 
    //get character
    CharacterReceived = Serial.read();

    //cases per character
    switch(CharacterReceived)
    {
      case '\n':
        //terminate password string
        password[LogOnIndex] = '\0';

        //terminate password string in record
        LogOnRecord[LogOnIndex+1] = '\0';
        
        //save password record in EEPROM
        WriteEeprom(LogOnAddress, LogOnRecord, Length+2);

        //print password
        PrintPassword();
        
        //next
        LogOnAddress += Length+2;
        StartTableAddress = LogOnAddress;
        LogOnState = 7;
      break;
      
      //password character
      default:
        //if buffer is not full
        if(LogOnIndex < Length)
        {
          //save character in password string
          password[LogOnIndex] = CharacterReceived;
          
          //save character in password record
          LogOnRecord[LogOnIndex+1] = CharacterReceived;

          //next
          LogOnIndex += 1;
        }
        else
        {
          //dump any other characters typed
          while(Serial.available() > 0)
          { 
            //get character
            Serial.read();
          }
          
          //print error message
          LogOnLengthError();

          //try again
          LogOnState = 3;
        }
      break;
    }
  }
}

void GetSSID(void)
{ 
  char Character;
  uint8_t Length;

  //if buffer is not empty
  Length = LOGON_LENGTH;
  if(LogOnIndex <= Length)
  {  
    //get next character
    Character = LogOnRecord[LogOnIndex+1];

    //cases per character
    switch(Character)
    {
      //end of string
      case '\0':
        //save character in ssid string
        ssid[LogOnIndex] = Character;
        
        //print SSID
        PrintSSID();
        
        //next
        LogOnAddress += Length+2;
        ReadLogOnRecord();
        LogOnIndex = 0;
        LogOnState = 6;
      break;
      
      //ssid character
      default:
        //save character in ssid string
        ssid[LogOnIndex] = Character;
        
        //next
        LogOnIndex += 1;
      break;
    }
  }
  else
  {
    //ask user to enter
    LogOnState = 1;
  }
}

void GetPassword(void)
{
  char Character;
  uint8_t Length;

  //if buffer is not empty
  Length = LOGON_LENGTH;
  if(LogOnIndex <= Length)
  {  
    //get next character
    Character = LogOnRecord[LogOnIndex+1];

    //cases per character
    switch(Character)
    {
      //end of string
      case '\0':
        //save character in password string
        password[LogOnIndex] = Character;
        
        //print Password
        PrintPassword();
        
        //next
        LogOnAddress += Length+2;
        StartTableAddress = LogOnAddress;
        LogOnState = 7;
      break;
      
      //password character
      default:
        //save character in password string
        password[LogOnIndex] = Character;
        
        //next
        LogOnIndex += 1;
      break;
    }
  }
  else
  {
    //ask user to enter
    LogOnState = 3;
  }  
}

