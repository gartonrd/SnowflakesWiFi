//Snowflakes WiFi 
// WriteEeprom()
//  handles as many bytes as are available by accounting for:
//    may have up to 4 chips
//    hi memory address bit is part of device address 
//    small wire buffer
//    page boundaries
//    internal write time
// ReadEeprom()
//  handles as many bytes as are available by accounting for:
//    may have up to 4 chips
//    hi memory address bit is part of device address 
//    small wire buffer 
// CalculateEepromDeviceAddress    
//   calculate device address including:
//     may have up to 4 chips
//     hi memory address bit is part of device address
//     
//    18Feb2016  Dean Garton
//      based on EEPROM routines from various examples

void WriteEeprom(uint32_t MemoryAddress, uint8_t *Data, uint32_t Length)
{
  uint32_t Index;
  uint8_t DeviceAddress;
  uint8_t ChunkCount;

  //loop to write all bytes
  Index = 0;
  while(Index < Length)
  {
    //calculate device address
    DeviceAddress = CalculateEepromDeviceAddress(MemoryAddress);
    
    //write device address
    Wire.beginTransmission(DeviceAddress);
    
    //write memory address
    Wire.write(U32ToU81(MemoryAddress));
    Wire.write(U32ToU80(MemoryAddress));

    //write one byte of data
    Wire.write(Data[Index]);
    Index += 1;
    MemoryAddress += 1;
    
    //loop to write up to 29 more bytes of data
    //wire buffer is only 30 bytes
    //do not cross a 256 byte page boundary
    ChunkCount = 29;
    while((Index < Length)  &&  (ChunkCount > 0) && ((MemoryAddress & 0xFF) != 0))
    {
      Wire.write(Data[Index]);
      ChunkCount -= 1;
      Index += 1;
      MemoryAddress += 1;
    }

    //end transmission
    Wire.endTransmission();

    //wait for internal write to complete
    delay(5);
  }
}

void ReadEeprom(uint32_t MemoryAddress, uint8_t *Data, uint32_t Length)
{
  uint32_t Index;
  uint8_t DeviceAddress;
  uint32_t ChunkCount;

  //loop to read all bytes
  Index = 0;
  while(Index < Length)
  {
    //calculate device address
    DeviceAddress = CalculateEepromDeviceAddress(MemoryAddress);
    
    //write device address
    Wire.beginTransmission(DeviceAddress);
    
    //write memory address
    Wire.write(U32ToU81(MemoryAddress));
    Wire.write(U32ToU80(MemoryAddress));

    //end transmission
    Wire.endTransmission();

    //delay at least 4.7 microseconds before next start transmission
    delayMicroseconds(5);

    //limit read to 30 bytes of data at a time
    //wire buffer is only 30 bytes
    ChunkCount = Length - Index;
    if(ChunkCount > 30)
    {
      ChunkCount = 30;
    }
  
    //start read from device
    Wire.requestFrom(DeviceAddress, U32ToU80(ChunkCount));
  
    //loop to read all bytes
    while(ChunkCount > 0)
    {
      if (Wire.available())
      {
        Data[Index] = Wire.read();
      }
      else
      {
        Serial.println("EEPROM read error");
      }
      ChunkCount -= 1;
      Index += 1;
      MemoryAddress += 1;
    }

    //delay at least 4.7 microseconds before next start transmission
    delayMicroseconds(5);
  }
}

uint8_t CalculateEepromDeviceAddress(uint32_t MemoryAddress)
{
  uint8_t DeviceAddress;

  DeviceAddress = U32ToU82(MemoryAddress);
  DeviceAddress |= 0x50;
  
  return DeviceAddress;
}

