/*
 * File:   tmrIt.c
 * Author: miguel
 *
 * Created on 27 de marzo de 2018, 09:05 AM
 * 
 * Encender y apagar un led con interrupciones
 * generadas por el timer 0
 * 
 * 
 * led Verde en RC2
 * boton RC1
 * 
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

#define PledVerde  PORTC
#define ledVerde   2
#define PledRojo  PORTC
#define ledRojo   3
#define PBoton      PORTC
#define Boton       1


#define T0INI 10
#define T1INI 100
char value[8];  //guarda espacio para imprimir un numero
volatile char tick_count;
#include <xc.h>


//Interrupciones

void low_priority interrupt low_isr()
{
        if(INTCONbits.T0IF == 1)
    {
        TMR0 = T0INI;
        tglbit(LAT(PledRojo),ledRojo);
        TMR0IF = 0;
        return;
    }

}

void interrupt high_priority high_isr()
{

    if(PIR1bits.TMR1IF == 1)
    {
        TMR1 = T1INI;
        tglbit(LAT(PledVerde),ledVerde);
        TMR1IF = 0;
        return;
    }

}

void main(void) {
    
    clrbit(TRIS(PledVerde),ledVerde);
    clrbit(TRIS(PledRojo),ledRojo);
    setbit(TRIS(PBoton),Boton);
    tick_count = 0;
    /*********************************
     Sección para configurar la interrupcion
     */
    //INTCON = 0b00000000; //valores del INTCON modificar
    //ADCON0bits.ADON = 0;
    //configuracion del Timer 0
    TMR0IE = 1; //habilita interrupciones del TMR0
    T0CS = 0;
    PSA = 0;
    TMR0ON = 0;
    T08BIT = 1; //timer a 8 bits
    T0PS2 = 1; //prescaler a 256
    T0PS1 = 1;
    T0PS0 = 1;
    TMR0 = T0INI;
    
    
    TMR0IP = 0; //activo en bajo para baja prioridad
    TMR1IP = 1; //activo en bajo para baja prioridad
    
    //Configuracion del Timer 1
    PIE1bits.TMR1IE = 1;
    T1CONbits.RD16 = 1;  //lectura en una operacion de 16 bits
    T1CKPS1 = 1; //prescaler por 8 
    T1CKPS0 = 1;
    TMR1CS = 0;  //depende del Fosc
    TMR1ON = 0;  //Timer encendido
    
    RCONbits.IPEN = 1;
    GIEH = 1; //habilita las interrupciones de alta prioridad
    GIEL = 1; //habilita las interrupciones de baja prioridad
    /*********************************/
    
    clrbit(LAT(PledVerde),ledVerde);
    clrbit(LAT(PledRojo),ledRojo);
    for( int i = 0 ;i < 10; i++)
            __delay_ms(100);
    TMR1ON = 1;
    TMR0ON = 1;
    while(1)
    {
      //  if(!(PBoton & (1<<Boton))){
      //      clrbit(LAT(PledVerde),ledVerde);
      //  }
      //  else
      //  {setbit(LAT(PledVerde),ledVerde);}

        
    }
    return;
}


