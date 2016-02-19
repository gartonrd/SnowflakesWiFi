//Snowflakes WiFi 
//  ProfileStateMachines()
//  ExecuteProfileStateMachine()
//  SetTimerIntensityState()
//  SetProfileIntensity()
//    set on, off, dim from 8 bit intensity level
//  DecrementProfileTimer()
//    decrement timer
//    if(timeout) increment state
//     
//    18Feb2016  Dean Garton
//      Execute profile for each snowflake

void ProfileStateMachines(void)
{
  uint8_t Index;

  //loop thru all state machines
  Index = 0;
  while(Index < 16)
  {
    //execute state machine
    ExecuteProfileStateMachine(Index);

    //set intensity
    SetProfileIntensity(Index);

    //decrement timer
    DecrementProfileTimer(Index);
      
    //next state machine
    Index += 1;
  }
}

void ExecuteProfileStateMachine(uint8_t Index)
{
  uint16_t Timer;
  uint16_t ElapsedTime;
  uint16_t IntensityMinimum;

  //case per state
  IntensityState = 3;
  switch(ProfileData[ProfileIndex].State[Index])
  { 
    //do nothing
    case 0x00:
      //do nothing
    break;

    //init fixed intensity
    case 0x38:
      Timer = 0;
      SetTimerIntensityState(Index, Timer, 1, 0x00);
    break;
         
    //init blink
    case 0x08:
      Timer = ProfileData[ProfileIndex].BlinkOnTime[Index];
      SetTimerIntensityState(Index, Timer, 1, 0x09);
    break;

    //init blink on once
    case 0x18:
      Timer = ProfileData[ProfileIndex].BlinkOnTime[Index];
      SetTimerIntensityState(Index, Timer, 1, 0x19);
    break;

    //init blink off once
    case 0x28:
      Timer = ProfileData[ProfileIndex].BlinkOffTime[Index];
      SetTimerIntensityState(Index, Timer, 0, 0x29);
    break;

    //init ramp up
    case 0x10:
      Timer = ProfileData[ProfileIndex].RampTime[Index];
      SetTimerIntensityState(Index, Timer, 1, 0x11);
    break;

    //init ramp down
    case 0x20:
      Timer = ProfileData[ProfileIndex].RampTime[Index];
      SetTimerIntensityState(Index, Timer, 1, 0x21);
    break;

    //blink on
    case 0x09:
      //wait for timeout
    break;
    
    //blink on timeout
    case 0x0A:
      Timer = ProfileData[ProfileIndex].BlinkOffTime[Index];
      SetTimerIntensityState(Index, Timer, 0, 0x0B);
    break;
    
    //blink off
    case 0x0B:
      //wait for timeout
    break;
    
    //blink off timeout
    case 0x0C:
      Timer = ProfileData[ProfileIndex].BlinkOnTime[Index];
      SetTimerIntensityState(Index, Timer, 1, 0x09);
    break;

    //blink on once
    case 0x19:
      //wait for timeout
    break;

    //blink on once timeout
    case 0x1A:
      Timer = 0;
      SetTimerIntensityState(Index, Timer, 0, 0x00);
    break;

    //blink off once
    case 0x29:
      //wait for timeout
    break;

    //blink off once timeout
    case 0x2A:
      Timer = 0;
      SetTimerIntensityState(Index, Timer, 1, 0x00);
    break;

    //ramp up
    case 0x11:
      //next intensity
      ElapsedTime = ProfileData[ProfileIndex].RampTime[Index] - ProfileTimer[Index];
      IntensityMinimum = ProfileData[ProfileIndex].StartIntensity[Index];
      WriteRampIntensity(Index, ElapsedTime, IntensityMinimum);
    break;

    //ramp up timeout
    case 0x12:
      Timer = 0;
      SetTimerIntensityState(Index, Timer, 2, 0x00);
    break;

    //ramp down
    case 0x21:
      //next intensity
      ElapsedTime = ProfileTimer[Index];
      IntensityMinimum = ProfileData[ProfileIndex].EndIntensity[Index];
      WriteRampIntensity(Index, ElapsedTime, IntensityMinimum);
    break;

    //ramp down timeout
    case 0x22:
      Timer = 0;
      SetTimerIntensityState(Index, Timer, 2, 0x00);
    break;

    //error
    default:
      //print message
      Serial.print(Index);
      Serial.print(" ");
      Serial.print(ProfileData[ProfileIndex].State[Index], HEX);
      Serial.print(" ");
      Serial.println("Invalid Profile State");
      
      //quit
      StopExecution();
      PatternState = 10;
    break;
  }
}

void SetTimerIntensityState(uint8_t Index, uint32_t Timer, uint8_t Intensity, uint8_t  State)
{
  //set timer
  ProfileTimer[Index] = Timer;

  //set intensity
  IntensityState = Intensity;

  //set state
  ProfileData[ProfileIndex].State[Index] = State;
}

void SetProfileIntensity(uint8_t Index)
{
  uint16_t Intensity;

  //set intensity per state
  switch(IntensityState)
  { 
    //off
    case 0:
      Intensity = 0x0000;
      WriteIntensity(Index, Intensity);
    break;

    //start intensity
    case 1:
      Intensity = ProfileData[ProfileIndex].StartIntensity[Index];
      WriteIntensity(Index, Intensity);
    break;
         
    //end intensity
    case 2:
      Intensity = ProfileData[ProfileIndex].EndIntensity[Index];
      WriteIntensity(Index, Intensity);
    break;

    //do nothing
    case 3:
      //do nothing
    break;

    //error
    default:
    //print message
      Serial.print(Index);
      Serial.print(" ");
      Serial.print(IntensityState, HEX);
      Serial.print(" ");
      Serial.println("Invalid Profile Intensity State");
      
      //quit
      StopExecution();
      PatternState = 10;
    break;
  }
}

void DecrementProfileTimer(uint8_t Index)
{
  //decrement profile timer
  if(ProfileTimer[Index] > 0)
  {
    ProfileTimer[Index] -=1;
    if(ProfileTimer[Index] == 0)
    {
      ProfileData[ProfileIndex].State[Index] += 1;
    }
  } 
}


