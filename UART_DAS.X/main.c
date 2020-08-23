/*
 * File:   main.c
 * Author: georg
 *
 * Created on August 8, 2020, 9:48 PM
 */

#include <xc.h>
#include "bit_settings.h"
#include "config.h"


void main(void) 
{
    IO_First_Init();
    Configure_Clock();    

    
    while(1){
        LATCbits.LATC7 = 0;
        __delay_us(1);
        LATCbits.LATC7 = 1;
        __delay_us(1);
    }
    
    
    
}
