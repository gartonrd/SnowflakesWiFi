/********************************************************
Snowflakes WiFi 
  A2_WriteTest
    write a test pattern into the EEPROM
********************************************************/

//start of patterns ===========================================================================================
//Toggle
//3Reps
uint8_t Toggle3Reps[] =
{
  0x90, 0x03, 0x54, 0x4F, 0x47, 0x47, 0x20, 0x31, 0x73, 0x20
};

//AllDim
//0Reps
uint8_t AllDim0Reps[] =
{
  0x90, 0x00, 0x41, 0x4C, 0x4C, 0x20, 0x44, 0x49, 0x4D, 0x20
};

//Blink
//0Reps
uint8_t Blink0Reps[] =
{
  0x90, 0x00, 0x42, 0x4C, 0x49, 0x4E, 0x4B, 0x20, 0x31, 0x73
};

//Ramp
//0Reps
uint8_t Ramp0Reps[] =
{
  0x90, 0x00, 0x52, 0x41, 0x4D, 0x50, 0x20, 0x34, 0x73, 0x20
};

//Shoot
//0Reps
uint8_t Shoot2Reps[] =
{
  0x90, 0x02, 0x53, 0x48, 0x4F, 0x4F, 0x54, 0x20, 0x20, 0x20
};

//=============================================================================================================

//profiles ====================================================================================================
//Toggle
//0x00  0% (off)
//0x01  100% (on)
uint8_t ToggleProfiles[] =
{  
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x80, 0x01, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//AllDim
//0x00  dim 6.25%
// -    dim
//0x0E  dim 93.75%
//0x0F  dim 100% (on)
uint8_t AllDimProfiles[] =
{
  0x80, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x80, 0x01, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x80, 0x02, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x80, 0x03, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x80, 0x04, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x80, 0x05, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x80, 0x06, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x80, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x80, 0x08, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x80, 0x09, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x80, 0x0A, 0xB0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x80, 0x0B, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x80, 0x0C, 0xD0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x80, 0x0D, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x80, 0x0E, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x80, 0x0F, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//Blink
//0x00  0% (off)
//0x01  blink 1 sec on, 1 sec off = 0x0064
//0x02  blink on once for 3 sec   = 0x012C
//0x03  blink off once for 8 sec  = 0x0320
//0X04  delay 5sec blink 1 sec on, 1sec off = 0x01F4 0x0064
//0x05  delay 9sec, blink on once 3 sec = 0x0384 0x012C
uint8_t BlinkProfiles[] =
{
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x80, 0x01, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00, 0x64,
  0x80, 0x02, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x2C, 0x00, 0x00,
  0x80, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x20,
  0x80, 0x04, 0x40, 0x00, 0x00, 0x00, 0x01, 0xF4, 0x00, 0x64, 0x00, 0x64,
  0x80, 0x05, 0x80, 0x00, 0x00, 0x00, 0x03, 0x84, 0x01, 0x2C, 0x00, 0x00
};

//Ramp
//0x00  0% (off)
//0x01  ramp up   0%   to 100% in 3 sec = 0x012C
//0x02  ramp down 100% to 10%  in 3 sec = 0x012C
//0x03  ramp up   10%  to 30%  in 3 sec = 0x012C
//0x04  ramp down 30%  to 0%   in 3 sec = 0x012C
uint8_t RampProfiles[] =
{
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x80, 0x01, 0x00, 0xFF, 0x01, 0x2C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x80, 0x02, 0xFF, 0x19, 0x01, 0x2C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x80, 0x03, 0x19, 0x4B, 0x01, 0x2C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x80, 0x04, 0x4B, 0x00, 0x01, 0x2C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//Shoot
//0x00  0% (off)
//0x01  100% (on)
uint8_t ShootProfiles[] =
{
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x80, 0x01, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//=============================================================================================================

//patterns ====================================================================================================
//Toggle
//0x0064 = 1sec
uint8_t TogglePattern[] =
{
  0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64,
  0x81, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x64
};

//All Dim
//0x012C = 3sec
uint8_t AllDimPattern[] =
{
  0x81,  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x01, 0x2C
};

//Blink
//0x044C = 11sec
//0X060E - 15.5 sec
uint8_t BlinkPattern[] =
{
  0x81, 0x01, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x4C,
  0x81, 0xFF, 0x04, 0x05, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x06, 0x0E
};

//Ramp
//0x0190 = 4s
uint8_t RampPattern[] =
{
  0x81, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x90,
  0x81, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x01, 0x90,
  0x81, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x01, 0x90,
  0x81, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x01, 0x90
};

//Shoot
//0x0064 = 1sec
//0x0001 = 10ms
uint8_t ShootPattern[] =
{
  0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64,
  0x81, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x01,
  0x81, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x01,
  0x81, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0xFF, 0x00, 0x01,
  0x81, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0xFF, 0xFF, 0x00, 0x01,
  0x81, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x01,
  0x81, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x01,
  0x81, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x01,
  0x81, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x01,
  0x81, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x01,
  0x81, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x01,
  0x81, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x01,
  0x81, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x01,
  0x81, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x01,
  0x81, 0xFF, 0xFF, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x01,
  0x81, 0xFF, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x01,
  0x81, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x01,
  0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64
};

//=============================================================================================================

//end of table ================================================================================================
uint8_t EndTable[] =
{
  0x91
}; 
//=============================================================================================================

//table =======================================================================================================
uint8_t *Data[] = 
{
  Toggle3Reps,
  ToggleProfiles,
  TogglePattern,
    AllDim0Reps,
    AllDimProfiles,
    AllDimPattern,

  Toggle3Reps,
  ToggleProfiles,
  TogglePattern,
    Blink0Reps,
    BlinkProfiles,
    BlinkPattern,

  Toggle3Reps,
  ToggleProfiles,
  TogglePattern,
    Ramp0Reps,
    RampProfiles,
    RampPattern,

  Shoot2Reps,
  ShootProfiles,
  ShootPattern,

  EndTable
};

uint32_t Length[] =
{
  sizeof(Toggle3Reps),
  sizeof(ToggleProfiles),
  sizeof(TogglePattern),
    sizeof(AllDim0Reps),
    sizeof(AllDimProfiles),
    sizeof(AllDimPattern),

  sizeof(Toggle3Reps),
  sizeof(ToggleProfiles),
  sizeof(TogglePattern),
    sizeof(Blink0Reps),
    sizeof(BlinkProfiles),
    sizeof(BlinkPattern),

  sizeof(Toggle3Reps),
  sizeof(ToggleProfiles),
  sizeof(TogglePattern),
    sizeof(Ramp0Reps),
    sizeof(RampProfiles),
    sizeof(RampPattern),

  sizeof(Shoot2Reps),
  sizeof(ShootProfiles),
  sizeof(ShootPattern),

  sizeof(EndTable)
};
//=============================================================================================================

//code ========================================================================================================
void WriteTestData(void)
{
  uint8_t Index;
  uint32_t WriteAddress;
  
  //loop to initialize tables
  Index = 0;
  WriteAddress = StartTableAddress;
  while(Index < (sizeof(Data)) / (sizeof(uint8_t *)))
  {
    // write to EEPROM
    WriteEeprom(WriteAddress, Data[Index], Length[Index]);

    //next
    WriteAddress += Length[Index];
    Index +=1;
  }

  //print message
  Serial.print(GetWriteTestDataDone());
}
//=============================================================================================================


