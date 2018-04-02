/*
 * File:   enc2.c
 * Author: miguel
 *
 * Created on 2 de abril de 2018, 09:05 AM
 * Se crea un programa para leer un encoder incremental
 * por medio de las interrupciones Cambio de bit
 * El encoder incremental se considera que tiene un total
 * de 48 pasos y el motor al cual está conectado 
 * tiene una velocidad de 100 rpm
 * 
 * Para determinar la velocidad del motor, se mide a 
 * el tiempo entre 
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
#define Penc1a PORTB
#define enc1a  0
#define Penc2a PORTB
#define enc2a  1
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
#define Penc1b PORTD
#define enc1b 0
#define Penc2b PORTD
#define enc2b 1

#define LED1a PORTCbits.RC0
#define LED1b PORTCbits.RC1

#include <xc.h>
#include <lcd4a.h>

char value[8];  //guarda espacio para imprimir un numero
volatile int contM1;
volatile int contM2;
volatile float rpm1;
volatile float rpm2;
#define T1INI 53036  //100 ms
//#define T1INI 64286 //10ms


//prototipos
void cwM1();
void ccwM1();
void stopM1();
void cwM2();
void ccwM2();
void stopM2();
void Adelante();
void Atras();

void low_priority interrupt low_isr()
{
    
    if(PIR1bits.TMR1IF == 1)
    {
        TMR1ON = 0;
        TMR1 = T1INI;
        rpm1 = contM1*10.0*60.0;
        rpm1= rpm1/48.0;
        rpm2 = contM2;
        contM1 = 0;
        contM2 = 0;
        TMR1IF = 0;
        TMR1ON = 1;
        return;
    }

}

void interrupt high_priority high_isr()
{
    if(INT0IF == 1)
    {
        /*
         * Si después del flanco de bajada hay un cero
         * en ENC1B se debe incrementar el contador
         */
        if(Penc1a&(1<<enc1a))
        {
            INTEDG0 = 0; //Interrupcion en flanco de bajada
            if((Penc1b&(1<<enc1b)))
            {
                contM1++;
               // LED1a = 1;
               // LED1b = 0;
            }
        }
        else
        {
            INTEDG0 = 1; //Interrupcion en flanco de subida
            if((Penc1b & (1<<enc1b)))
            {
                contM1--;
               // LED1a = 0;
               // LED1b = 1;
            }
        }
            INT0IF = 0;
        return;
    }

    if(INT1IF == 1)
    {
        /*
         * Si después del flanco de bajada hay un cero
         * en ENC1B se debe incrementar el contador
         */
        if(Penc2a&(1<<enc2a))
        {
            INTEDG1 = 0; //Interrupcion en flanco de bajada
            if((Penc2b&(1<<enc2b)))
            {
                contM2++;
                LED1a = 1;
                LED1b = 0;
            }
        }
        else
        {
            INTEDG1 = 1; //Interrupcion en flanco de subida
            if((Penc2b & (1<<enc2b)))
            {
                contM2--;
                LED1a = 0;
                LED1b = 1;
            }
        }
            INT1IF = 0;
        return;
    }    
}

#include <xc.h>

void main(void)
{
    contM1 = 0;
    TRISB = 0x03; //configuracion del puertoB
    setbit(TRIS(Penc1b),enc1b);
    setbit(TRIS(Penc2b),enc2b);
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;
    LED1a = 0;
    LED1b = 0;
    lcd_init();
    lcd_msg(" Habilita int's");
    
    /*
     * Sección para configurar la Interrupcion
     * Externa 0
     */
    INT0IE = 1;  //habilita las interrupciones 
    INTEDG0 = 1; //Interrupcion en flanco de subida
    
    /*
     * Sección para configurar la Interrupcion
     * Externa 1
     */
    INT1IE = 1;
    INTEDG1 = 1; //Interrupcion por flanco de subida
    INT1IP = 1;  //interrupcion de alta prioridad
    
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
    
    Adelante();
    
    TMR1ON = 1;
    /*************PROGRAMA PRINCIPAL++++++++++*/
    while(1)
    {
        __delay_ms(980);
        sprintf(value,"%4.3f",rpm2);
        lcd_gotoxy(5,2);
        lcd_msg(value);
        
    }
    return;
}


void cwM1()
{
    setbit(LAT(Pm1en),m1en);
    setbit(LAT(Pm1aa),m1aa);
    clrbit(LAT(Pm1bb),m1bb);    
}

void ccwM1()
{
    setbit(LAT(Pm1en),m1en);
    clrbit(LAT(Pm1aa),m1aa);
    setbit(LAT(Pm1bb),m1bb);
}

void stopM1()
{
    clrbit(LAT(Pm1en),m1en);
    clrbit(LAT(Pm1aa),m1aa);
    clrbit(LAT(Pm1bb),m1bb);
}

void cwM2()
{
    setbit(LAT(Pm2en),m2en);
    setbit(LAT(Pm2aa),m2aa);
    clrbit(LAT(Pm2bb),m2bb);    
}

void ccwM2()
{
    setbit(LAT(Pm2en),m2en);
    clrbit(LAT(Pm2aa),m2aa);
    setbit(LAT(Pm2bb),m2bb);
}

void stopM2()
{
    clrbit(LAT(Pm2en),m2en);
    clrbit(LAT(Pm2aa),m2aa);
    clrbit(LAT(Pm2bb),m2bb);
}

void Adelante()
{
    cwM1();
    ccwM2();
}

void Atras()
{
    ccwM1();
    cwM2();
}