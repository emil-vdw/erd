#include "esp8266.h"
#include <stdio.h>
#include <string.h>

#pragma config PMDL1WAY = ON            // Peripheral Module Disable Configuration (Allow only one reconfiguration)
#pragma config IOL1WAY = ON             // Peripheral Pin Select Configuration (Allow only one reconfiguration)
#pragma config FUSBIDIO = ON            // USB USID Selection (Controlled by the USB Module)
#pragma config FVBUSONIO = ON           // USB VBUS ON Selection (Controlled by USB Module)

// DEVCFG2
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
#pragma config FPLLMUL = MUL_20         // PLL Multiplier (20x Multiplier)
#pragma config UPLLIDIV = DIV_2         // USB PLL Input Divider (2x Divider)
#pragma config UPLLEN = OFF             // USB PLL Enable (Disabled and Bypassed)
#pragma config FPLLODIV = DIV_2         // System PLL Output Clock Divider (PLL Divide by 2)

// DEVCFG1
#pragma config FNOSC = FRCPLL           // Oscillator Selection Bits (Fast RC Osc with PLL)
#pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Disabled)
#pragma config IESO = OFF               // Internal/External Switch Over (Disabled)
#pragma config POSCMOD = OFF            // Primary Oscillator Configuration (Primary osc disabled)
#pragma config OSCIOFNC = OFF           // CLKO Output Signal Active on the OSCO Pin (Disabled)
#pragma config FPBDIV = DIV_1           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/8)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config WDTPS = PS1048576        // Watchdog Timer Postscaler (1:1048576)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable (Watchdog Timer is in Non-Window Mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))
#pragma config FWDTWINSZ = WINSZ_25     // Watchdog Timer Window Size (Window Size is 25%)

// DEVCFG0
#pragma config JTAGEN = OFF             // JTAG Enable (JTAG Disabled)
#pragma config ICESEL = ICS_PGx1        // ICE/ICD Comm Channel Select (Communicate on PGEC1/PGED1)
#pragma config PWP = OFF                // Program Flash Write Protect (Disable)
#pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP = OFF                 // Code Protect (Protection Disabled)


#include <xc.h>
#include <pthread/limits.h>


#define SYSCLK  40000000L
#define PWM_FREQ    16000


void main()
{
    
    int i=0;
    RPB7Rbits.RPB7R = 0x0005;
    int dc = 0;
    ANSELB = 0;  
    TRISBbits.TRISB5 = 0; 
    LATBbits.LATB5 = 0;
    UART1Configure();
    UART2Configure(); 
    connect();
 
 
     while(1)
     {
        
        char x = SerialReceive();
        int i =0;
        char send[500];
        bool power=true;
        
        switch(x)
        {
            case 'p': 
                power = !power;
                
                if(power)
                {    
                    dc = 0;
                    pwm(dc);
                }
                
            SerialTransmit("AT+CIPSTART=2,\"TCP\",\"192.168.4.100\",50150\r\n");

            while(getU2() != '\n');
            for(; i<100000; i++);
            
            if(power)
            {
                SerialTransmit("AT+CIPSEND=2,11\r\n");
                SerialTransmit("Fan is on\r\n");
            }
            else
            {
                SerialTransmit("AT+CIPSEND=2,11\r\n");
                SerialTransmit("Fan is off\r\n");
            }
            
           LATBbits.LATB5 = 1;
           for(; i<1000000; i++);
           LATBbits.LATB5 = 0; 
           
            break;
            case '1':
            SerialTransmit2("1");
            if(dc!=100)
                dc+=10;
            pwm(dc);
         
            
           sprintf(send, "p%d\r\n",dc);
           SerialTransmit("AT+CIPSTART=2,\"TCP\",\"192.168.4.100\",50150\r\n");
           while(getU2() != '\n');
           for(; i<100000; i++);
           
           if(dc < 10)
           { 
                SerialTransmit("AT+CIPSEND=2,4\r\n");
                SerialTransmit(send);
           }
           else
           {    
               SerialTransmit("AT+CIPSEND=2,5\r\n");
               SerialTransmit(send);
           }
           
           LATBbits.LATB5 = 1;
           for(; i<1000000; i++);
           LATBbits.LATB5 = 0;   
           break;
            case '2':
            SerialTransmit2("2");
            if(dc!=0)
                 dc-=10;
             pwm(dc);
            
            

            sprintf(send, "p%d\r\n",dc);
            SerialTransmit("AT+CIPSTART=2,\"TCP\",\"192.168.4.100\",50150\r\n");

            while(getU2() != '\n');
            for(; i<100000; i++);
            if(dc < 10)
            { 
                 SerialTransmit("AT+CIPSEND=2,4\r\n");
                 SerialTransmit(send);
            }
            else
            {    
                SerialTransmit("AT+CIPSEND=2,5\r\n");
                SerialTransmit(send);
            }

            LATBbits.LATB5 = 1;
            for(; i<1000000; i++);
            LATBbits.LATB5 = 0; 
               break;
               
            case '3':
                SerialTransmit2("3");
                    //Decrease angle
                break;
            case '4':
                SerialTransmit2("4");
                    //Increase angle
                break;
                        
            case 'm':
                //change mode
                break;
               
            case 'u':
                  
                LATBbits.LATB5 = 1;
                for(; i<1000000; i++);
                LATBbits.LATB5 = 0; 
                SerialTransmit2("z"); 
                break;
            
            case 'z':
                integration();
                break;
        }
     }
}

