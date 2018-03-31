/*
 * File:   lcdt_1.c
 * Author: miguel
 *
 * Created on 3 de marzo de 2018, 10:38 PM
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
int numero = 1019;

void main(void) {
    
   
#ifdef PIC18F45K50
   ANSELD = 0;
   ANSELC = 0;
   ANSELB = 0;
#endif

   TRISCbits.TRISC2 = 0;
   clrbit(LAT(lcd_rw),p_rw);
   ledVerde = 0;
   lcd_init();
   lcd_cmd(0x01);
   lcd_gotoxy(1,1);
   
   lcd_msg("@montiel");
   lcd_gotoxy(9,2);
   lcd_setcursor_vb(1,1);
   lcd_putc('a');
   lcd_putc(lcd_hexa(6));
   sprintf(value,":%d",numero);
   lcd_msg(value);
   while(1){
       ledVerde ^=1;
       __delay_ms(500);
   }
        return;
}
