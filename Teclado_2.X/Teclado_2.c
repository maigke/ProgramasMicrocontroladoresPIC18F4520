/*
 * File:   Teclado_2.c
 * Author: miguel
 *
 * Created on 15 de enero de 2018, 06:31 PM
 */
#define PIC18F45K50
//#define PIC18F4550
//#define PIC18F452    //se define que micro se va a usar

#ifdef PIC18F4550
#include <config.h>
#include <pic18f4550.h>
#endif

#ifdef PIC18F452
#include <config2.h>
#include <pic18f452.h>
#endif 

#ifdef PIC18F45K50
#include <config3.h>
#include <pic18f45k50.h>
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


#define PortRota PORTA
#define unidad   2
#define decena   3
#define centena  4
#define umillar  5

void display(unsigned char valor);
void rota(unsigned char display);

#include <xc.h>

void main(void) {

    unsigned char tecla, vtecla,i;
    
  // asm ("CLRF ANSELA");
    ANSELA = 0x00;

    vInitKeyPad();

    //inicializa el sector para turnar displays
    clrbit(TRIS(PortRota), unidad); //salida
    clrbit(TRIS(PortRota), decena); //salida
    clrbit(TRIS(PortRota), centena); //salida
    clrbit(TRIS(PortRota), umillar); //salida

    cLed_0 = 0;
    cLed_1 = 0;
    cLed_2 = 0;
    cLed_3 = 0;
    cLed_4 = 0;
    cLed_5 = 0;
    cLed_6 = 0;
    cLed_7 = 0;
    

    while (1) {
        tecla = kbd_getc();
        if (vtecla != tecla) {
            display(tecla);
        }
        for ( i = 0; i < 5; i++) {
                rota(i);
                __delay_ms(5);
            }
        vtecla = tecla;
    }
}

void display(unsigned char valor) {

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

void rota(unsigned char display) {
    
    switch (display) {
        case 0:
            setbit(LAT(PortRota), unidad);
            clrbit(LAT(PortRota), decena);
            clrbit(LAT(PortRota), centena);
            clrbit(LAT(PortRota), umillar);
            break;
        case 1:
            clrbit(LAT(PortRota), unidad);
            setbit(LAT(PortRota), decena);
            clrbit(LAT(PortRota), centena);
            clrbit(LAT(PortRota), umillar);
            break;
        case 2:
            clrbit(LAT(PortRota), unidad);
            clrbit(LAT(PortRota), decena);
            setbit(LAT(PortRota), centena);
            clrbit(LAT(PortRota), umillar);
            break;
        case 3:
            clrbit(LAT(PortRota), unidad);
            clrbit(LAT(PortRota), decena);
            clrbit(LAT(PortRota), centena);
            setbit(LAT(PortRota), umillar);
            break;
        default:
            clrbit(LAT(PortRota), unidad);
            clrbit(LAT(PortRota), decena);
            clrbit(LAT(PortRota), centena);
            clrbit(LAT(PortRota), umillar);
    }

}