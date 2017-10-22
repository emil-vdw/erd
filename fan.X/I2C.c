#include "I2C.h"
#include "hmi.h"
#include <xc.h>


void init_i2c(int speed)//100khz
{
    
    TRISCbits.RC3 = 1;
    TRISCbits.RC4 = 1;

    SSPADD = 2;
    SSPSTATbits.SMP = 1;
    SSPCON1bits.SSPEN = 1;
    SSPCON1bits.SSPM = 0b1000;
}

void wait_ready(void)
{
    while(READY);
}

void i2c_send(unsigned char send_data)
{
    unsigned int timeout = 1000;
    SSPBUF = send_data; 
    timeout = 1000;
    wait_i2c();
    while (SSPCON2bits.ACKSTAT == 1 && timeout > 0)
    {
        timeout--;
    }
    Nop();
}

void i2c_Write
    (unsigned char i2c_addr, unsigned char write_addr,
    unsigned char *data, unsigned char NoOfBytes)
{
    unsigned char i;
    i2c_send((i2c_addr << 1) + 0x00);
    i2c_send(write_addr);
    for (i = 0 ; i < NoOfBytes ; i++)
        i2c_send(data[i]);
}

void i2c_repeat_start(void)
{
    SSPCON2bits.SEN = 1;
    wait_i2c();
    while(SSPCON2bits.SEN);
}

void i2c_start(void)
{
    wait_ready();
    SSPCON2bits.SEN = 1;
    wait_i2c();
    while(SSPCON2bits.SEN);
}

void i2c_stop(void)
{
    SSPCON2bits.PEN = 1;
    wait_i2c();
    while(SSPCON2bits.PEN);
    while(READY);
    while(!READY);
}

void wait_i2c(void)
{
    while(PIR1bits.SSPIF == 0);
    PIR1bits.SSPIF = 0;
}

void i2c_read(unsigned char i2c_addr, unsigned char read_addr, unsigned char *data, unsigned char
    NoOfBytes)
{
    unsigned char i;
    i2c_send((i2c_addr << 1) + 0x00);
    i2c_send(read_addr);
    i2c_repeat_start();
    i2c_send((i2c_addr << 1) + 0x01);
    if (NoOfBytes > 1)
    {
    for (i = 0; i < NoOfBytes - 1; i++)
    data[i] = i2c_read_ack();
    }
    data[NoOfBytes-1] = i2c_read_nack();
    // device address + write
    // IQS address-command
    // device address + read
    // all bytes except last must be followed by an ACK
    // last byte read must be followed by a NACK
}

unsigned char i2c_read_ack(void)
{
    unsigned char temp;
    unsigned int timeout = 1000;
    
    SSPCON2bits.RCEN = 1;
    wait_i2c();
    timeout = 1000;
    while (SSPSTATbits.BF == 0 && timeout > 0)
    {
        timeout--;
    }
    temp = SSPBUF;
    SSPCON2bits.ACKDT = 0;
    SSPCON2bits.ACKEN = 1;
    timeout = 1000;
    wait_i2c();
    timeout = 1000;
    while (SSPCON2bits.ACKEN == 1 && timeout > 0)
    {
        timeout--;
    }
    return temp;
}

unsigned char i2c_read_nack(void)
{
    unsigned char temp;
    unsigned int timeout = 1000;
    // create a timeout for communication
    SSPCON2bits.RCEN = 1;
    wait_i2c();
    timeout = 1000;
    while (SSPSTATbits.BF == 0 && timeout > 0)
    {
        timeout--;
    }
    temp = SSPBUF; // store received byte
    SSPCON2bits.ACKDT = 1;
    SSPCON2bits.ACKEN = 1; // enable NACK
    // execute NACK sequence
    timeout = 1000;
    wait_i2c();
    timeout = 1000;
    while (SSPCON2bits.ACKEN == 1 && timeout > 0)
    {
        timeout--;
    } // Reload timer
    // verify acknowledge sequence is complete
    return temp; // return data
}

void IQS333_setup(void)
{
    unsigned char data_buffer[12];
    /* Read product number */
    
//    data_buffer[0] = 0b00100000;
//    i2c_start();
//    i2c_Write(IQS333_ADDR, FLAGS, &data_buffer[0], 1);
//    i2c_stop();
    
    data_buffer[0] = 0b11111111;
    data_buffer[1] = 0b00000011;
    i2c_start();
    i2c_Write(IQS333_ADDR, ACT_CH, &data_buffer[0], 2);
    i2c_stop();
    
    data_buffer[0] = 0x06;
    data_buffer[1] = 0x08;
    data_buffer[2] = 0x00;
    
    
    i2c_start();
    i2c_Write(IQS333_ADDR, 0x08, &data_buffer[0], 3);
    i2c_stop();
    
    data_buffer[0] = 0x20;
    
    
    i2c_start();
    i2c_Write(IQS333_ADDR, 0x01, &data_buffer[0], 1);
    i2c_stop();
    
    /* Set the ATI Targets (Target Counts) */

    do
    {
        delay(2);
        i2c_start();
        i2c_read(IQS333_ADDR, FLAGS, &data_buffer[0], 1);
        i2c_stop();
    }
    while ((data_buffer[0] & 0b00000100) == 0b00000100);
  
    i2c_start();
    i2c_read(IQS333_ADDR, PROXSETTINGS , &data_buffer[0], 2);
    i2c_stop();
}