//Snowflakes WiFi 
//  DisplayTestPattern()
//  Walk()
//  VersionNumber()
//  
//    11Mar2016  Dean Garton 
//      version 2 
//  
//    18Feb2016  Dean Garton
//      All Off
//      Light each flake, one at a time
//      Display version, flake 1 = LSB
//      All Off

void DisplayTestPattern(uint16_t Delay)
{
  AllPWMOff();

  //walk pattern
  Walk(Delay);

  VersionNumber();
  delay(Delay*2);

  AllPWMOff();
}

void Walk(uint16_t Delay)
{
  uint8_t Index;
    
  WritePWMChannel(0, 0x1000, 0x0000);
  
  delay(Delay);

  //loop to shift through all outputs
  Index = 1;
  while(Index < 16)
  {
    //turn previous channel off
    WritePWMChannel(Index-1, 0x0000, 0x1000);

    //turn next channel on
    WritePWMChannel(Index, 0x1000, 0x0000);

    delay(Delay);

    Index += 1;
  }

  //channel 15 off
  WritePWMChannel(15, 0x0000, 0x1000);
}

void VersionNumber(void)
{
  uint8_t Index;
  uint16_t Mask;
  uint16_t VerNum;
     
  //loop to display version number
  Index = 0;
  Mask = 1;
  VerNum = VERSION;
  
  while(Index < 16)
  {
    //if bit is set in version number
    if((VerNum & Mask) != 0)
    {
      //turn output on
      WritePWMChannel(Index, 0x1000, 0x0000);
    }
    else
    {
      //turn output off
      WritePWMChannel(Index, 0x0000, 0x1000);
    }

    //next
    Index += 1;
    Mask = Mask<<1;
  }
}

