/*
 * File:   Hola_4.c
 * Author: miguel
 *
 * Created on 8 de enero de 2018, 07:18 PM
 */
#define PIC18F4550
//#define PIC18F452    //se define que micro se va a usar

#ifdef PIC18F4550
    #include <config.h>
    #include <pic18f4550.h>
#else
    #include <config2.h>
    #include <pic18f452.h>
#endif    

#include <xc.h>
#define _XTAL_FREQ 20000000

void main(void) {
    asm("MOVLW 0");
    asm("MOVWF LATD");
    asm("MOVLW 0");
    asm("MOVWF LATB");
    
    /*1nput --- 1
    0utput  -- 0 */
    TRISDbits.TRISD7 = 0;
    TRISBbits.TRISB0 = 0;
    
    //habilita las resistencias de pull up PORT D
#ifdef PIC18F4550
    PORTEbits.RDPU = 1;
#endif
    //habilita las resistencias de pull up PORTB
    INTCON2bits.RBPU = 0;
    
    LATDbits.LATD7 = 0;
    LATBbits.LATB0 = 0;
    while(1)
    {
        LATDbits.LATD7 = 0;
        LATBbits.LATB0 = 0;
        __delay_ms(500);
        
        LATDbits.LATD7 = 1;
        LATBbits.LATB0 = 0;
        __delay_ms(500);
        
        LATDbits.LATD7 = 0;
        LATBbits.LATB0 = 1;
        __delay_ms(500);
        
        LATDbits.LATD7 = 1;
        LATBbits.LATB0 = 1;
        __delay_ms(500);
    }
    return;
}
