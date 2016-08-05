#include <xc.h>

#define _XTAL_FREQ      8000000 // 8MHz Clock 

#define RX_TRIS TRISIObits.TRISIO0
#define RX_PORT GPIObits.GP0


void wl_rx_init()
{
    RX_TRIS = 1;
    RX_PORT = 0;
}

unsigned char wl_rx_readbit()
{
    while( !RX_PORT );
    //__delay_us(10);
    //if(!RX_PORT) return 0;
    while( RX_PORT );
    TMR0 = 0;
    while( !RX_PORT &&  TMR0 < 185 );

    if( TMR0 > 170 ){
        return 1;
    }else{
        return 0;
    }
}

unsigned char wl_rx_readbyte()
{
    unsigned char i, data = 0;

    for(i=0;i<8;i++){
        
        data = data << 1;
        data |= wl_rx_readbit();

    }

    return data;
}

unsigned char wl_rx_receive(unsigned char *addr, unsigned char *data)
{
    unsigned char i=0;
    //unsigned char data[8] = {0};
 
    while(i < 10){
        if(wl_rx_readbit() == 0)
            i++;
        else
            return 0;
    }
        
    while(wl_rx_readbit() != 1);
    
    unsigned char bt1, len, bt4;
    
    bt1   = wl_rx_readbyte();
    *addr = wl_rx_readbyte();
    len   = wl_rx_readbyte();
    bt4   = wl_rx_readbyte();
    
    //printf("- %x -", bt1);
    
    if(bt1 == 0x55 && bt4 == 0x77){
        for(i=0;i<len;i++){
            *(data + i) = wl_rx_readbyte();
        }
        
        //for(i=0;i<8;i++){
            //printf("| Received from device %x (%x, %x, %x, %x, %x, %x, %x, %x) | ", bt2, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
        //}
    }else{
        len = 0;
    }    
    
    return len;
}


