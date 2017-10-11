#include <p18f452.h>
#include "ConfigRegs.h"

#define LCD_PORT     PORTD
#define LCD_PWR      PORTDbits.RD7                      // LCD power pin
#define LCD_EN       PORTDbits.RD6                      // LCD enable
#define LCD_RW       PORTDbits.RD5                      // LCD read/write line
#define LCD_RS       PORTDbits.RD4                      // LCD register select line

//// single bit for selecting command register or data register
//#define instr        0
//#define data         1
//
///**
// Section: Macro Definitions
//*/
//// set up the timing for the LCD delays
//#define LCD_delay           5     // ~5mS
//#define LCD_Startup         15    // ~15mS
//
//// Command set for Hitachi 44780U LCD display controller
//#define LCD_CLEAR           0x01
//#define LCD_HOME            0x02
//#define LCD_CURSOR_BACK     0x10
//#define LCD_CURSOR_FWD      0x14
//#define LCD_PAN_LEFT        0x18
//#define LCD_PAN_RIGHT       0x1C
//#define LCD_CURSOR_OFF      0x0C
//#define LCD_CURSOR_ON       0x0E
//#define LCD_CURSOR_BLINK    0x0F
//#define LCD_CURSOR_LINE2    0xC0

//// display controller setup commands from page 46 of Hitachi datasheet
//#define FUNCTION_SET        0x28                         // 4 bit interface, 2 lines, 5x8 font
//#define ENTRY_MODE          0x06                         // increment mode
//#define DISPLAY_SETUP       0x0C                         // display on, cursor off, blink off

//#define LCDLine1()          LCDPutCmd(LCD_HOME)          // legacy support
//#define LCDLine2()          LCDPutCmd(LCD_CURSOR_LINE2)  // legacy support
//#define shift_cursor()      LCDPutCmd(LCD_CURSOR_FWD)    // legacy support
//#define cursor_on()         LCDPutCmd(LCD_CURSOR_ON)     // legacy support
//#define DisplayClr()        LCDPutCmd(LCD_CLEAR)         // Legacy support



void CMDWRT4(unsigned char);
void DATWRT4(unsigned char);
void MSDelay(unsigned int);
void LCDinit(void);
void LCDPutChar(int);
void LCDWriteStr(const rom far char *);

void main(void) {
    unsigned char *str = "Grosvenor";

    LCDinit();
    LCDWriteStr(str);
    CMDWRT4(0xC0);
    LCDWriteStr("Hail KRONOS");
    while(1);
}

void LCDinit(void) 
{
        int i;
        // clear latches before enabling TRIS bits
        LCD_PORT = 0;
        TRISD = 0x00;
        // power up the LCD
        LCD_PWR = 1;
        MSDelay(5);
 //       CMDWRT4(0x33);   //reset sequence provided by data sheet
 //       MSDelay(1);
        CMDWRT4(0x32);   //reset sequence provided by data sheet
        MSDelay(1);
        CMDWRT4(0x28);   //Function set to four bit data length
                                         //2 line, 5 x 7 dot format
        MSDelay(1);
        CMDWRT4(0x06);  //entry mode set, increment, no shift
        MSDelay(1);
        CMDWRT4(0x0E);  //Display set, disp on, cursor on, blink off
        MSDelay(1);
        CMDWRT4(0x01);  //Clear display
        MSDelay(1);
        CMDWRT4(0x80);  //set start posistion, home position
        MSDelay(1);

}
void CMDWRT4(unsigned char command)
{
        unsigned char x;
        
        x = (command & 0xF0) >> 4;         //shift high nibble to center of byte for Pk5-Pk2
        LCD_PORT =LCD_PORT & 0xF0;          //clear 0-3
        LCD_PORT = LCD_PORT | x;          //sends high nibble to PORTK
        LCD_RS = 0;                     //set RS to command (RS=0)
        LCD_RW = 0;
        LCD_EN = 1;                     //rais enable
        MSDelay(5);
        LCD_EN = 0;                     //Drop enable to capture command
        x = (command & 0x0F);            // shift low nibble to center of byte for Pk5-Pk2
        LCD_PORT = LCD_PORT & 0xF0;         //clear bits Pk5-Pk2
        LCD_PORT = LCD_PORT | x;             //send low nibble to PORTK
        LCD_EN = 1;                     //rais enable
        MSDelay(5);
        LCD_EN = 0;                     //Drop enable to capture command
}

void DATWRT4(unsigned char data){
     unsigned char x;
        
        x = (data & 0xF0) >> 4;         //shift high nibble to center of byte for Pk5-Pk2
        LCD_PORT =LCD_PORT & 0xF0;          //clear 0-3
        LCD_PORT = LCD_PORT | x;          //sends high nibble to PORTK
        LCD_RS = 1;                     //set RS to command (RS=0)
        LCD_RW = 0;
        LCD_EN = 1;                     //rais enable
        MSDelay(5);
        LCD_EN = 0;                     //Drop enable to capture command
                     //wait
        x = (data & 0x0F);            // shift low nibble to center of byte for Pk5-Pk2
        LCD_PORT = LCD_PORT & 0xF0;         //clear bits Pk5-Pk2
        LCD_PORT = LCD_PORT | x;             //send low nibble to PORTK
        LCD_EN = 1;                     //rais enable
        MSDelay(5);
        LCD_EN = 0;                     //Drop enable to capture command

}


void LCDPutChar(int Data)
{
  DATWRT4(Data);              // Go output the character to the display
  MSDelay(1);                          // Wait until it's finished
}

void LCDWriteStr(const rom far char  *Str)
{
  int i = 0;                                     // Char index buffer

  while (Str[i])                                   // While string not finished
    LCDPutChar(Str[i++]);                          // Go display current char
}



void MSDelay(unsigned int itime)
  {
    unsigned int i; unsigned int j;
    for(i=0;i<itime;i++)
      for(j=0;j<1000;j++);
 }
