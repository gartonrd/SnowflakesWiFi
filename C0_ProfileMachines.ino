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
  //fixed intensity -------------------------------------------------------
    //init fixed intensity
    case 0x00:
      Timer = 0;
      Intensity = StartIntensity[ProfileIndex[Index]];
      WriteTimerIntensityState(Index, Timer, Intensity, 0x70);
    break;

    //blink off once --------------------------------------------------------
    //init blink off once
    case 0x10:
      Timer = BlinkOffTime[ProfileIndex[Index]];
      Intensity = 0x0000;
      WriteTimerIntensityState(Index, Timer, Intensity, 0x11);
    break;

   //blink off once
    case 0x11:
      //wait for timeout
    break;

    //blink off once timeout
    case 0x12:
      Timer = 0;
      Intensity = StartIntensity[ProfileIndex[Index]];
      WriteTimerIntensityState(Index, Timer, Intensity, 0x70);
    break;

 //blink on once ---------------------------------------------------------
    //init blink on once
    case 0x20:
      Timer = BlinkOnTime[ProfileIndex[Index]];
      Intensity = StartIntensity[ProfileIndex[Index]];
      WriteTimerIntensityState(Index, Timer, Intensity, 0x21);
    break;

    //blink on once
    case 0x21:
      //wait for timeout
    break;

    //blink on once timeout
    case 0x22:
      Timer = 0;
      Intensity = 0x0000;
      WriteTimerIntensityState(Index, Timer, Intensity, 0x70);
    break;

    //delay on once ---------------------------------------------------------
    //init delay on once
    case 0xA0:
      Timer = BlinkDelayTime[ProfileIndex[Index]];
      Intensity = 0x0000;
      WriteTimerIntensityState(Index, Timer, Intensity, 0xA1);
    break;

    //delay on once
    case 0xA1:
      //wait for timeout
    break;

    //delay on once timeout
    case 0xA2:
      Timer = BlinkOnTime[ProfileIndex[Index]];
      Intensity = StartIntensity[ProfileIndex[Index]];
      WriteTimerIntensityState(Index, Timer, Intensity, 0x21);
    break;

    //blink -----------------------------------------------------------------
    //init blink
    case 0x30:
      Timer = BlinkOnTime[ProfileIndex[Index]];
      Intensity = StartIntensity[ProfileIndex[Index]];
      WriteTimerIntensityState(Index, Timer, Intensity, 0x31);
    break;
    
    //blink on
    case 0x31:
      //wait for timeout
      
    break;
    
    //blink on timeout
    case 0x32:
      Timer = BlinkOffTime[ProfileIndex[Index]];
      Intensity = 0x0000;
      WriteTimerIntensityState(Index, Timer, Intensity, 0x33);
    break;
    
    //blink off
    case 0x33:
      //wait for timeout
    break;
    
    //blink off timeout
    case 0x34:
      Timer = BlinkOnTime[ProfileIndex[Index]];
      Intensity = StartIntensity[ProfileIndex[Index]];
      WriteTimerIntensityState(Index, Timer, Intensity, 0x31);
    break;

    //delay blink -----------------------------------------------------------
    //init delay blink
    case 0xB0:
      Timer = BlinkDelayTime[ProfileIndex[Index]];
      Intensity = 0x0000;
      WriteTimerIntensityState(Index, Timer, Intensity, 0xB1);
    break;

    //delay blink
    case 0xB1:
      //wait for timeout
    break;

    //delay blink timeout
    case 0xB2:
      Timer = BlinkOnTime[ProfileIndex[Index]];
      Intensity = StartIntensity[ProfileIndex[Index]];
      WriteTimerIntensityState(Index, Timer, Intensity, 0x31);
    break;
    
    //ramp up ---------------------------------------------------------------
    //init ramp up
    case 0x40:
      Timer = RampTime[ProfileIndex[Index]];
      Intensity = StartIntensity[ProfileIndex[Index]];
      WriteTimerIntensityState(Index, Timer, Intensity, 0x41);
    break;

    //ramp up
    case 0x41:
      //next intensity
      ElapsedTime = RampTime[ProfileIndex[Index]] - ProfileTimer[Index];
      IntensityMinimum = StartIntensity[ProfileIndex[Index]];
      WriteRampIntensity(Index, ElapsedTime, IntensityMinimum);
    break;

    //ramp up timeout
    case 0x42:
      Timer = 0;
      Intensity = EndIntensity[ProfileIndex[Index]];
      WriteTimerIntensityState(Index, Timer, Intensity, 0x70);
    break;
 
    //ramp down ------------------------------------------------------------- 
    //init ramp down
    case 0x50:
      Timer = RampTime[ProfileIndex[Index]];
      Intensity = StartIntensity[ProfileIndex[Index]];
      WriteTimerIntensityState(Index, Timer, Intensity, 0x51);
    break;

    //ramp down
    case 0x51:
      //next intensity
      ElapsedTime = ProfileTimer[Index];
      IntensityMinimum = EndIntensity[ProfileIndex[Index]];
      WriteRampIntensity(Index, ElapsedTime, IntensityMinimum);
    break;

    //ramp down timeout
    case 0x52:
      Timer = 0;
      Intensity = EndIntensity[ProfileIndex[Index]];
      WriteTimerIntensityState(Index, Timer, Intensity, 0x70);
    break;

    //do nothing ------------------------------------------------------------
    case 0x70:
      //do nothing
    break;
    
    //error -----------------------------------------------------------------
    default:
      ProfileStateError(Index);
    break;
  }
}

