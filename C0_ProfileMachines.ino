/********************************************************
Snowflakes WiFi 
  C0_ProfileMachines
    Execute profile for each snowflake
********************************************************/

void ProfileStateMachines(void)
{
  uint8_t Index;

  //loop thru all state machines
  Index = 0;
  while(Index < 16)
  {
    ExecuteProfileStateMachine(Index);

    if(ProfileTimer[Index] > 0)
    {
      ProfileTimer[Index] -=1;
      
      //if(timeout)
      if(ProfileTimer[Index] == 0)
      {
        ProfileState[Index] += 1;
      }
    } 
      
    //next state machine
    Index += 1;
  }
}

void ExecuteProfileStateMachine(uint8_t Index)
{
  uint16_t Timer;
  uint16_t Intensity;
  uint16_t ElapsedTime;
  uint16_t IntensityMinimum;

  switch(ProfileState[Index])
  { 
    case 0x00:
      //do nothing
    break;

    //init fixed intensity
    case 0x38:
      Timer = 0;
      Intensity = StartIntensity[ProfileIndex[Index]];
      WriteTimerIntensityState(Index, Timer, Intensity, 0x00);
    break;

    //init delay blink
    case 0x48:
      Timer = BlinkDelayTime[ProfileIndex[Index]];
      Intensity = 0x0000;
      WriteTimerIntensityState(Index, Timer, Intensity, 0x49);
    break;
         
    //init blink
    case 0x08:
      Timer = BlinkOnTime[ProfileIndex[Index]];
      Intensity = StartIntensity[ProfileIndex[Index]];
      WriteTimerIntensityState(Index, Timer, Intensity, 0x09);
    break;

    //init delay on once
    case 0x58:
      Timer = BlinkDelayTime[ProfileIndex[Index]];
      Intensity = 0x0000;
      WriteTimerIntensityState(Index, Timer, Intensity, 0x59);
    break;
    
    //init blink on once
    case 0x18:
      Timer = BlinkOnTime[ProfileIndex[Index]];
      Intensity = StartIntensity[ProfileIndex[Index]];
      WriteTimerIntensityState(Index, Timer, Intensity, 0x19);
    break;

    //init blink off once
    case 0x28:
      Timer = BlinkOffTime[ProfileIndex[Index]];
      Intensity = 0x0000;
      WriteTimerIntensityState(Index, Timer, Intensity, 0x29);
    break;

    //init ramp up
    case 0x10:
      Timer = RampTime[ProfileIndex[Index]];
      Intensity = StartIntensity[ProfileIndex[Index]];
      WriteTimerIntensityState(Index, Timer, Intensity, 0x11);
    break;

    //init ramp down
    case 0x20:
      Timer = RampTime[ProfileIndex[Index]];
      Intensity = StartIntensity[ProfileIndex[Index]];
      WriteTimerIntensityState(Index, Timer, Intensity, 0x21);
    break;

    //delay blink
    case 0x49:
      //wait for timeout
    break;

    //delay blink timeout
    case 0x4A:
      Timer = BlinkOnTime[ProfileIndex[Index]];
      Intensity = StartIntensity[ProfileIndex[Index]];
      WriteTimerIntensityState(Index, Timer, Intensity, 0x09);
    break;

    //blink on
    case 0x09:
      //wait for timeout
    break;
    
    //blink on timeout
    case 0x0A:
      Timer = BlinkOffTime[ProfileIndex[Index]];
      Intensity = 0x0000;
      WriteTimerIntensityState(Index, Timer, Intensity, 0x0B);
    break;
    
    //blink off
    case 0x0B:
      //wait for timeout
    break;
    
    //blink off timeout
    case 0x0C:
      Timer = BlinkOnTime[ProfileIndex[Index]];
      Intensity = StartIntensity[ProfileIndex[Index]];
      WriteTimerIntensityState(Index, Timer, Intensity, 0x09);
    break;

    //delay on once
    case 0x59:
      //wait for timeout
    break;

    //delay on once timeout
    case 0x5A:
      Timer = BlinkOnTime[ProfileIndex[Index]];
      Intensity = StartIntensity[ProfileIndex[Index]];
      WriteTimerIntensityState(Index, Timer, Intensity, 0x19);
    break;

    //blink on once
    case 0x19:
      //wait for timeout
    break;

    //blink on once timeout
    case 0x1A:
      Timer = 0;
      Intensity = 0x0000;
      WriteTimerIntensityState(Index, Timer, Intensity, 0x00);
    break;

    //blink off once
    case 0x29:
      //wait for timeout
    break;

    //blink off once timeout
    case 0x2A:
      Timer = 0;
      Intensity = StartIntensity[ProfileIndex[Index]];
      WriteTimerIntensityState(Index, Timer, Intensity, 0x00);
    break;

    //ramp up
    case 0x11:
      //next intensity
      ElapsedTime = RampTime[ProfileIndex[Index]] - ProfileTimer[Index];
      IntensityMinimum = StartIntensity[ProfileIndex[Index]];
      WriteRampIntensity(Index, ElapsedTime, IntensityMinimum);
    break;

    //ramp up timeout
    case 0x12:
      Timer = 0;
      Intensity = EndIntensity[ProfileIndex[Index]];
      WriteTimerIntensityState(Index, Timer, Intensity, 0x00);
    break;

    //ramp down
    case 0x21:
      //next intensity
      ElapsedTime = ProfileTimer[Index];
      IntensityMinimum = EndIntensity[ProfileIndex[Index]];
      WriteRampIntensity(Index, ElapsedTime, IntensityMinimum);
    break;

    //ramp down timeout
    case 0x22:
      Timer = 0;
      Intensity = EndIntensity[ProfileIndex[Index]];
      WriteTimerIntensityState(Index, Timer, Intensity, 0x00);
    break;

    default:
      ProfileStateError(Index);
    break;
  }
}

