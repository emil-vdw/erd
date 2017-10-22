/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H

#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <string.h>

void serialSetup()
{
    TRISCbits.RC6=0;//tx
    TRISCbits.RC7=1;//rx
    TXSTA = 0b00100000;
    RCSTA = 0b10010000;
    
    BAUDCONbits.BRG16 =0;
    
    SPBRG = 12; //Baud rate of 9600 bps
    PIE1bits.RCIE = 1;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
}

void SerialTransmit(const char *buffer)
{
    
//     unsigned int size = strlen(buffer);
//     
//     while(size)
//     {
//         while(TXIF);
//          TXREG = *buffer;          
// 
//        buffer++;                   
//        size--;         
//     }
//     
//     while( !TRMT);   
    int i;
    for(i=0;buffer[i]!='\0';i++)
      UART_Write(buffer[i]);

}

void UART_Write(char data)
{
  while(!TXSTAbits.TRMT && PIR1bits.TXIF == 0);
  TXREG = data;
}

char SerialReceive()
{
   while(!RCIF);
    return RCREG;
}

#endif	/* XC_HEADER_TEMPLATE_H */

