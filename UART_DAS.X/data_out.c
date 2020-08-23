/*
 * File:   data_out.c
 * Author: georg
 *
 * Created on April 8, 2019, 7:15 PM
 */
#include <stdint.h>
#include <xc.h>
#include "data_out.h"
#include "bit_settings.h"

//define the number of bytes to send, maximum of 10
//take into account the counter
#define bytes_to_send 2

//define the sendperiod of the data output:
//available: 100ms,25ms,10ms 
#define send_period_ms 100

char out_buffer[10]={0,0,0,0,0,0,0,0,0,0};
char int_counter=0;
char counter_max = 0;
char counter_tx = 0;

void send_data(void) {
    // transmit counter
    counter_tx++;
    if(counter_tx > (bytes_to_send-1)){
        counter_tx = 0;
    }
    out_buffer[(bytes_to_send-1)] = counter_tx;
 
    
    for(char i=0;i<bytes_to_send;i++)
    {  
        write_raw(out_buffer[i]);
    }
}


void write_raw(char ch){
    while(!PIR1bits.TXIF);
    TXREG=ch;
}


void timebase_interrupt(void){
    if(PIR1bits.TMR1IF==1)
    {
        PIR1bits.TMR1IF=0;
        T1CONbits.TMR1ON=0;
   
        switch(send_period_ms){
            case 100:
                TMR1L=0;//~33 ms
                TMR1H=54;
                counter_max=4; 
                break;
                
            case 25:
                TMR1L=48;//~25 ms
                TMR1H=78;
                counter_max=1; 
                break;
                
            case 10:
                TMR1L=64;//~10 ms
                TMR1H=175;
                counter_max=1; 
                break;
            default:
                TMR1L=0;//~33 ms
                TMR1H=61;
                counter_max=4; 
                break;
        }
        
        if(int_counter==counter_max)
        {
            send_data();//100ms
            int_counter=0;
        }

    int_counter++;
    T1CONbits.TMR1ON=1;

    }    
}


void init_timebase(void){
            ///////////tmr1 timer/////////////////////
    TMR1L=0;//~33 ms
    TMR1H=61;
    T1CONbits.T1CKPS1=1;////prescaler 1/4/
    T1CONbits.T1CKPS0=0;
    
    // FOSC/4 Internal Clock
    T1CONbits.TMR1CS0=0;
    T1CONbits.TMR1CS1=0;
    
    T1CONbits.T1OSCEN=0;
    T1CONbits.TMR1ON=0;
    T1CONbits.TMR1ON=1;///tmr1 on
    

    /////INTRERUPERI////////
    INTCON=0x00;
    PIR1=0x00;
    PIR2=0x00;
    PIE1=0x00;
    PIE2=0x00;
    
    INTCONbits.GIE=1;///int gen
    INTCONbits.PEIE=1;//int periferice
    PIE1bits.TMR1IE=1;///int on
    PIR1bits.TMR1IF=0;//flag
        
}


void init_data_out(long baud_rate){
            
    //SPBRG
    switch(baud_rate)
    {
     case 9600:
        SPBRG1=103;//103
        break;
     case 19200:
        SPBRG1=207;
        break;
     case 57600:
        SPBRG1=68;
        break;
     case 115200:
        SPBRG1=34;
        break;
    }
    //TXSTA
    TXSTA1bits.TX9=0;  //8 bit transmission
    TXSTA1bits.TXEN=1; //Transmit enable
    TXSTA1bits.SYNC=0; //Async mode
    TXSTA1bits.BRGH=1; //High speed baud rate

    //RCSTA
    RCSTA1bits.SPEN=1;   //Serial port enabled
    RCSTA1bits.RX9=0;    //8 bit mode
    RCSTA1bits.CREN=1;   //Enable receive
    RCSTA1bits.ADDEN=0;  //Disable address detection
    TRISCbits.TRISC7 = 1;
    
    PEIE = 1; 
    
    //Receive interrupt
    PIE1bits.RCIE=1;
}