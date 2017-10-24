#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>


#ifndef ESP8266_H
#define	ESP8266_H

#ifdef	__cplusplus
extern "C" {
#endif
    
void connect();
char SerialReceive();
void UART1Configure();
void UART2Configure();
char getU2();
void SerialTransmit(const char *buf);
void SerialTransmit2(const char *buf);
void pwm(int dc);
void integration();

#ifdef	__cplusplus
}
#endif

#endif



