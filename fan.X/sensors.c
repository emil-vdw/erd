#include <pic18f45k20.h>

#include "sensors.h"
#include "hmi.h"
#include "lcd.h"
#include "math.h"


void init_adc()
{
    TRISAbits.RA0 = 1;
    TRISAbits.RA1 = 1;
    TRISAbits.RA2 = 1;
    TRISAbits.RA3 = 1;
    ANSELbits.ANS0 = 1;
    ANSELbits.ANS1 = 1;
    ANSELbits.ANS2 = 1;
    ANSELbits.ANS3 = 1;
    delay(2);
    ADCON1 = 0;
    delay(2);
    ADCON2 = 0b00101011;
    delay(2);
}

void adc_poll(void)
{
    while(ADCON0bits.GO);
}

int get_temp(void)
{
    return 27;
    ADCON0 = T_ADC;
    adc_poll();
    int result = ADRESH;
    result = ((10000 )/(255 - result))* result;
    int logres = log(result/10000);
    result = 1/(1/25 + (1/3950)*logres);
    logres = 0;
    return result;
}

int get_light(void)
{
    ADCON0 = L_ADC;
    adc_poll();
    int result = ADRESH;
    if(result > 100)
    {
        return 0;
    }
    return 1;
}






