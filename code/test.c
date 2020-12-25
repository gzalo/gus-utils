#include "lcd.h"
#include "lcd.c"

uint8_t testVariable = 1234;

#define LCD_BUTTON      (0x1000)
#define LCD_BUTTON_DUMMY (0x0000)
#define LCD_BUTTON_START (0x0001)
#define LCD_BUTTON_STOP (0x0002)

#define LCD_TEST_VARIABLE  (0x1001)

int main(){
   int16_t iteration = 0;

   // Enable interrupts
   enable_interrupts(INT_RDA);   // UART reception interrupt
   enable_interrupts(GLOBAL);

   // Store initial RAM Values
   lcdWriteSRAMSingle(LCD_TEST_VARIABLE, testVariable);
   
   while(1){
      uint16_t readAddr, readData;
      if(lcdGetData(&readAddr, &readData)){
         // Processs LCD reads
         
         switch(readAddr){
            case LCD_BUTTON:
               if(readData == LCD_BUTTON_START){
                  // Start something
               } else if(readData == LCD_BUTTON_STOP){
                  // Stop something
               }
               break;
            case LCD_TEST_VARIABLE:
               // When read succeeds, sync the variable value
               testVariable = readData;
               break;
         }
      }   

      if(iteration >= 1000 == 0){
         iteration = 0;
         // Every second, send command to read from variable
         lcdReadSRAM(LCD_TEST_VARIABLE, 1); 
      }

      delay_ms(1);
   }
   return 0;
}
