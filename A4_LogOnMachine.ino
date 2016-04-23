/********************************************************
Snowflakes WiFi 
  A4_LogOnMachine

  if (logon info exists in EEPROM)
    get logon info from EEPROM
  else
    prompt user for logon info and save in EEPROM
  endif
********************************************************/

void LogOnStateMachine(void)
{
  uint8_t EndLoop;

  //loop to get logon info
  EndLoop = 0;
  while(EndLoop == 0)
  {
    //case per state
    switch(LogOnState)
    {
      //check for ssid record
      case 0:
         CheckSSID();
      break;
      
       //prompt for ssid
      case 1:
          PromptSSID();
      break;
    
      //set ssid
      case 2:
        SetSSID();
      break;

      //prompt for password
      case 3:
        PromptPassword();
      break;
    
      //set password
      case 4:
        SetPassword();
      break;
    
      //get ssid
      case 5:
        GetSSID();
      break;
    
      //get password
      case 6:
        GetPassword();
      break;
        
      //done - exit
      case 7:
        EndLoop = 1;
      break;
    
      //error
      default:
        LogOnStateError();
      break;
    }
    
    //give operating system a chance to stroke watchdog timer
    delay(1);
  }
}

