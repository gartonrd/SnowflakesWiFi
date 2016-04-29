//Snowflakes WiFi
//  globals
//  setup()
//  loop()
//  HandleTimerIRQ()
//  StartExecution()
//  StopExecution()
//
//    Apr2016 Kevin Garton
//      version 3
//        implement HTTP server for uploading
//        patterns in ASCII hex to EEPROM
//
//    11Mar2016  Dean Garton
//      version 2
//        allow profiles to be redefined for each new pattern
//        remove scale factor
//        make all times 16 bits
//        make all times 10ms resolution
//        add Blink Delay Time
//        do not turn all displays off with start of new pattern
//        show record number in decimal, not alpha
//
//    18Feb2016  Dean Garton
//      display patterns per tables in EEPROM

//version number
  #define VERSION 0x0006

//includes
  #include <Wire.h>
  #include <Ticker.h>
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>

//defines
  #define START_OF_PATTERN_LENGTH 10;
  #define PROFILE_LENGTH 12;
  #define PATTERN_LENGTH 19;           //also change in PatternRecord[] below
  #define END_OF_TABLE_LENGTH 1;
  #define PROFILE_SIZE 36;             //also change in profile parameters[] below
  #define LOGON_LENGTH 50;             //also change in LogonRecord, ssid, and password below

//objects
  Ticker TimerIRQ;
  ESP8266WebServer server(80);
  
//global variables
  uint8_t Execute;

// Used to report to the browser; see D0_ReadPrint
  const int MaxWebQueueSize = 10;
  int WebQueueSize = 0;
  String WebQueue[MaxWebQueueSize];

//profile parameters
  uint8_t  State[36];
  uint16_t StartIntensity[36];
  uint16_t EndIntensity[36];
  uint16_t RampTime[36];
  uint16_t Numerator[36];
  uint32_t Denominator[36];
  uint16_t BlinkDelayTime[36];
  uint16_t BlinkOnTime[36];
  uint16_t BlinkOffTime[36];

//logon state machine
  //execution
    uint8_t  LogOnState;
    uint32_t LogOnAddress;
  //bookkeeping
    uint32_t StartTableAddress;
    uint8_t LogOnIndex;
  //buffers
    uint8_t LogOnRecord[52];  //LOGON_LENGTH + RecordID + EndOfString
    char ssid[51];            //LOGON_LENGTH + EndOfString
    char password[51];        //LOGON_LENGTH + EndOfString

//pattern state machine
  //execution
    uint8_t  PatternState;
    uint32_t PatternAddress;
    uint16_t PatternTimer;
  //bookkeeping
    char     PatternName[9];
    uint16_t RecordNumber;
    uint8_t  PatternReps;
    uint32_t PatternStartAddress;
  //buffers
    uint8_t PatternRecord[19];

//profile state machines
  //execution
    uint8_t  ProfileState[16];
    uint8_t  ProfileIndex[16];
    uint16_t ProfileTimer[16];

//write all PWM channels
  //buffers
    uint8_t PWMBuffer[16*4];

void setup()
{
  //begin library routines
  Serial.begin(115200);
  Wire.begin(0, 2);        //ESP8266 SDA=0 and SCL=2
  Wire.setClock(400000L);  //400Khz

  // See D0_ReadPrint
  InitializeWebQueue();

  //initialize PWM
  InitializePWM();

  //get logon information
  LogOnState = 0;
  LogOnStateMachine();
  
  StartWebServer(server);

  //display test pattern
  DisplayTestPattern(1000);  

  //start execution
  //completely interrupt driven from timer
  StartExecution();
  Serial.print(GetStartExecutionOptions());
}

void loop()
{ 
  char CharacterReceived;

  // Check for activity on HTTP server
  server.handleClient();
  
  //if character typed
  if(Serial.available() > 0)
  { 
    //get character
    CharacterReceived = Serial.read();

    //wait for any more characters to arrive
    delay(10);
    
    //dump any other characters typed
    while(Serial.available() > 0)
    { 
      //get character
      Serial.read();
    }

    //if executing
    if(Execute == 1)
    {
      //stop ececution
      StopExecution();
      Serial.print(GetStopExecutionOptions());
    }
    else
    {
      //case per character typed
      switch(CharacterReceived)
      {
        //write flake test patterns to EEPROM
        case 'W':
          WriteTestData();
          Serial.print(GetStopExecutionOptions());
        break;

        //write logon information to EEPROM
        case 'L':
          LogOnState = 1;
          LogOnStateMachine();
          Serial.print(GetStopExecutionOptions());
        break;
    
        //start execution
        default:
          StartExecution();
          Serial.print(GetStartExecutionOptions());
        break;
      }
    }
  }
}

void HandleTimerIRQ(void)
{ 
  //pattern state machine
  PatternStateMachine();
  
  //if pattern state machine is not redefining profiles
  if(PatternState == 6)
  {
    //profile state machine
    ProfileStateMachines();
  
    //write all PWM channels
    WriteAllPWM(PWMBuffer, sizeof(PWMBuffer));
  }
}

void StartExecution(void)
{       
   //initialize pattern state
   //pattern state machine will initialize itself and profile state machines
   PatternState = 0;
  
  //attach callback function to ticker
  TimerIRQ.attach_ms(10, HandleTimerIRQ);

  //print message
  Serial.print(GetExecutionStartedDisplay());

  //change state
  Execute = 1;
}

void StopExecution(void)
{
  //detach callback function from ticker
  TimerIRQ.detach();

  //print message
  Serial.print(GetExecutionStoppedDisplay());
  
  //change state
  Execute = 0;

  //delay
  delay(10);

  //all off
  AllPWMOff();
}

