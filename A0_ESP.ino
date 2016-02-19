//Snowflakes WiFi 
//  globals
//  setup()
//  loop()
//  HandleTimerIRQ()
//  StartExecution()
//  StopExecution()
//  
//    18Feb2016  Dean Garton
//      display patterns per tables in EEPROM

//version number
#define Version 0x0001

#include <Wire.h>
#include <Ticker.h>

//objects
Ticker TimerIRQ;

//variables
uint8_t Execute;

//pattern state machine
  //execution
  uint8_t  PatternState;
  uint32_t PatternAddress;
  uint16_t PatternTimer;
  
  //wrap
  uint32_t PatternStartTableAddress;
  uint32_t PatternStartAddress;
  
  //counters
  uint16_t PatternNumber;
  uint16_t RecordNumber;
  
  uint8_t  PatternReps;
  uint8_t  ScaleFactor;
  char     PatternName[9];
  
  //print flags
  uint8_t  StartOfPattern;
  uint8_t  EndOfTable;

  //buffers
  uint8_t PatternRecord[18];
  uint8_t ProfileRecord[6];

//profile state machines
  //execution structure
  struct   ProfileData_t
  {
    uint8_t  State[16];
    uint16_t StartIntensity[16];
    uint16_t EndIntensity[16];
    uint16_t RampTime[16];
    uint16_t Numerator[16];
    uint32_t Denominator[16];
    uint16_t BlinkOnTime[16];
    uint16_t BlinkOffTime[16];
  };
  ProfileData_t ProfileData[2];
  uint8_t  ProfileIndex;
  uint8_t  ProfileNextIndex;

  //timer
  uint16_t ProfileTimer[16];

  //variables
  uint8_t IntensityState;

void setup()
{
  uint16_t TestPatternDelay;
  
  //begin library routines
  Serial.begin(115200);
  Wire.begin(0, 2);       //ESP8266 SDA=0 and SCL=2

  //initialize PWM
  InitializePWM();

  //display test pattern
  TestPatternDelay = 1000;
  DisplayTestPattern(TestPatternDelay);

  //start execution
  //completely interrupt driven from timer
  StartExecution();
}

void loop()
{ 
  char CharacterReceived;
  
   //if character typed
  if(Serial.available() > 0)
  { 
    //get character
    CharacterReceived =  Serial.read();
 
    //if executing
    if(Execute == 1)
    {
      //stop ececution
      StopExecution();
    }
    else
    {
      //if W typed
      if(CharacterReceived == 'W')
      {
          //write test data to EEPROM
          WriteTestData();
      }
      else
      {
        //start execution
        StartExecution();
      }
    }
  }
}

void HandleTimerIRQ(void)
{
  //pattern state machine
  PatternStateMachine();

  //profile state machine
  ProfileStateMachines();
}

void StartExecution(void)
{       
   //initialize pattern state
   //pattern state machine will initialize itself and profile state machines
   PatternState = 0;
  
  //attach callback function to ticker
  TimerIRQ.attach_ms(10, HandleTimerIRQ);

  //print message
  Serial.println("Execution Started");
  Serial.println("Send any character to stop execution");

  //change state
  Execute = 1;
}

void StopExecution(void)
{
  //detach callback function from ticker
  TimerIRQ.detach();

  //print message
  Serial.println("Execution Stopped");
  Serial.println("Send W to write default pattern");
  Serial.println("Send any other character to start execution");
  
  //change state
  Execute = 0;

  //delay
  delay(10);

  //all off
  AllPWMOff();
}

