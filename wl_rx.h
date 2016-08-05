/* 
 * File:   wl_rx.h
 * Author: dinusha
 *
 * Created on November 8, 2015, 10:27 PM
 */

#ifndef WL_RX_H
#define	WL_RX_H

#ifdef	__cplusplus
extern "C" {
#endif
   
    
void wl_rx_init();
unsigned char wl_rx_readbit();
unsigned char wl_rx_readbyte();
unsigned char wl_rx_receive(unsigned char *addr, unsigned char *data);


#ifdef	__cplusplus
}
#endif

#endif	/* WL_RX_H */

