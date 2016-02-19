//Snowflakes WiFi 
//  U16 consists of U81, U80
//  U32 consists of U83, U82, U81, U80
//  
//    18Feb2016  Dean Garton
//      pack / unpack 8 bit bytes from 16 and 32 bit words

uint8_t U32ToU83(uint32_t U32)
{
  U32 = U32>>24;
  
  return (uint8_t)U32;
}

uint8_t U32ToU82(uint32_t U32)
{
  U32 = U32>>16;
  
  return (uint8_t)U32;
}

uint8_t U32ToU81(uint32_t U32)
{
  U32 = U32>>8;
  
  return (uint8_t)U32;
}

uint8_t U32ToU80(uint32_t U32)
{
  return (uint8_t)U32;
}

uint8_t U16ToU81(uint32_t U16)
{
  U16 = U16>>8;
  
  return (uint8_t)U16;
}

uint8_t U16ToU80(uint32_t U16)
{
  return (uint8_t)U16;
}

uint16_t U8ToU16(uint8_t U81, uint8_t U80)
{
  uint16_t U16;

  U16 = U81;
  U16 = U16<<8;
  U16 |= U80;
  
  return U16;
}

uint32_t U8ToU32(uint8_t U83, uint8_t U82, uint8_t U81, uint8_t U80)
{
  uint32_t U32;
  
  U32 = U83;
  U32 = U32<<8;
  U32 |= U82;
  U32 = U32<<8;
  U32 |= U81;
  U32 = U32<<8;
  U32 |= U80;
  
  return U32;
}

