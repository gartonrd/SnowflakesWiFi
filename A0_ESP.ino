/********************************************************
Snowflakes WiFi 
  A0_ESP
    top level routines
********************************************************/

//version number
  #define VERSION 0x000A

//includes
  #include <Wire.h>
  #include <Ticker.h>
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
  #include <ESP8266mDNS.h>
  #include <WiFiUdp.h>
  #include <ArduinoOTA.h>

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

  InitializePWM();

  //get logon information
  LogOnState = 0;
  LogOnStateMachine();
  
  StartWebServer(server);

  ArduinoOTA.onStart([]()
  {
    Serial.println("OTA Start");
  });
  
  ArduinoOTA.onEnd([]()
  {
    Serial.println("\nOTA End");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  
  ArduinoOTA.begin();
  
  DisplayTestPattern(1000);

  //completely interrupt driven from timer
  StartExecution();
  Serial.print(GetStartExecutionOptions());
}

void loop()
{ 
  char CharacterReceived;

  // Check for activity on HTTP server
  server.handleClient();
  ArduinoOTA.handle();
  
  //if character typed
  if(Serial.available() > 0)
  { 
    CharacterReceived = Serial.read();

    //wait for any more characters to arrive
    delay(10);
    
    //dump any other characters typed
    while(Serial.available() > 0)
    { 
      Serial.read();
    }

    //if executing
    if(Execute == 1)
    {
      StopExecution();
      Serial.print(GetStopExecutionOptions());
    }
    else
    {
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
  PatternStateMachine();
  
  //if pattern state machine is not redefining profiles
  if(PatternState == 6)
  {
    ProfileStateMachines();
  
    WriteAllPWM(PWMBuffer, sizeof(PWMBuffer));
  }
}

void StartExecution(void)
{       
   //initialize pattern state
   //pattern state machine will initialize itself and profile state machines
   PatternState = 0;
  
  TimerIRQ.attach_ms(10, HandleTimerIRQ);

  Serial.print(GetExecutionStartedDisplay());

  //change state
  Execute = 1;
}

void StopExecution(void)
{
  TimerIRQ.detach();

  Serial.print(GetExecutionStoppedDisplay());

  //change state
  Execute = 0;

  delay(10);

  AllPWMOff();
}

