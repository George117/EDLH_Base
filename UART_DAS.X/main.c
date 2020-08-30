/*
 * File:   main.c
 * Author: georg
 *
 * Created on August 8, 2020, 9:48 PM
 */

#include <xc.h>
#include "bit_settings.h"
#include "config.h"
#include "data_out.h"

char receive_buffer;

void main(void) 
{
    IO_First_Init();
    Configure_Clock();    
    
    init_data_out(9600);
    init_timebase();
    
    while(1){
        LATCbits.LATC1 = 0;
        __delay_us(1);
        LATCbits.LATC1 = 1;
        __delay_us(1);
    }
    
    
    
}
//interrupt routine
void __interrupt() timer(void)
{
    timebase_interrupt();
    
    if(RCIF){
        receive_buffer = RC1REG;
        RCIF = 0;
    }
}