//Snowflakes WiFi 
//  GetPatternRecord()
//  
//    18Feb2016  Dean Garton
//      get next pattern record

void GetPatternRecord(void)
{
  uint8_t EndLoop;
  uint32_t Length;
  
  //loop to get record
  EndLoop = 0;
  while(EndLoop == 0)
  {
    //read from eeprom
    ReadPatternRecord();

    //case per record ID
    switch(PatternRecord[0])
    {
      //start of pattern record
      case 0x90:
        //decrement reps
        if(PatternReps > 0)
        {
          PatternReps -= 1;
        }

        //if reps = 0
        if(PatternReps == 0)
        {
          //increment to next pattern
          PatternNumber += 1;
          RecordNumber = 0;
          PatternReps = PatternRecord[1];
          ScaleFactor = PatternRecord[2];
          GetPatternName();
          Length = 11;
          PatternAddress += Length;
          PatternStartAddress = PatternAddress;
        }
        else
        {
          //reset for another rep of the same pattern
          RecordNumber = 0;
          PatternAddress = PatternStartAddress;
        }

        //set flag to print when pattern is executed
        StartOfPattern = 1;
      break;

      //end of table record
      case 0x91:
        //decrement reps
        if(PatternReps > 0)
        {
          PatternReps -= 1;
        }

        //if reps = 0
        if(PatternReps == 0)
        {
          //increment to start of table
          PatternNumber = 0;
          RecordNumber = 0;
          PatternAddress = PatternStartTableAddress;

          //set flag to print when pattern is executed
          EndOfTable = 1;
        }
        else
        {
          //reset for another rep of the same pattern
          RecordNumber = 0;
          PatternAddress = PatternStartAddress;

          //set flag to print when pattern is executed
          StartOfPattern = 1;
        }
      break;

      //pattern record
      case 0x81:
        //increment
        RecordNumber += 1;
        Length = 18;
        PatternAddress += Length;

        //print when pattern is executed
  
        //next
        PatternState += 1;
        EndLoop = 1;
      break;

      //fatal error
      default:
        FatalTableError();
      break;
    }
  }
}
