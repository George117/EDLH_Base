/*
 * File:   adc_module.c
 * Author: georg
 *
 * Created on August 23, 2020, 4:26 PM
 */

#include "adc_module.h"
#include "bit_settings.h"

/* 12 bit conversion
 * TAD = 2 us 
 * Single ended
 * VREF- is connected to VSS
 * VREF+ is connected to VDD
 */
void Configure_ADC_Module(void)
{
    ADCON0bits.ADRMD = 0; // ADRESL and ADRESH provide data formatted for a 12-bit result
    
    ADCON1bits.ADFM = 0; // Sign-magnitude result format.
    
    ADCON1bits.ADCS2 = 1; // TAD 2us
    ADCON1bits.ADCS1 = 1; // FOSC/64
    ADCON1bits.ADCS0 = 0;
    
    ADCON1bits.ADNREF = 0; //VREF- is connected to VSS
    
    ADCON1bits.ADPREF1 = 0; //VREF+ is connected to VDD
    ADCON1bits.ADPREF0 = 0; 
    
    ADCON2bits.TRIGSEL = 0; // Disabled
    
    ADCON2bits.CHSN3 = 1; // ADC Negative reference ? selected by ADNREF
    ADCON2bits.CHSN2 = 1;
    ADCON2bits.CHSN1 = 1;
    ADCON2bits.CHSN0 = 1;        
            
    ADCON0bits.ADON = 1; // ADC Enable bit
}

/* Generic implementation
 */
void Configure_ADC_Channel(unsigned char channel)
{
    if(channel & 0b1000){
        TRISB = TRISB | (1 << channel_mapping[channel]);
        ANSELB = ANSELB | (1 << channel_mapping[channel]);
        WPUB = WPUB & (~(1 << channel_mapping[channel])); 
    }
    else{
        TRISA = TRISA | (1 << channel_mapping[channel]);
        ANSELA = ANSELA | (1 << channel_mapping[channel]);
        WPUA = WPUA & (~(1 << channel_mapping[channel]));  
    }
}


int Get_Value_From_Channel(unsigned char channel)
{
    unsigned short rez_adc = 0;
    unsigned short high_reg = 0;
    unsigned short low_reg = 0;    
    
    ADCON0bits.CHS = channel; // selected channel

    __delay_us(100); // give it some time
    
    //Start acq and wait to finish
    ADCON0bits.GO = 1;
    while(ADCON0bits.GO == 1){};
    
    //get the 12bit result
    high_reg = ADRESH;
    low_reg = ADRESL;
    rez_adc = ((high_reg << 8) | low_reg);
    rez_adc = rez_adc >> 4;
    
    return rez_adc;
}