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
volatile int rpm;
#define T1INI 53036

void low_priority interrupt low_isr()
{
    
    if(PIR1bits.TMR1IF == 1)
    {
        TMR1ON = 0;
        TMR1 = T1INI;
        rpm = cont*5/48;
        rpm= rpm*60;
        cont = 0;
        TMR1IF = 0;
        TMR1ON = 1;
        return;
    }

}

void interrupt high_priority high_isr()
{
    if(INT0IF == 1)
    {
        cont++;
        
        INT0IF = 0;
        return;
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
    
    /**
     * Configurar el Timer 1 para que genere una 
     * interrupcion de 1 ms y cuente los pulsos
     * 
     */
    TMR1IP = 0; //activo en bajo para baja prioridad
    
    //Configuracion del Timer 1
    PIE1bits.TMR1IE = 1;
    T1CONbits.RD16 = 1;  //lectura en una operacion de 16 bits
    T1CKPS1 = 1; //prescaler por 8 
    T1CKPS0 = 1;
    TMR1CS = 0;  //depende del Fosc
    TMR1ON = 0;  //Timer apagado
    TMR1 = T1INI;  //precarga para que desborde cada 10 milisegundo
    
    
    //Configuracion global de las interrupciones
    IPEN = 1; //Interrupciones por prioridad
    GIEH = 1;
    GIEL = 1;
    /******************************************/
    
    //Arranca el motor
    setbit(LAT(Pm1en),m1en);
    setbit(LAT(Pm1aa),m1aa);
    clrbit(LAT(Pm1bb),m1bb);
    TMR1ON = 1;
    /*************PROGRAMA PRINCIPAL++++++++++*/
    while(1)
    {
        __delay_ms(980);
        sprintf(value,"%4d",rpm);
        lcd_gotoxy(5,2);
        lcd_msg(value);
    }
    return;
}
