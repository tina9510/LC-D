#include <p18f4520.h>
#include "ConfigRegs.h"
/*
 configuration bits
 */
#define LCD_PORT     PORTD                              // LCD - microcontroller port D 
#define LCD_DDR      TRISD                              // data direction register (called 'TRISD' in this microcontroller)
#define LCD_PWR      PORTDbits.RD7                      // LCD power pin
#define LCD_EN       PORTDbits.RD6                      // LCD enable
#define LCD_RW       PORTDbits.RD5                      // LCD read/write line
#define LCD_RS       PORTDbits.RD4                      // LCD register select line
/*
 useful command
 */
#define LCD_CLEAR           0x01      //clears display and returns cursor to the home position(address 0)
#define LCD_CURSOR_L        0x10      //set cursor-move left
#define LCD_CURSOR_R        0x14      //set cursor-move right
#define LCD_DISP_L          0x18      //display-shift left
#define LCD_DISP_R          0x1C      //display-shift right
#define LCD_CURSOR_OFF      0x0C      //sets all display on and cursor off
#define LCD_CURSOR_ON       0x0E      //sets all display on and cursor on
#define LCD_CURSOR_BLINK    0x0F      //sets all display on, cursor on and cursor blink on 
#define LCD_CURSOR_LINE1    0x80      //first line
#define LCD_CURSOR_LINE2    0xC0      //second line
#define LCD_4bit_2Line_5X7  0x28      //4-bit interface, 2 lines, 5*7 dots 
#define LCD_INC_NOSHIFT     0x06      //increment cursor position, no display shift (These operations are performed during data read/write)
#define LCD_DIS1_CUR0_BLI0  0x0C      //sets all display on, cursor off and  cursor blink off

/*useful macro function*/
#define LCDLine1()          LCD_CMD_WRT(LCD_CURSOR_LINE1)       //LCD command - write - first line
#define LCDLine2()          LCD_CMD_WRT(LCD_CURSOR_LINE2)       //LCD command - write - second line
#define Shift_cursor_l()    LCD_CMD_WRT(LCD_CURSOR_L)    //LCD command - cursor move left
#define Shift_cursor_r()    LCD_CMD_WRT(LCD_CURSOR_R)    //LCD command - cursor move right
#define Cursor_on()         LCD_CMD_WRT(LCD_CURSOR_ON)    //LCD command - cursor on
#define DisplayClr()        LCD_CMD_WRT(LCD_CLEAR)        //LCD command - clear LCD

/*funciton prototype*/
void LCD_CMD_WRT(unsigned char);
void LCD_TXT_WRT(unsigned char);
void LCD_putc(char data); //print character
void LCD_puts(const rom far char *); //print string
void LCD_init(void);  //initialize LCD
void LCD_Delay(unsigned int);  

void main(void){
    LCD_init();                      //initialise LCD
    LCDLine1();                     //move cursor to the 1st line of LCD
    LCD_puts("KRONOS OF");         //print string
    LCDLine2();                   //move cursor to the 2nd line of LCD
    LCD_puts("HOUSE GROSVENOR"); //print string
    
    while(1);
}

void LCD_init(void){
    LCD_PORT = 0;                     //clear port D 
    LCD_DDR = 0;                      //set DDR to 0, configure as output
    LCD_Delay(1);
 
    //activate LCD by sending 33 & 32
    LCD_CMD_WRT(0x33);                
    LCD_CMD_WRT(0x32);
    LCD_Delay(1);
 
    LCD_CMD_WRT(LCD_4bit_2Line_5X7);  //Function set to four bit data length 2 line, 5 x 7 dot format
    LCD_Delay(1);
 
    LCD_CMD_WRT(LCD_INC_NOSHIFT);     //entry mode set, increment, no display shift
    LCD_Delay(1);
 
    LCD_CMD_WRT(LCD_DIS1_CUR0_BLI0);  //Display set, disp on, cursor on, blink off
    DisplayClr();
 
    return;
}

/************************************************************/
/****LCD command function - sending command to LCD display***/
/************************************************************/
void LCD_CMD_WRT(unsigned char cmd){
    
    unsigned char x;
   /***split cmd into 2 parts, each of them has 4-bits.The aim is to save the pins***/
    /*4 high significant bits*/
    x = (cmd & 0xF0) >> 4; 
    LCD_PORT = LCD_PORT & 0xF0;
    LCD_PORT = LCD_PORT | x;
    LCD_RS = 0;  //send command
    LCD_RW = 0;
    /*enable LCD to receive command*/
    LCD_EN = 1;
    LCD_Delay(2);
    LCD_EN = 0;   //disable LCD to manage command
 
    /*4 low significant bits*/
    x = (cmd & 0x0F);
    LCD_PORT = LCD_PORT & 0xF0;
    LCD_PORT = LCD_PORT | x;
    /*enable LCD to receive command*/
    LCD_EN = 1;
    LCD_Delay(2);
    LCD_EN = 0;    //disable LCD to manage command
};

/************************************************************/
/****LCD transmit function - writing data to LCD display*****/
/************************************************************/
void LCD_TXT_WRT(unsigned char data){
    
    unsigned char x;
   /***split cmd into 2 parts, each of them has 4-bits.The aim is to save the pins***/
    /*4 high significant bits*/
    x = (data & 0xF0) >> 4;
    LCD_PORT =LCD_PORT & 0xF0;
    LCD_PORT = LCD_PORT | x;
    LCD_RS = 1;  //register status, send data=1, send command=0
    LCD_RW = 0; //set to 0, write to LCD; (set to 1, read from LED)
    /*enable LCD to receive command*/
    LCD_EN = 1;
    LCD_Delay(2);
    LCD_EN = 0;   //disable LCD to manage command
 
    /*4 low significant bits*/
    x = (data & 0x0F);
    LCD_PORT = LCD_PORT & 0xF0;
    LCD_PORT = LCD_PORT | x;
    /*enable LCD to receive command*/
    LCD_EN = 1;
    LCD_Delay(2);
    LCD_EN = 0;    //disable LCD to manage command
}

/************************************************************/
/**********put character / string in LCD ********************/
/************************************************************/
void LCD_putc(char data){
    LCD_TXT_WRT(data);
    LCD_Delay(4);
}
void LCD_puts(const rom far char *Str){
    char tcount = 0; //index
    while(Str[tcount]){  
        LCD_TXT_WRT(Str[tcount++]); //increment pointer, transmit string 
    }        
}

/************************************************************/
/**********************LCD delay*****************************/
/************************************************************/
void LCD_Delay(unsigned int itime)
  {
    unsigned int i; unsigned int j;
    for(i=0;i<itime;i++)
      for(j=0;j<1000;j++);
 }
