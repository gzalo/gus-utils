#include "lcd.h"

static const uint16_t CRC_TAB_L[]={
0x0000, 0xC0C1, 0xC181, 0x0140,
0xC301, 0x03C0, 0x0280, 0xC241,
0xC601, 0x06C0, 0x0780, 0xC741,
0x0500, 0xC5C1, 0xC481, 0x0440
};

static const uint16_t CRC_TAB_H[]={
0x0000, 0xCC01, 0xD801, 0x1400,
0xF001, 0x3C00, 0x2800, 0xE401,
0xA001, 0x6C00, 0x7800, 0xB401,
0x5000, 0x9C01, 0x8801, 0x4400
};

uint16_t CRC16(uint16_t BreakPoint, uint8_t *Buffer, uint32_t Length){
   uint16_t CRC_Value;
   uint8_t Dat;
   CRC_Value = BreakPoint;
   while(Length--){
      Dat = *Buffer++;
      Dat ^= CRC_Value;
      CRC_Value >>= 8;
      CRC_Value ^= CRC_TAB_L[Dat & 0x0F];
      CRC_Value ^= CRC_TAB_H[(Dat >> 4)& 0x0F];
   }
   return CRC_Value;
}

uint8_t uartSendByte(uint8_t val){
   return putc(val, SGUS);
}

uint8_t lcdSendFrame(uint8_t *data, uint8_t len){
   uartSendByte(LCD_FRAME_HEADER_1);
   uartSendByte(LCD_FRAME_HEADER_2);
   
   if(LCD_USE_CRC){
      uartSendByte(len+2); //Add checksum
   }else{
      uartSendByte(len); //NO checksum
   }
   
   uint8_t i;
   for(i=0;i<len;i++)
      uartSendByte(data[i]);
   
   if(LCD_USE_CRC){
      //MSB CHECKSUM!
      uint16_t crc = CRC16(LCD_CRC_BREAKPOINT, data, len);
   
      //Send CRC 
      //MSB
      uartSendByte(crc>>8);
      //LSB
      uartSendByte(crc&0xFF);
   }
   
   //Response should be 
   //FRAME + 0x02 + DATA + 0xFF (if OK, 0 otherwise) + CRC
   return 0;
}

uint8_t lcdWriteReg8(uint8_t addr, uint8_t data){
   uint8_t frameData[3];
   frameData[0] = 0x80;
   frameData[1] = addr;
   frameData[2] = data;
   return lcdSendFrame(frameData, 3);
}

uint8_t lcdWriteReg16(uint8_t addr, uint16_t data){
   uint8_t frameData[4];
   frameData[0] = 0x80;
   frameData[1] = addr;
   frameData[2] = data>>8;
   frameData[3] = data&0xFF;
   return lcdSendFrame(frameData, 4);
}

uint8_t lcdWriteRegMultiple(uint8_t addr, uint8_t *data, uint8_t len){
   uint8_t frameData[LCD_MAX_MULTIPLE_WRITES];
   frameData[0] = 0x80;
   frameData[1] = addr;
   
   if(len>=LCD_MAX_MULTIPLE_WRITES/2)
      return 1;
   
   int i;
   for(i=0;i<len;i++)
      frameData[i+2] = data[i];
   
   return lcdSendFrame(frameData, len+2);
}

uint8_t lcdReadReg(uint8_t addr, uint8_t *data, uint8_t len){
   uint8_t frameData[3];
   frameData[0] = 0x81;
   frameData[1] = addr;
   frameData[2] = len;
   
   if(len > LCD_MAX_MULTIPLE_WRITES/2)
      return 1;
   
   return lcdSendFrame(frameData, 3);
}

uint8_t lcdWriteSRAM(uint16_t addr, uint16_t *data, uint8_t len){
   uint8_t frameData[LCD_MAX_MULTIPLE_WRITES];
   frameData[0] = 0x82;
   frameData[1] = addr>>8;
   frameData[2] = addr&0xFF; 
   
   if(len>=100)
      return 1;
   
   int i;
   for(i=0;i<len;i++){
      frameData[i*2+3] = data[i]>>8;
      frameData[i*2+4] = data[i]&0xFF;
   }
   
   return lcdSendFrame(frameData, 3+2*len);
}

uint8_t lcdWriteSRAMSingle(uint16_t addr, uint16_t val){
   uint8_t frameData[5];
   frameData[0] = 0x82;
   frameData[1] = addr>>8;
   frameData[2] = addr&0xFF;
   frameData[3] = val>>8;
   frameData[4] = val&0xFF;
   
   return lcdSendFrame(frameData, 5);
}

uint8_t lcdWriteSRAMDouble(uint16_t addr, uint16_t val1, uint16_t val2){
   uint8_t frameData[7];
   frameData[0] = 0x82;
   frameData[1] = addr>>8;
   frameData[2] = addr&0xFF;
   frameData[3] = val1>>8;
   frameData[4] = val1&0xFF;
   frameData[5] = val2>>8;
   frameData[6] = val2&0xFF;

   return lcdSendFrame(frameData, 7);
}

