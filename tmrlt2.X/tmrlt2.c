/*
 * File:   tmrlt2.c
 * Author: miguel
 *
 * Created on 28 de marzo de 2018, 07:26 PM
 */


#include <xc.h>
#include <config5.h>
#include <pic18f4520.h>
#define _XTAL_FREQ 4000000
#define PledVerde  PORTC
#define ledVerde   2
#define T0INI 100

void pausa_10ms()
{
    TMR0 = T0INI;
    T0IF = 0;
    while(T0IF==0);
}

void main(void) {
    clrbit(TRIS(PledVerde),ledVerde);
    T0CS=0; PSA=0;
    T0PS2=1; T0PS1=0; T0PS0=1;
    while(1){
        pausa_10ms();
        tglbit(LAT(PledVerde),ledVerde);
        pausa_10ms();
    }
    return;
}
