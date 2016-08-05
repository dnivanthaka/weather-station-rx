/*
 * File:   main.c
 * Author: dinusha
 *
 * Created on November 8, 2015, 10:22 PM
 */


#include <xc.h>

#include "wl_rx.h"

#pragma config BOREN = ON, CPD = OFF, FOSC = INTRCIO, WDTE = OFF, CP = OFF,  PWRTE = ON

#define _XTAL_FREQ      4000000 // 4MHz Clock

#define I2C_SDA GPIObits.GP2
#define I2C_SDA_TRIS TRISIObits.TRISIO2
#define I2C_CLK GPIObits.GP5
#define I2C_CLK_TRIS TRISIObits.TRISIO5

#define I2C_ADDR 0x40

#define I2C_START (I2C_CLK && !I2C_SDA)
#define I2C_MY_ADDRESS(data)    ((I2C_ADDR & 0xFE) == (data & 0xFE))

unsigned char data[32] = {0};
unsigned char dev_id = 0, data_len = 0;

void pic_init();
unsigned char readbyte();
unsigned char readbit();
void sendack();


void pic_init()
{
//    asm("bsf STATUS, 0");
//    asm("call 3ffh");
//    asm("movwf OSCCAL");
//    asm("bcf STATUS, 0");
    
    //Disabling the A2D converter
    ADCON0bits.ADON = 0;
    CMCON = 0x07;
    ANSEL = 0;

    //PORTC = 0x00;
    //TRISC = 0x00;
    //TRISCbits.TRISC1 = 1;
    //PORTC = 0x00;
    
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.T0SE = 0;
    OPTION_REGbits.PSA  = 0;
    OPTION_REGbits.PS0  = 1;                // 800ns
    OPTION_REGbits.PS1  = 0;
    OPTION_REGbits.PS2  = 0;
    
    I2C_SDA_TRIS = 1;
    I2C_CLK_TRIS = 1;

    I2C_SDA = 0;
    I2C_CLK = 0;
    
    TRISIObits.TRISIO4 = 0;

    // Setting up interrupt on change setting

    IOCBbits.IOC2   = 1;
    INTCONbits.GPIE = 1;
    INTCONbits.GIE  = 1;
}

unsigned char readbyte()
{
    unsigned char i, data = 0;

    for(i=0;i<8;i++){
        data |= readbit();
        
        data <<= 1;
    }

    return data;
}

unsigned char readbit()
{
    //while(I2C_CLK);
    while(!I2C_CLK);

    return I2C_SDA;
}

void sendack()
{
    while(I2C_CLK);
    while(!I2C_CLK);

    I2C_SDA_TRIS = 0;
    __delay_us(10);
    I2C_SDA_TRIS = 1;
}

void interrupt isr()
{
    if(INTCONbits.GPIF){
        if( I2C_START ){
            //GPIObits.GP4 = 1;
            unsigned char data = readbyte();
            //GPIObits.GP4 = 1;
            if(I2C_MY_ADDRESS(data)){
                GPIObits.GP4 = 1;
                sendack();
            }
        }
        
        INTCONbits.GPIF = 0;
    }
}

void main(void) 
{
    pic_init();
    
    GPIObits.GP4 = 0;
    //wl_rx_init();
    //wl_rx_receive(&dev_id, data);
    
    while(1);
    
    return;
}
