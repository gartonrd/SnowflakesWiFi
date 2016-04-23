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
  PrintBreakLine();
  
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
        
        //save ssid record in EEPROM
        WriteEeprom(LogOnAddress, LogOnRecord, LogOnIndex+1);
        
        //print SSID
        PrintSSID();
        
        //next
        LogOnAddress += LogOnIndex+1;
        LogOnState = 3;
      break;
      
      //ssid character
      default:
        //if buffer is not full
        Length = LOGON_LENGTH;
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

        //add end of table record
        LogOnRecord[LogOnIndex+1] = 0x91;
        
        //save password record in EEPROM
        WriteEeprom(LogOnAddress, LogOnRecord, LogOnIndex+2);

        //print password
        PrintPassword();
        
        //next
        LogOnAddress += LogOnIndex+1;
        StartTableAddress = LogOnAddress;
        LogOnState = 7;
      break;
      
      //password character
      default:
        //if buffer is not full
        Length = LOGON_LENGTH;
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
  if(LogOnIndex < Length)
  {  
    //get next character
    Character = LogOnRecord[LogOnIndex+1];

    //cases per character
    switch(Character)
    {
      //password record ID
      case 0x83:
        //terminate ssid string
        ssid[LogOnIndex] = '\0';
        
        //print SSID
        PrintSSID();
        
        //next
        LogOnAddress += LogOnIndex+1;
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
  if(LogOnIndex < Length)
  {  
    //get next character
    Character = LogOnRecord[LogOnIndex+1];

    //cases per character
    switch(Character)
    {
      //start of pattern record ID
      case 0x90:
        //terminate password string
        password[LogOnIndex] = '\0';
        
        //print Password
        PrintPassword();
        
        //next
        LogOnAddress += LogOnIndex+1;
        StartTableAddress = LogOnAddress;
        LogOnState = 7;
      break;

      //end of table record ID
      case 0x91:
        //terminate password string
        password[LogOnIndex] = '\0';
        
        //print Password
        PrintPassword();
        
        //next
        LogOnAddress += LogOnIndex+1;
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

