/********************************************************
Snowflakes WiFi 
  A0_ESP
    top level routines
********************************************************/

//version number
  #define VERSION 0xFFFF

//includes
  #include <Wire.h>
  #include <Ticker.h>
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
  #include <ESP8266mDNS.h>
  #include <WiFiUdp.h>
  #include <ArduinoOTA.h>
  #include <FS.h>

//defines
  #define START_OF_PATTERN_LENGTH 10;
  #define PROFILE_LENGTH 12;
  #define PATTERN_LENGTH 19;           //also change in PatternRecord[] below
  #define END_OF_TABLE_LENGTH 1;
  #define PROFILE_SIZE 36;             //also change in profile parameters[] below

//objects
  Ticker TimerIRQ;
  ESP8266WebServer server(80);
  
//global variables
  uint8_t Execute;
  
//settings - will be initialized at run time
    char Ssid[51] = "ssid";          //ssid for internet connection
    char Password[51] = "password";  //password for internet connection
    char AcBoard[5] = "n";          //output polarity for AC board
    char TestPattern[5] = "y";      //run test pattern
    char Internet[5] = "y";         //connect to internet

// Used to report to the browser;
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

//pattern state machine
  //execution
    uint8_t  PatternState;
    uint32_t StartTableAddress;
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
  Serial.begin(115200);
  
  SPIFFS.begin();
  InitializeAllSettings();

  Wire.begin(0, 2);        //ESP8266 SDA=GPIO0 and SCL=GPIO2
  Wire.setClock(400000L);  //400Khz
  InitializePWM();

  if(tolower(TestPattern[0]) == 'y')
  {
    DisplayTestPattern(1000);
  }
  
  if(tolower(Internet[0]) == 'y')
  {
    InitializeWebQueue();
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
  }
  
  //completely interrupt driven from timer
  StartExecution();
   
  RunMenu();
}

void loop()
{
  char CharacterReceived;

  // Check for activity on internet
  server.handleClient();
  ArduinoOTA.handle();
  
  //if character typed
  if(Serial.available() > 0)
  {     
    CharacterReceived = tolower(Serial.read());

    //dump remaining characters
    delay(20);
    while(Serial.available() > 0)
    {
      Serial.read();
      delay(20);
    }

    //if executing
    if(Execute == 1)
    {
      RunActions(CharacterReceived);
    }
    else
    {
      StoppedActions(CharacterReceived);
    }

    //if executing
    if(Execute == 1)
    {
      RunMenu();
    }
    else
    {
      StoppedMenu();
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

  Serial.print(ExecutionStartedMessage());

  //change state
  Execute = 1;
}

void StopExecution(void)
{
  TimerIRQ.detach();

  Serial.print(ExecutionStoppedMessage());

  //change state
  Execute = 0;

  delay(10);

  AllPWMOff();
}

