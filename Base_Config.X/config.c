/*
 * File:   config.c
 * Author: georg
 *
 * Created on August 14, 2020, 10:30 PM
 */


#include <xc.h>
#include "config.h"



/* Set the IO to known config
 */
void IO_First_Init(void)
{
    TRISA = 0x00;   
    TRISB = 0x00;
    TRISC = 0x00;
    
    ANSELA = 0x00;
    ANSELB = 0x00;
    ANSELC = 0x00;
    
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;  
    
    INTCONbits.GIE=0;
}

/* Configure to 32MHZ
 */
void Configure_Clock(void)
{
    OSCCONbits.SCS0 = 0;
    OSCCONbits.SCS1 = 0;
    
    
    // 111x For PLL enable
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF3 = 1;
    

    /*calibrate the INTOSC
     * 100000 = minimum freq
     * 111111
     * 000000 = factory calib
     * 000001
     * 011111 =  maximum freq
     * signed
     */ 
    OSCTUNE = 0b00011111;
    
    // Enable PLL => CLK = 32MHZ
    OSCCONbits.SPLLEN = 1;
}