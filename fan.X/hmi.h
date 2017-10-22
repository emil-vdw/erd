#ifndef HMI_H
#define HMI_H

#define RS PORTCbits.RC0
#define EN PORTCbits.RC1

void display_temp(void);
void display_light(void);
void display_speed(void);
void display_mode(void);
void hmi_out(void);
void delay(int x);

#endif