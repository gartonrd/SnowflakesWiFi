/********************************************************
Snowflakes WiFi 
  B0_PatternMachine
    Execute the pattern records
********************************************************/

void PatternStateMachine(void)
{
  if(PatternTimer > 0)
  {
    PatternTimer -= 1;
  }

  //if timeout
  if((PatternTimer == 0) && (PatternState == 6))
  {
    //execute next pattern
    PatternState = 5;
  }
  
  //case per state
  switch(PatternState)
  {
     //initialize
    case 0:
       InitializeStateMachines();
    break;

    //check first record
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

    //get profile records
    case 4:
      GetProfileRecords();
    break;
      
    //get pattern record
    case 5:
      GetPatternRecord();
    break;

    //wait for timeout
    case 6:
      //wait for timeout
    break;

    //fatal error
    case 7:
      //do nothing
    break;

    //error
    default:
      PatternStateError();
    break;
  }
}

void InitializeStateMachines()
{
  uint8_t Index;
    StartTableAddress = 0;
    PatternAddress = StartTableAddress;
    PatternTimer = 0;
    
    RecordNumber = 0;
    PatternReps = 0;

  //initialize profile state machines variables
  Index = 0;
  while(Index < 16)
  {
    //set state to do nothing
    ProfileState[Index] = 0x00;
    
    ProfileIndex[Index] = 0;
    ProfileTimer[Index] = 0;
    
    Index += 1;
  }

  //next state
  PatternState = 1;
}
