/*
 * File:   main.c
 * Author: georg
 *
 * Created on August 8, 2020, 9:48 PM
 */

#include <xc.h>
#include "bit_settings.h"
#include "config.h"
#include "adc_module.h"
#include "data_out.h"
#include "edlh.h"

void main(void) 
{
    IO_First_Init();
    
    EDLH_TX_RST_HOLD();
    
    Configure_Clock();    
    
    
    
    Configure_ADC_Module();
    Configure_ADC_Channel(AN0);
    Configure_ADC_Channel(AN1);
    
    init_data_out(19200);
    init_timebase();
    
    EDLH_Init();
    EDLH_Display();
    
    EDLH_TX_RUN();
    
    while(1){
        EDLH_Display();    
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