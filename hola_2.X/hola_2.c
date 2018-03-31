/*
 * File:   hola_2.c
 * Author: miguel
 *
 * Created on 8 de enero de 2018, 06:25 PM
 * este programa enciende un led verde (PD7)
 * posteriormente un led rojo (pb0)
 * y finalmente ambos, a intervalos de 500 ms
 */

#include <config.h>
#include <xc.h>
#include <pic18f4550.h>
#define _XTAL_FREQ 20000000


void main(void) {
    
    TRISD = 0x7F; //para el led verde 0111 1111
    TRISB = 0xFE; //para el led rojo  1111 1110
        //habilita las resistencias de pull up PORT D
    PORTEbits.RDPU = 1;
    //habilita las resistencias de pull up PORTB
    INTCON2bits.RBPU = 0;
    while(1)
    {
        LATB = 0x01;
        LATD = 0x00;
        __delay_ms(500);
        LATB = 0x00;
        LATD = 0x80;
        __delay_ms(500);
        LATB = 0x01;
        LATD = 0x80;
        __delay_ms(500);
    }
    return;
}
