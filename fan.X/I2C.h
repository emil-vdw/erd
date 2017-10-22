#ifndef I2C_H
#define I2C_H

#define CLOCK_SPEED 1000000
#define READY PORTEbits.RE0
#define READY_TRIS TRISEbits.TRISE0
#define IQS333_ADDR 0x64
#define FLAGS 0x01
#define TOUCH 0X03
#define ACT_CH 0X0E
#define THRESHOLDS 0X09
#define PROXSETTINGS 0X08


#define TOUCH_THRESHOLD_CH1 0x10
#define TOUCH_THRESHOLD_CH2 0x10
#define TOUCH_THRESHOLD_CH3 0x10
#define TOUCH_THRESHOLD_CH4 0x10
#define TOUCH_THRESHOLD_CH5 0x10
#define TOUCH_THRESHOLD_CH6 0x10
#define TOUCH_THRESHOLD_CH7 0x10
#define TOUCH_THRESHOLD_CH8 0x10
#define TOUCH_THRESHOLD_CH9 0x10
#define PROX_THRESHOLD 0x10




void init_i2c(int speed);
void wait_ready(void);
void i2c_Write
    (unsigned char i2c_addr, unsigned char write_addr,
    unsigned char *data, unsigned char NoOfBytes);
void i2c_send(unsigned char data);
void i2c_start(void);
void i2c_stop(void);
void wait_i2c(void);
unsigned char i2c_read_ack(void);
unsigned char i2c_read_nack(void);
void i2c_read(unsigned char i2c_addr, unsigned char read_addr, unsigned char *data, unsigned char
    NoOfBytes);
void IQS333_setup(void);



#endif
