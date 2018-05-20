/*
 * File:   usart_6,c.c
 * Author: miguel
 *
 * Created on 20 de mayo de 2018, 12:07 AM
 */

#include <stdio.h>
#include <xc.h>
#define PIC18F4520
//#define PIC18F4550
//#define PIC18F45K50

#define _XTAL_FREQ 16000000
#include <xc.h>
#include <lcd4a.h>
#include <usart.h>

char value[8];  //guarda espacio para imprimir un numero


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

#define led LATCbits.LATC4


void main(void)
{
        unsigned char c;
    
    TRISCbits.TRISC4 = 0;
    sci_conf();
    lcd_init();
    lcd_msg("Puerto Serial");
    lcd_gotoxy(1,2); 
    lcd_msg("hola");
    printf("Muestra caracteres\n\r");
    while(1)
    {
        for (unsigned char x = 65; x <=90; x++)
            {
                printf("letra: %c\n\r",x);
                led^= 1;
                lcd_gotoxy(1,2); lcd_msg("     ");
                lcd_gotoxy(1,2); lcd_putc(x);
                __delay_ms(2000);
            }
    }
    return;
}
