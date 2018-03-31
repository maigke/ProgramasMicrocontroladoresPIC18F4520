/*
 * File:   hola_1.c
 * Author: miguel
 *
 * Created on 8 de enero de 2018, 04:56 PM
 * Este programa permite encender un led conectado en el pin 29
 * que corresponde al puerto PD6
 */



#include <xc.h>
#include <pic18f4550.h>
#include <config.h>
#define _XTAL_FREQ 4000000

void main(void) {
    /*
     * Se configura como salida al pin PD6
     * 0 -- 0utput
     * 1 -- 1nput
     */
    asm("CLRF LATD");
    asm("MOVLW 0BFh"); //1011 111 TRISD
    asm("MOVWF TRISD");
    //habilita las resistencias de pull up
    PORTEbits.RDPU = 1;
    LATD = 0x40;
    while (1) {
    }
    return;
}
