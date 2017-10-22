#ifndef LCD_H
#define LCD_H

void InitLCD(void);
void WriteLCD( unsigned char* message ) ;
void WriteCtrl( unsigned char value ) ;
void WriteData( unsigned char value ) ;
void WriteCommon( unsigned char value ) ;
void clear_lcd(void);
void goto_xy(unsigned char  x,unsigned char y);
void integerToLcd(int num );
void pulse_enable(void);

#endif