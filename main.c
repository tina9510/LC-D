#include <p18f4520.h>
#include "ConfigRegs.h"
/*
 configuration bits
 */
#define LCD_PORT     PORTD
#define LCD_DDR      TRISD
#define LCD_PWR      PORTDbits.RD7                      // LCD power pin
#define LCD_EN       PORTDbits.RD6                      // LCD enable
#define LCD_RW       PORTDbits.RD5                      // LCD read/write line
#define LCD_RS       PORTDbits.RD4                      // LCD register select line
/*
 useful command
 */
#define LCD_CLEAR           0x01
#define LCD_CURSOR_L        0x10
#define LCD_CURSOR_R        0x14
#define LCD_DISP_L          0x18
#define LCD_DISP_R          0x1C
#define LCD_CURSOR_OFF      0x0C
#define LCD_CURSOR_ON       0x0E
#define LCD_CURSOR_BLINK    0x0F
#define LCD_CURSOR_LINE1    0x80
#define LCD_CURSOR_LINE2    0xC0
#define LCD_4bit_2Line_5X8  0x28
#define LCD_INC_NOSHIFT     0x06
#define LCD_DIS1_CUR0_BLI0  0x0C
/*useful macro function*/
#define LCDLine1()          LCD_CMD_WRT(LCD_CURSOR_LINE1)       
#define LCDLine2()          LCD_CMD_WRT(LCD_CURSOR_LINE2)  
#define Shift_cursor_l()    LCD_CMD_WRT(LCD_CURSOR_L)    
#define Shift_cursor_r()    LCD_CMD_WRT(LCD_CURSOR_R)    
#define Cursor_on()         LCD_CMD_WRT(LCD_CURSOR_ON)    
#define DisplayClr()        LCD_CMD_WRT(LCD_CLEAR)    

/*funciton prototype*/
void LCD_CMD_WRT(unsigned char);
void LCD_TXT_WRT(unsigned char);
void LCD_putc(char data);
void LCD_puts(const rom far char *);
void LCD_init(void);
void LCD_Delay(unsigned int);

void main(void){
    LCD_init();
    LCDLine1();
    LCD_puts("KRONOS OF");
    LCDLine2();
    LCD_puts("HOUSE GROSVENOR");
    
    while(1);
}

void LCD_init(void){
    LCD_PORT = 0;
    LCD_DDR = 0;
    LCD_PWR = 1;    
    LCD_CMD_WRT(0x33);
    LCD_Delay(1);
    LCD_CMD_WRT(0x32);
    LCD_Delay(1);
    LCD_CMD_WRT(LCD_4bit_2Line_5X8);  //Function set to four bit data length 2 line, 5 x 7 dot format
    LCD_Delay(1);
    LCD_CMD_WRT(LCD_INC_NOSHIFT);     //entry mode set, increment, no shift
    LCD_Delay(1);
    LCD_CMD_WRT(LCD_DIS1_CUR0_BLI0);  //Display set, disp on, cursor on, blink off
    DisplayClr();
    return;
}

void LCD_CMD_WRT(unsigned char cmd){
    
    unsigned char x;
    
    x = (cmd & 0xF0) >> 4;
    LCD_PORT =LCD_PORT & 0xF0;
    LCD_PORT = LCD_PORT | x;
    LCD_RS = 0;
    LCD_RW = 0;
    LCD_EN = 1;
    LCD_Delay(2);
    LCD_EN = 0;
    x = (cmd & 0x0F);
    LCD_PORT = LCD_PORT & 0xF0;
    LCD_PORT = LCD_PORT | x;
    LCD_EN = 1;
    LCD_Delay(2);
    LCD_EN = 0;
}

void LCD_TXT_WRT(unsigned char data){
    
    unsigned char x;
    
    x = (data & 0xF0) >> 4;
    LCD_PORT =LCD_PORT & 0xF0;
    LCD_PORT = LCD_PORT | x;
    LCD_RS = 1;
    LCD_RW = 0;
    LCD_EN = 1;
    LCD_Delay(2);
    LCD_EN = 0;
    x = (data & 0x0F);
    LCD_PORT = LCD_PORT & 0xF0;
    LCD_PORT = LCD_PORT | x;
    LCD_EN = 1;
    LCD_Delay(2);
    LCD_EN = 0;
}

void LCD_putc(char data){
    LCD_TXT_WRT(data);
    LCD_Delay(4);
}

void LCD_puts(const rom far char *Str){
    char tcount;
    while(Str[tcount]){
        LCD_TXT_WRT(Str[tcount++]);
    }        
}

void LCD_Delay(unsigned int itime)
  {
    unsigned int i; unsigned int j;
    for(i=0;i<itime;i++)
      for(j=0;j<1000;j++);
 }