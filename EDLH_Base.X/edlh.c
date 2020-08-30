/*
 * File:   edlh.c
 * Author: georg
 *
 * Created on August 30, 2020, 11:04 AM
 */
#include <string.h>
#include <stdio.h>
#include "bit_settings.h"
#include "edlh.h"
#include "oled.h"
#include "adc_module.h"

#define vcc     3.3

//determined empirically
#define zero_amps_level (vcc/2)

#define quanta_4096 (vcc/4096)

// 5v/2^10
#define quanta_1024 (vcc/1024)
                    
// 5v/2^8
#define quanta_256 (vcc/256)

unsigned int I_meas_samples[4] = {0x00,0x00,0x00,0x00};    
unsigned int U_meas_samples[4] = {0x00,0x00,0x00,0x00};  

float instant_batt_current = 0;
float instant_batt_voltage = 0;

extern char out_buffer[10];

void EDLH_Init(void)
{
    OLED_Init(0x3C);
    OLED_clear();
    OLED_string("EDLH Reporting..", 0, 0);
    OLED_write();
    __delay_ms(1000);
}

void EDLH_Display(void)
{
    

    
    // clear the frame buffer
    OLED_clear();

    // measure the battery current
   // read_I_meas();

    // measure the battery voltage
    read_V_meas();

    // reset the buffers
    memset(buffer, 0, 3);
    memset(batt_current, 0, 6);
    memset(batt_voltage, 0, 6);

//    // batt current convert from number to string
//    sprintf(buffer, "%1.1f", instant_batt_current);
//    strcpy(batt_current,battery_curr_string);
//    strcat(batt_current,buffer);
//    strcat(batt_current,ampere);

     // five volt supply convert from number to string
    sprintf(buffer, "%1.2f", instant_batt_voltage);
    strcpy(batt_voltage,battery_volt_string);
    strcat(batt_voltage,buffer);
    strcat(batt_voltage,volt);

    // populate the frame buffer
    OLED_string(batt_voltage,0,0);
   // OLED_string(batt_current,0,8);

    // write to display
    OLED_write();


}


void read_I_meas(void)
{
    unsigned int rez_adc = 0;

    //Start acq and wait to finish
    ADCON0bits.GO = 1;
    while(ADCON0bits.GO == 1){};
    
    //get the 12bit result
    rez_adc = Get_Value_From_Channel(AN1);
    
    //4 samples average
    I_meas_samples[0] = (rez_adc + I_meas_samples[1] + I_meas_samples[2] + I_meas_samples[3]) / 4;
    I_meas_samples[3] = I_meas_samples[2];
    I_meas_samples[2] = I_meas_samples[1];
    I_meas_samples[1] = I_meas_samples[0];
    
    //populate byte 0 and 1 of data output
   // out_buffer[0] = I_meas_samples[0] >> 2;
  //  out_buffer[1] = I_meas_samples[0] & 0x03;
 
    //calculate the current
    instant_batt_current = ((((I_meas_samples[0] * quanta_4096) - zero_amps_level)/*Current value*/ * 10/*-> A*/) * 1.05/*curve correction*/) + 0.05/*offset*/;
}

void read_V_meas(void)
{
    unsigned int rez_adc = 0;

    rez_adc = Get_Value_From_Channel(AN0);
    
    //4 samples average
    U_meas_samples[0] = (rez_adc + U_meas_samples[1] + U_meas_samples[2] + U_meas_samples[3]) / 4;
    U_meas_samples[3] = U_meas_samples[2];
    U_meas_samples[2] = U_meas_samples[1];
    U_meas_samples[1] = U_meas_samples[0];
    
    //populate byte 2 and 3 of data output
   // out_buffer[2] = U_meas_samples[0] >> 2;
   // out_buffer[3] = U_meas_samples[0] & 0x03;

    //calculate the voltage
    instant_batt_voltage = ((U_meas_samples[0] * quanta_4096)/0.1276785714285714);
}