void connect()
{
    int x =0;
    for(; x<10000000; x++);
    SerialTransmit("AT+CWSAP=\"Smart Fan\",\"erd320end#\",7,4\r\n");
    int y =0;
    for(; y<10000000; y++);
    SerialTransmit("AT+RST\r\n");
    int z =0;
    for(; z<10000000; z++);
    SerialTransmit("AT+CIPMUX=1\r\n");
    int i =0;
    for(; i<10000000; i++);
    SerialTransmit("AT+CIPSERVER=1,1025\r\n"); 
}

char getU2(void)
{
    while(!U2STAbits.URXDA);
    return U2RXREG;
}

char SerialReceive()
{
    char rec;
    char temp;

    if(U2STAbits.OERR == 1)
        U2STAbits.OERR =0;
    U2MODESET = 0x8000;
    U2MODECLR = 0x4000;  
    TRISBbits.TRISB5 = 0; 
    U2STAbits.URXEN=1;
    
    

    while(getU2() != ':');
    temp = getU2();
    
    if(temp == 'w')
        connect();
    else
    { 
      getU2();
      getU2();
      return temp;
    }
}

void UART1Configure()
{
    RPB15Rbits.RPB15R = 1;// TX = RB15, blue
    
    U1MODE = 0;         
    U1STA = 0x1400;  
    U1BRG = 259;
    U1MODESET = 0x8000;
}

void UART2Configure()
{
    U2RXRbits.U2RXR = 4; // RX = RB8, green
    RPB9Rbits.RPB9R = 2;// TX = RB9, blue
    
    U2MODE = 0;         
    U2STA = 0x1400;  
    U2BRG = 259; 
    U2MODESET = 0x8000;
}

void SerialTransmit(const char *buffer)
{
    
     unsigned int size = strlen(buffer);
     
     while(size)
     {
        while(U2STAbits.UTXBF);
        U2TXREG = *buffer;          
 
        buffer++;                   
        size--;         
     }
     
     while( !U2STAbits.TRMT);   
}

void SerialTransmit2(const char *buffer)
{
    
     unsigned int size = strlen(buffer);
     
     while(size)
     {
        while(U1STAbits.UTXBF);
          U1TXREG = *buffer;          
 
        buffer++;                   
        size--;         
     }
     
     while( !U1STAbits.TRMT);   
}

void pwm(int dc)
{
    OC1CON = 0x0006; 
    PR2 = (SYSCLK / PWM_FREQ) - 1;
    OC1RS = (PR2 + 1) * ((float)dc / 100);
 
    T2CONSET = 0x8000;    
    OC1CONSET = 0x8000;     
}

void integration()
{   
    char refresh[9];
    
    int j=0;
    for(;j<strlen(refresh);j++)
        refresh[j] = getU2();
    
    
    sprintf("u%s\r\n",refresh);
    SerialTransmit("AT+CIPSTART=2,\"TCP\",\"192.168.4.100\",50150\r\n"); //192.168.4.100

    while(getU2() != '\n');
    int i=0;
    for(; i<100000; i++);
    SerialTransmit("AT+CIPSEND=2,9\r\n");
    SerialTransmit(refresh);
}



