/*
 * File:   Hola_3.c
 * Author: miguel
 *
 * Created on 8 de enero de 2018, 06:44 PM
 * Este programa hace lo mismo que el anterior
 * Hola_2, solo que se utiliza la configuración
 * de un solo bit por registro
 */
#define AA PORTD
#define BB 0
#include <config.h>
#include <xc.h>
#include <pic18f4550.h>
#define _XTAL_FREQ 20000000


void main(void) {
    
    TRISD &= ~(1 << 7);
    TRISB &= ~(1 << 0);
    //TRIS(AA) = 0x00;
    //TRISB = 0x00;
    clrbit(LAT(AA),3);
            //habilita las resistencias de pull up PORT D
    PORTEbits.RDPU = 1;
    //habilita las resistencias de pull up PORTB
    INTCON2bits.RBPU = 0;
    LAT(PORTB) = 0x00;
    LATB = 0x00;
    LATD = 0x00;
    while (1)
    {
        LATB |= (1 << BB);
        LAT(AA) &= ~(1 << 7);
        __delay_ms(500);
        LATB &= ~(1 << BB);
        LATD |= (1 << 7);
        __delay_ms(500);
        LATB |= (1 << BB);
        LATD |= (1 << 7);
        __delay_ms(500);
        LATB &= ~(1 << BB);
        LATD &= ~(1 << 7);
        __delay_ms(500);
    }
    return;
}
