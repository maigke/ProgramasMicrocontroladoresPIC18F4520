/*
 * File:   Teclado_1.c
 * Author: miguel
 *
 * Created on 15 de enero de 2018, 12:42 PM
 * Este programa muestra en 4 leds el valor 
 * asignado a cada tecla de un teclado matricial
 * de 4 x 4
 * Esquema de conexion
 * columnas Nibble menos significativo  Puerto D
 * Renglones Nibble menos signiticativo Puerto B
 * Leds Puerto C Nibble menos signiticativo 
 */

#define PIC18F45K50
//#define PIC18F4450
//#define PIC18F452    //se define que micro se va a usar

#ifdef PIC18F45K50
#include "config3.h"
#include <pic18f45k50.h>
#endif

#ifdef PIC18F4550
#include <config.h>
#include <pic18f4550.h>
#endif

#ifdef PIC18F452
#include <config2.h>
#include <pic18f452.h>
#endif 

#ifdef PIC18F4450
#include <config4.h>
#include <pic18f4450.h>
#endif

#include <keyb.h>
#include <xc.h>
#define _XTAL_FREQ 4000000

#define Led_0  LATDbits.LATD1
#define Led_1  LATCbits.LATC1
#define Led_2  LATCbits.LATC2
#define Led_3  LATDbits.LATD0
#define Led_4  LATDbits.LATD2
#define Led_5  LATDbits.LATD3
#define Led_6  LATCbits.LATC6
#define Led_7  LATCbits.LATC7

#define cLed_0  TRISDbits.TRISD1
#define cLed_1  TRISCbits.TRISC1
#define cLed_2  TRISCbits.TRISC2
#define cLed_3  TRISDbits.TRISD0
#define cLed_4  TRISDbits.TRISD2
#define cLed_5  TRISDbits.TRISD3
#define cLed_6  TRISCbits.TRISC6
#define cLed_7  TRISCbits.TRISC7


void display(unsigned char valor);

void main(void) {
    
    unsigned char tecla,vtecla;
    ANSELA = 0x00;
    vInitKeyPad();
    
    
    cLed_0 = 0;
    cLed_1 = 0;
    cLed_2 = 0;
    cLed_3 = 0;
    cLed_4 = 0;
    cLed_5 = 0;
    cLed_6 = 0;
    cLed_7 = 0;
    //vtecla = 100;
    display(8);
    
    //__delay_ms(1550);

    while(1)
    {
        tecla = kbd_getc();
        if(tecla!='\0')
            display(tecla-'0');
    }
    return;
}

void display(unsigned char valor)
{

    if (valor & (1 << 0)) Led_0 = 1;
    else Led_0 = 0;
    
    if (valor & (1 << 1)) Led_1 = 1;
    else Led_1 = 0;
    
    if (valor & (1 << 2)) Led_2 = 1;
    else Led_2 = 0;
    
    if (valor & (1 << 3)) Led_3 = 1;
    else Led_3 = 0;
    
    if (valor & (1 << 4)) Led_4 = 1;
    else Led_4 = 0;
    
    if (valor & (1 << 5)) Led_5 = 1;
    else Led_5 = 0;
    
    if (valor & (1 << 6)) Led_6 = 1;
    else Led_6 = 0;
    
    if (valor & (1 << 7)) Led_7 = 1;
    else Led_7 = 0;
}