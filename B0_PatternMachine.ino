//Snowflakes WiFi 
//  PatternStateMachine()
//  InitializeStateMachines()
//  
//    18Feb2016  Dean Garton
//      Check the EEPROM tables
//      Execute the pattern records

void PatternStateMachine(void)
{
  //decrement pattern timer
  if(PatternTimer > 0)
  {
    PatternTimer -= 1;
  }

  //execute next pattern
  if((PatternTimer == 0) && (PatternState == 9))
  {
    NextPattern();
  }
  
  //case per state
  switch(PatternState)
  {
    //initialize
    case 0:
       InitializeStateMachines();
    break;
      
    //check first record ID
    case 1:
      CheckFirstRecord();
    break;

    //check profile records
    case 2:
      CheckProfileRecords();
    break;

    //check pattern records
    case 3:
      CheckPatternRecords();
    break;

    //get pattern record
    case 4:
      GetPatternRecord();
    break;

    //get profiles 0-3
    case 5:
      GetProfileRecords(0, 3);
    break;

    //get profiles 4-7
    case 6:
      GetProfileRecords(4, 7);
    break; 

    //get profiles 8-11
    case 7:
      GetProfileRecords(8, 11);
    break; 

    //get profiles 12-15
    case 8:
      GetProfileRecords(12, 15);
    break; 

    //wait for timeout
    case 9:
      //wait for timeout
    break;

    //fatal error
    case 10:
      //do nothing
    break;

    //error
    default:
      //print message
      Serial.println(PatternState, HEX);
      Serial.println(" ");
      Serial.println("Invalid Pattern State");
      
      //quit
      StopExecution();
      PatternState = 10;
  }
}

void InitializeStateMachines()
{
  uint8_t Index;

  //initialize pattern state machine variables
    //execution
    PatternAddress = 0;
    PatternTimer = 0;
    
    //counters
    PatternNumber = 0;
    RecordNumber = 0;
    
    PatternReps = 0;
    ScaleFactor = 0x64;
    strcpy(PatternName, "Profile ");
        
    //print flags
    StartOfPattern = 0;
    EndOfTable = 0;
 
  //initialize profile state machines variables
    //execution structure
    ProfileNextIndex = 0;
    Index = 0;
    while(Index < 16)
    {
      //set state to fixed intensity
      ProfileData[ProfileNextIndex].State[Index] = 0x38;
       
      //intensity = off
      ProfileData[ProfileNextIndex].StartIntensity[Index] = 0x0000;

      //set profile timer to 0
      ProfileTimer[Index] = 0;
      
      //next
      Index += 1;
    }
    ProfileIndex = 0;
    ProfileNextIndex = 1;

  //next state
  PatternState += 1;
}

