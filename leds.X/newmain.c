/*
 * File:   newmain.c
 * Author: miguel
 *
 * Created on 30 de diciembre de 2017, 11:21 PM
 */
#include <xc.h>

//#pragma code _HIGH_INTERRUPT_VECTOR = 0x000C08
void interrupt high_ISR (void)
{
    
}

//#pragma code _LOW_INTERRUPT_VECTOR = 0x000C18
void interrupt low_priority LowIsr  (void)
{
    
}




#define _XTAL_FREQ 20000000

void main(void) {
    
    TRISAbits.TRISA4 = 0;
    //xx00 0000
    TRISB =0x00;
    while(1){
    LATAbits.LATA4 = 1;
    __delay_ms(500);
    LATAbits.LATA4 = 0;}
    Reset();
    return;
}
