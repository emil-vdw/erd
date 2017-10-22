#ifndef SEN_H
#define SEN_H

# define T_ADC 0b00000011
# define L_ADC 0b00000111
# define P_ADC 0b00001011

# define TEST_ADC 0b00001111

void init_adc(void);
int get_temp(void);
void adc_poll(void);
void display_temp(void);

#endif