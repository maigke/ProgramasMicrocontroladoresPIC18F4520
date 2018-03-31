/*
 * File:   Hola_6.c
 * Author: miguel
 *
 * Created on 9 de enero de 2018, 09:59 AM
 * Elabora el programa Hola_5 pero realizando un 
 * eliminador de rebotes en los botones
 * 
 * 
 * Leer las terminales PD2 y PD3 y de acuerdo con el
 * siguiente esquema encender un led bicolor de la 
 * forma que muestra a continuacion
 * +++++++++++++++++++++++++
 * +PD2   | PD3  |  Bicolor |
 * +   1  |  1   |  Verde   |
 * +   1  |  0   |  Rojo    |
 * +   0  |  1   |  Naranja |
 * +   0  |  0   |  Apagado |
 * +++++++++++++++++++++++++
 * 
 * Las salidas para el led bicolor estarán conectadas
 * en Led Rojo PD7
 * en Led Verde PB0
 * 
 */


//#define PIC18F4550
#define PIC18F452    //se define que micro se va a usar

#ifdef PIC18F4550
#include <config.h>
#include <pic18f4550.h>
#else
#include <config2.h>
#include <pic18f452.h>
#endif    

#define PLed  LATDbits
#define Led   LATD0
#include <xc.h>
#define _XTAL_FREQ 20000000

void main(void) {
    
    
    //Se limpian los latch de la salidas
    LATD = 0x0;
    LATB = 0x0;
    //se configuran los pines requeridos
    TRISD = 0x7F; //0111 1111
    TRISB = 0xFE; //1111 1110

    /****************************************************/
    //habilita las resistencias de pull up PORT D
#ifdef PIC18F4550
    PORTEbits.RDPU = 1;
#endif
    //habilita las resistencias de pull up PORTB
    INTCON2bits.RBPU = 0;
    /****************************************************/
    //Se apagan los leds
    LATBbits.LATB0 = 0;
    LATDbits.LATD7 = 0;
    
    while (1) {
        if ((PORTDbits.RD2) && !(PORTDbits.RD3)) 
        {
            __delay_ms(20);
            if ((PORTDbits.RD2) && !(PORTDbits.RD3))
            {
                LATDbits.LATD7 = 1;
                LATBbits.LATB0 = 0;
                while((PORTDbits.RD2) && !(PORTDbits.RD3));
            }    
        }    
        
        else if (!(PORTDbits.RD2) && (PORTDbits.RD3)) 
        {
            __delay_ms(20);
            if (!(PORTDbits.RD2) && (PORTDbits.RD3))
            {
                LATDbits.LATD7 = 1;
                LATBbits.LATB0 = 1;
                while(!(PORTDbits.RD2) && (PORTDbits.RD3));
            }
        } 
        else if (!(PORTDbits.RD2) && !(PORTDbits.RD3)) 
        {
            __delay_ms(20);
            if (!(PORTDbits.RD2) && !(PORTDbits.RD3))
            {
                LATDbits.LATD7 = 0;
                LATBbits.LATB0 = 0;
                while(!(PORTDbits.RD2) && !(PORTDbits.RD3));
            }
        }
        
        else
        {
            LATDbits.LATD7 = 0;
            LATBbits.LATB0 = 1;
        }
    }
    return;
}
