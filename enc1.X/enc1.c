/*
 * File:   enc1.c
 * Author: miguel
 *
 * Created on 29 de marzo de 2018, 02:54 PM
 * Enciende un motor M1 a velocidad completa
 * Mide la velocidad en INT_EXT_0
 * Muestra la velocidad en lcd
 * para calcular la velocidad se cuentan los
 * pulsos del encoder cada 1 ms
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


//Definiciones de los pines
#define PM1 PORTB
#define M1  0
#define PM2 PORTB
#define M2  1
#define Pm1aa PORTB
#define m1aa 2
#define Pm1bb PORTB
#define m1bb 3
#define Pm1en PORTB
#define m1en 4
#define Pm2aa PORTB
#define m2aa 5
#define Pm2bb PORTB
#define m2bb 6
#define Pm2en PORTB
#define m2en 7


#include <xc.h>
#include <lcd4a.h>

char value[8];  //guarda espacio para imprimir un numero
volatile int cont;

void low_priority interrupt low_isr()
{


}

void interrupt high_priority high_isr()
{
    if(INT0IF == 1)
    {
        cont++;
        
        INT0IF = 0;
        return;
    }
    
    if(TMR1IF == 1)
    {
        
    }
}

void main(void)
{
    cont = 0;
    TRISB = 0x03; //configuracion del puertoB
    lcd_init();
    lcd_msg(" Habilita int's");
    
    /*
     * Sección para configurar la Interrupcion
     */
    INT0IE = 1;  //habilita las interrupciones 
    INTEDG0 = 1; //Interrupcion en flanco de subida
    
    
    //Configuracion global de las interrupciones
    IPEN = 1; //Interrupciones por prioridad
    GIEH = 1;
    GIEL = 1;
    /******************************************/
    
    //Arranca el motor
    setbit(LAT(Pm1en),m1en);
    setbit(LAT(Pm1aa),m1aa);
    clrbit(LAT(Pm1bb),m1bb);
    /*************PROGRAMA PRINCIPAL++++++++++*/
    while(1)
    {
        __delay_ms(980);
        sprintf(value,"%4d",cont);
        lcd_gotoxy(1,2);
        lcd_msg(value);
    }
    return;
}
