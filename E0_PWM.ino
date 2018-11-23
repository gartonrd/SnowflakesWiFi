/********************************************************
Snowflakes WiFi 
  E0_PWM
    utilities to read/write the PWM(PCA9685) chip
    based on PWM routines from various examples
********************************************************/

void InitializePWM(void)
{
  uint8_t Mode2;
  
  //set update frequency to maximum rate
  WritePWMRegister(0x00, 0x10);  //go to sleep
  WritePWMRegister(0xFE, 0x03);  //set prescaler
  WritePWMRegister(0x00, 0x00);  //wake up
  delayMicroseconds(500);        //wait for oscillator to stabilize

  //set mode 1 register
  //  clear restart
  //  internal clock
  //  auto increment
  //  awake
  //  subaddresses disabled
  WritePWMRegister(0x00, 0xA0);

  //set mode 2 register
  if(tolower(AcBoard[0]) == 'y')
  {
    //  outputs not inverted
    //  outputs change on ACK
    //  outputs open drain
    //  outputs off when disabled
    Mode2 = 0x08;      
  }
  else
  {
    //  outputs inverted
    //  outputs change on ACK
    //  outputs open drain
    //  outputs off when disabled
    Mode2 = 0x18;       
  }

  WritePWMRegister(0x01, Mode2);

  AllPWMOff();
}

void AllPWMOff(void)
{
  WritePWMRegister(0xFA, 0x00);  //all on low
  WritePWMRegister(0xFB, 0x00);  //all on hi
  WritePWMRegister(0xFC, 0x00);  //all off low
  WritePWMRegister(0xFD, 0x10);  //all off hi
}

void WriteAllPWM(uint8_t *Data, uint32_t Length)
{
  uint32_t Index;
  uint8_t ChunkCount;
    
  //loop to write all bytes
  Index = 0;
  while(Index < Length)
  {
    //write device address
    Wire.beginTransmission(0x40);
  
    //write register number
    Wire.write(0x06 + Index);

    //loop to write up to 30 bytes of data
    //wire buffer is only 30 bytes
    ChunkCount = 30;
    while((Index < Length)  &&  (ChunkCount > 0))
    {
      Wire.write(Data[Index]);
      ChunkCount -= 1;
      Index += 1;
    }

    Wire.endTransmission(); 

    //delay at least 4.7 microseconds before next start transmission
    delayMicroseconds(5);
  }
}

void WritePWMChannel(uint8_t ChannelNumber, uint16_t TimeOn, uint16_t TimeOff)
{
  //write device address
  Wire.beginTransmission(0x40);
 
  //write channel number
  Wire.write((0x06 + (ChannelNumber*4)));

  //write Time On
  Wire.write(U16ToU80(TimeOn));
  Wire.write(U16ToU81(TimeOn));

  //write Time Off
  Wire.write(U16ToU80(TimeOff));
  Wire.write(U16ToU81(TimeOff));

  Wire.endTransmission();

  //delay at least 4.7 microseconds before next start transmission
  delayMicroseconds(5);
}

void WritePWMRegister(uint8_t RegisterNumber, uint8_t Value)
{
  //write device address
  Wire.beginTransmission(0x40);
  
  //write register number
  Wire.write(RegisterNumber);

  //write one byte of data
  Wire.write(Value);

  Wire.endTransmission();

  //delay at least 4.7 microseconds before next start transmission
  delayMicroseconds(5);
}

uint8_t ReadPWMregister(uint8_t RegisterNumber)
{
  uint8_t Value;
  
  //write device address
  Wire.beginTransmission(0x40);
  
  Wire.write(RegisterNumber);
  Wire.endTransmission();

  //delay at least 4.7 microseconds before next start transmission
  delayMicroseconds(5);
  
  //request read from device
  Wire.requestFrom(0x40, 1);

  Value = Wire.read();

  //delay at least 4.7 microseconds before next start transmission
  delayMicroseconds(5);

  return Value;
}

