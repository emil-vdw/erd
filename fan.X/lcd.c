#include "lcd.h"
#include "hmi.h"
#include <xc.h>

void clear_lcd(void)
{
    WriteCtrl(0x01);
}

void InitLCD(void) 
{ 
//    WriteCtrl(3); 
//    delay(50); 
//    WriteCtrl(3); 
//    delay(10); 
//    WriteCtrl(3); 
//    delay(10); 
//    WriteCtrl(2); 
//    delay(10); 
//    /* Function Set */ 
//    WriteCtrl(2); 
//    delay(10); 
//    WriteCtrl(8); 
//    delay(10); 
//    /* Display OFF */ 
//    WriteCtrl(0); 
//    delay(10); 
//    WriteCtrl(8); 
//    delay(10); 
//    /* Display ON */ 
//    WriteCtrl(0); 
//    delay(10); 
//    WriteCtrl(0x0F); 
//    delay(10); 
//    /* Entry mode */ 
//    WriteCtrl(0); 
//    delay(10); 
//    WriteCtrl(6); 
//    delay(10); 
//    /* Clear Screen */ 
//    WriteCtrl(0); 
//    delay(10); 
//
//    WriteCtrl(1); 
//    delay(100); 
//    /* Cursor home */ 
//    WriteCtrl(0); 
//    delay(10); 
//    WriteCtrl(2); 
//    delay(100);
    
//    backup
        /* Wait a bit after power-up */ 
    
    delay(200); 
    /* Initialize the LCD to 4-bit mode */
    PORTD = 3;
    delay(2);
    pulse_enable();
    delay(25);
    pulse_enable();
    delay(25);
    pulse_enable();
    PORTD = 2;
    delay(2);
    pulse_enable();
    delay(25);
    WriteCtrl(0x08); 
    delay(250);
    WriteCtrl(0x28);
    delay(250);
    /* Function Set */ 
    WriteCtrl(0x14);
    delay(250);
    /* Display OFF */ 
    
    WriteCtrl(0x06);  // cursor shift direction
    delay(250);
    WriteCtrl(0x80);  //set ram address
    delay(250);
    clear_lcd();
    WriteCtrl(0x0C); 
    delay(250);

}

void WriteLCD( unsigned char* message ) 
{
    delay(1);
    unsigned char i; 
    for(i=0; i<20; i++) 
    { 
        if( !message[i] ) 
            break; 
        WriteData(message[i]); 
    } 
} 

void WriteCtrl( unsigned char value ) 
{ 
    RS = 0;
    delay(1); 
    WriteCommon( value ); 
} 

void WriteData( unsigned char value ) 
{ 
    RS = 1; 
    delay(1); 
    WriteCommon( value ); 
} 

void pulse_enable(void)
{
    EN = 1;
    delay(2); 
    EN = 0;
}

void WriteCommon( unsigned char value ) 
{ 
    delay(1); 
//    send first nibble
    PORTD = ((value >> 4) & 0x0F);// P0 = P0 & 0x0F; 
    pulse_enable(); 
    delay(5); 
//    send second nibble
    PORTD  = (value & 0x0F);
    pulse_enable();
    
    delay(1);  
} 



void goto_xy(unsigned char  x,unsigned char y)
{
    if(x<40)
    {
        if(y) x|=0b01000000;
        x|=0b10000000;
        WriteCtrl(x);
    }
}

void integerToLcd(int num )
{
	//	Break down the original number into the thousands, hundreds, tens,
	//	and ones places and then immediately write that value to the LCD
    unsigned char thousands	,hundreds,tens,ones;
    thousands = num / 1000;

    hundreds = ((num - thousands*1000)-1) / 100;

    WriteData( hundreds + 0x30);
    tens = (num%100)/10;	

    WriteData( tens + 0x30);
    ones = num%10;

    WriteData( ones + 0x30);
}