uint8_t lcdReadSRAM(uint16_t addr, uint8_t len){
   uint8_t frameData[4];
   frameData[0] = 0x83;
   frameData[1] = addr>>8;
   frameData[2] = addr&0xFF;
   frameData[3] = len;   
   
   if(len>=128)
      return 1;
   
   return lcdSendFrame(frameData, 4);
}

uint8_t lcdChangePic(uint16_t picId){
   return lcdWriteSRAMDouble(0x84, 0x5A01, picId);
   // SGUS ONLY => return lcdWriteReg16(0x03, picId);
}

/* SGUS ONLY */
uint8_t lcdSendKeyCode(uint8_t keyCode){
   return lcdWriteReg8(0x4F, keyCode);
}

//Buzztime unit is 10 ms. Range 0-255 (0-2.550s)
uint8_t lcdActivateBuzzer(uint8_t buzzTime){
   return lcdWriteReg8(0x02, buzzTime);
}

//Level is 0-0x40 (0-64)
uint8_t lcdChangeBacklight(uint8_t level){
   return lcdWriteReg8(0x01, level);
}

uint8_t lcdCopyPaste(uint16_t vp, uint16_t picId, uint16_t xCopyLeft, uint16_t yCopyTop, uint16_t xCopyRight, uint16_t yCopyBottom,  uint16_t xPaste, uint16_t yPaste){
   uint16_t data[9];
   data[0] = 0x0006;
   data[1] = 0x0001;
   data[2] = picId;
   data[3] = xCopyLeft;
   data[4] = yCopyTop;
   data[5] = xCopyRight;
   data[6] = yCopyBottom;
   data[7] = xPaste;
   data[8] = yPaste;
   
   return lcdWriteSRAM(vp, data, 9);
}

volatile uint8_t lcdReadBuffer[LCD_READ_BUFFER_SIZE]; 
volatile uint8_t lcdReadBufferHead = 0;
volatile uint8_t lcdReadBufferTail = 0;

volatile uint16_t lcdReadBytes = 0;
volatile uint16_t lcdRemainingBytes = 0;
volatile uint8_t lcdReadFinished = 0;

#int_rda
void lcdRxReady(){
   // On serial port rx interrupt
   lcdReadBuffer[lcdReadBufferHead] = getc(SGUS);
   lcdReadBufferHead++;
   lcdReadBufferHead%=LCD_READ_BUFFER_SIZE;

   clear_interrupt(INT_RDA);
}

uint8_t lcdLocalBuffer[16];

uint8_t lcdGetData(uint16_t *addr, uint16_t *value){
   
   while(lcdReadBufferHead != lcdReadBufferTail){ // New characters have arrived

      if(lcdReadBytes == 0){
         uint8_t readByte = lcdReadBuffer[lcdReadBufferTail];
         lcdReadBufferTail++;
         lcdReadBufferTail%=LCD_READ_BUFFER_SIZE;
         
         if(readByte == LCD_FRAME_HEADER_1) // Continue only if the header matches expected value
            lcdReadBytes = 1; 
         
      }else if(lcdReadBytes == 1){
         uint8_t readByte = lcdReadBuffer[lcdReadBufferTail];
         lcdReadBufferTail++;
         lcdReadBufferTail%=LCD_READ_BUFFER_SIZE;
         
         if(readByte == LCD_FRAME_HEADER_2) // Continue only if the header matches expected value
            lcdReadBytes = 2;
            
      }else if(lcdReadBytes == 2){
         lcdRemainingBytes = lcdReadBuffer[lcdReadBufferTail]; 
         lcdReadBufferTail++;
         lcdReadBufferTail%=LCD_READ_BUFFER_SIZE;

         lcdReadBytes = 3;      
      }else{
         lcdLocalBuffer[lcdReadBytes-3] = lcdReadBuffer[lcdReadBufferTail];
         lcdReadBufferTail++;
         lcdReadBufferTail%=LCD_READ_BUFFER_SIZE;

         lcdReadBytes++;

         if(lcdReadBytes == lcdRemainingBytes+3){ // Just read every expected byte
            lcdReadBytes = 0;
            lcdReadFinished = 1;

            if(lcdLocalBuffer[0] == 0x81){
               // ADDR = lcdReadBuffer[4]
               // LEN = lcdReadBuffer[5]
               // DATA = lcdReadBuffer[6]...
               // Read register (not implemented yet)
            }else if(lcdLocalBuffer[0] == 0x83){
               // ADDR_H = lcdReadBuffer[4]
               // ADDR_L = lcdReadBuffer[5]
               // LEN = lcdReadBuffer[6]
               // DATA = lcdReadBuffer[7]...
               *addr = lcdLocalBuffer[1];
               *addr <<= 8;
               *addr |= lcdLocalBuffer[2];
               
               // len = lcdLocalBuffer[3]

               *value = lcdLocalBuffer[4];
               *value <<= 8;
               *value |= lcdLocalBuffer[5];
               return 1;
            }

         }
      }
   }

   return 0;
}

// Example of sent data (auto upload) when a button gets pressed:
// 5A A5 06 83 [00 20] 01 [00 00]
