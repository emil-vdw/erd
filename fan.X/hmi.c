/* 
 * File:   hmi.c
 * Author: emil
 *
 * Created on 24 September 2017, 7:15 PM
 */

// PIC18F45K20 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1H
#pragma config FOSC = INTIO67   // Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 18        // Brown Out Reset Voltage bits (VBOR set to 1.8 V nominal)

// CONFIG2H
#pragma config WDTEN = OFF      // Watchdog Timer Enable bit (WDT is controlled by SWDTEN bit of the WDTCON register)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config HFOFST = ON      // HFINTOSC Fast Start-up (HFINTOSC starts clocking the CPU without waiting for the oscillator to stablize.)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = OFF     // Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection Block 0 (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection Block 1 (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection Block 2 (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection Block 3 (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection Block 0 (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection Block 1 (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection Block 2 (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection Block 3 (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection Block 0 (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection Block 1 (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection Block 2 (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection Block 3 (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks)

#include "hmi.h"
#include "sensors.h"
#include "lcd.h"
#include "I2C.h"
#include "serial.h"

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 #pragma interrupt ISR save=section(".tmpdata"),PROD,section("MATH_DATA")

int mode = 0;
int speed = 1;
int state = 1;

void adcToLcd();
void SerialTransmit(const char *buf);
char SerialReceive();
/*
 * 
 */
int main(void) 
{ 
    //*********Serial Communication******************
    
    OSCCONbits.IRCF0 = 0;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.SCS1 =1;
    

    while(1)
    {
     
        while(SerialReceive() != 'u');
        while(!PIR1bits.TXIF); // COMMENT ADDED HERE
         TXREG = 'p';        
        //SerialTransmit("u26101000");

    }
    //*********Serial Communication******************  
    
    
    TRISC = 0;
    TRISD = 0;
    ANSEL = 0;
    READY_TRIS = 1;
    int state_old = 0;
   



    
// 
    EN = 0;
    InitLCD(); 
    hmi_out();
    
    
//    init_i2c(100000);
    delay(250);
//    IQS333_setup();
//    
    init_adc();
    hmi_out();
    
    while(1)
    {
        adcToLcd();
        delay(100);
        
//        get_input();
//        state_old = state;
//        state = get_light();
//        if(state != state_old)
//            hmi_out();
    }
    
    
} 





void adcToLcd()
{
    clear_lcd();
    delay(5);
    ADCON0 = P_ADC;
    adc_poll();
    int result = ADRESH;
    WriteLCD("ADC:");
    integerToLcd(result);
}

void hmi_out(void)
{
    if(!state)
    {
        clear_lcd();
        WriteLCD("Idle");
        return;
    }
    clear_lcd();
    delay(1);
    goto_xy(1,15);
    delay(1);
    display_speed();
//    delay(1);
//    goto_xy(1,0);
//    delay(1);
//    display_light();
    delay(1);
    goto_xy(0,0);
    delay(1);
    display_temp();
    delay(1);
    goto_xy(9,0);
    delay(1);
    display_mode();
    
    
}

void delay(int x)
{
    int i;
    for (x ;x>1;x--)
    {
        for (i=0;i<=110;i++);
    }
}

void display_mode()
{
    if(mode == 0)
        WriteLCD("Auto");
    else
        WriteLCD("Manual");
}

void display_temp(void)
{
    int temp = get_temp();
    integerToLcd(temp);
    WriteData(0b11011111);
    WriteData('C');
}

void display_light(void)
{
    integerToLcd(get_temp());
}

void display_speed(void)
{
    integerToLcd(speed);
}

int get_input(void)
{
    unsigned char data_buffer[12];
    i2c_start();
    i2c_read(IQS333_ADDR, TOUCH, &data_buffer[0], 2);
    i2c_stop();
    
    if(data_buffer[0] == 1 && data_buffer[1] == 0) //up
    {
        if(speed != 10)
            speed += 1;
    }
    else if(data_buffer[0] == 65)//down
    {
        if(speed != 1)
            speed -= 1;
    }
    else if(data_buffer[0] == 129)//state
    {
        state = !state;
    }
    else if(data_buffer[0] == 1 && data_buffer[1] == 1)//mode
    {
        mode = !mode;
    }
    else
    {
        return;
    }
    hmi_out();
    delay(100);
    return;
}


void high_priority interrupt High_Priority_Interrupt(void) 
{
    unsigned char tempChar;
    if(PIR1bits.RCIF)
    {
        tempChar = RCREG;
        switch(tempChar)
        {
            case 'u':
                serialTransmit('i');
                break;
        }
    }
}