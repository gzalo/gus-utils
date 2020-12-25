#ifndef LCD_H
#define LCD_H

#include <stdint.h>

#use RS232(STREAM = SGUS, UART1, BAUD = 115200, PARITY = N, BITS = 8, ERRORS, STOP = 1, TIMEOUT = 100)

#define LCD_FRAME_HEADER_1 (0x5A)
#define LCD_FRAME_HEADER_2 (0xA5)
#define LCD_CRC_BREAKPOINT (0xFFFF) 
#define LCD_READ_BUFFER_SIZE (0x40) // 64 words (128 bytes)

#define LCD_USE_CRC (0)
#define LCD_MAX_MULTIPLE_WRITES (64)

void lcdInit();

uint8_t lcdSendFrame(uint8_t *data, uint8_t len);
uint8_t lcdWriteReg8(uint8_t addr, uint8_t data);
uint8_t lcdWriteReg16(uint8_t addr, uint16_t data);
uint8_t lcdWriteRegMultiple(uint8_t addr, uint8_t *data, uint8_t len);
uint8_t lcdReadReg(uint8_t addr, uint8_t *data, uint8_t len);
uint8_t lcdWriteSRAM(uint16_t addr, uint16_t *data, uint8_t len);
uint8_t lcdWriteSRAMSingle(uint16_t addr, uint16_t val);
uint8_t lcdReadSRAM(uint16_t addr, uint8_t len);
uint8_t lcdChangePic(uint16_t picId);
uint8_t lcdSendKeyCode(uint8_t keyCode);
uint8_t lcdActivateBuzzer(uint8_t buzzTime);
uint8_t lcdChangeBacklight(uint8_t level);
uint8_t lcdCopyPaste(uint16_t vp, uint16_t picId, uint16_t xCopyLeft, uint16_t yCopyTop, uint16_t xCopyRight, uint16_t yCopyBottom,  uint16_t xPaste, uint16_t yPaste);
uint8_t lcdGetData(uint16_t *addr, uint16_t *data);

#endif
