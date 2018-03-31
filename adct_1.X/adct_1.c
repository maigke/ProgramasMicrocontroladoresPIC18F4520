/*
 * File:   adct_1.c
 * Author: miguel
 *
 * Created on 26 de marzo de 2018, 07:25 PM
 */
#include <stdio.h>
#include <stdlib.h>

#define PIC18F4520
//#define PIC18F4550
//#define PIC18F45K50

#define _XTAL_FREQ 4000000
#include <xc.h>

#ifdef PIC18F4520
#include <config5.h>
#include <pic18f4520.h>
#endif

#ifdef PIC18F45K50
#include <config3.h>
#include <pic18f45k50.h>
#endif
#ifdef PIC18F4550
#include <config.h>
#include <pic18f4550.h>
#endif

#include <lcd4a.h>

#define ledVerde   PORTCbits.RC2

char value[8];  //guarda espacio para imprimir un numero

#include <xc.h>

void main(void) {
    float valor;
    
    TRISD = 0x00;
   TRISB = 0x00;
   
#ifdef PIC18F45K50
    ANSELA = 0x01; //Entrada analógica en RA0
#endif
    //Configurando adc
    ADCON1 = 0x0E; //Solo el puerto RA0 es analógico
    TRISA0 = 1; 
    ADFM = 1; //justificacion a la izquierda
    ADCON0 = 0x00; //se selecciona el AN0
                    //modulo ADC esta habilitado
    ADON = 1;
    
   TRISCbits.TRISC2 = 0;
   clrbit(LAT(lcd_rw),p_rw);
   ledVerde = 0;
   lcd_init();
   lcd_cmd(0x01);
   lcd_gotoxy(1,1);
   
   lcd_msg("@montiel");
   
    while(1)
    {
        ADIF = 0;
        ADCON0bits.GO = 1; //comienza la conversión
        while(ADIF==0);
            //enviar dato
        valor=ADRES;
        valor = valor*500.0/1023.0;
        valor -= 4;
        sprintf(value,"%3.3f",valor);
        lcd_gotoxy(4,2);
        lcd_msg(value);
        lcd_msg(" °C");
    }
    return;
}
