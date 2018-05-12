/*
 * 
 * Original: Basado en la versión de Nick Gammon para Arduino
 * Puede consultar la fuente aqui
 * https://github.com/tehniq3/tachometer-RPM-/blob/master/RPM_intr_nick_gammon_1.ino
 * 
 * File:   color_1.c
 * Author: miguel
 *
 * Created on 13 de abril de 2018, 04:22 PM
 * Programa para leer la interrupcion externa
 * cada flanco de subida, de un tren de pulsos
 * que sale un sensor de color TCS230 compatible
 * 
 * Se usara el Timer 1 para determinar el periodo
 * de la señal y establecer un valor promedio 
 * para la detección del color
 */



#include <stdio.h>
#include <stdlib.h>

#define PIC18F4520
//#define PIC18F4550
//#define PIC18F45K50

#define _XTAL_FREQ 4000000
#include <xc.h>
#include <lcd4a.h>

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


#define MAX 10

/* Prototipos*/
void calcula();
void prepareForInterrupts ();


typedef enum{

    false = 0,
    true = 1
} boolean;

volatile boolean first;
volatile boolean triggered;
volatile unsigned long overflowCount;
volatile unsigned long startTime;
volatile unsigned long finishTime;

void low_priority interrupt low_isr()
{
    if(TMR1IF == 1)
    {
        overflowCount++;
        TMR1IF =0;
        return;
    }
}

void interrupt high_priority high_isr()
{
    
    if(INT0IF == 1)
    {
        unsigned int counter = TMR1;  // quickly save it
 
        // wait until we noticed last one
        if (triggered)
        {
            INT0IF = 0;
          return;
        }

        if (first)
          {
          startTime = (overflowCount << 16) + counter;
          first = false;
          INT0IF = 0;
          return; 
          }

        finishTime = (overflowCount << 16) + counter;
        triggered = true;
        INT0IF = 0;
        return;
    }
}



void main(void)
{
    char acum= 0;
    //Zona de configuraciones
    
    
    lcd_init();
    lcd_msg("Frecuencia");
    /**
     * Configurar el Timer 1 para que genere una 
     * interrupcion de 1 us y cuente los pulsos
     * 
     */
    //Configuracion del Timer 1
    T1CONbits.RD16 = 1;  //lectura en una operacion de 16 bits
    T1CKPS1 = 0; //prescaler por 1
    T1CKPS0 = 0;
    TMR1CS  = 0;  //depende del Fosc
    TMR1ON  = 1;  //Timer apagado
    TMR1IP  = 0;  //prioridad del timer 1 baja
    TMR1IE  = 1;  //habilitacion de la interrupcion
    
    prepareForInterrupts ();
    

    //Configuracion global de las interrupciones
    IPEN = 1; //Interrupciones por prioridad
    GIEH = 1;
    GIEL = 1;
    /******************************************/ 
   
   
    while(1)
    {
       __delay_ms(1000);     
       calcula();     
    }
    
    return;
}


void calcula()
{
    float freq = 0.0;
    float temp = 0.0;
    if (!triggered)
    {
    lcd_gotoxy(5,2);
    lcd_msg(":)");
    return;
    }

  unsigned long elapsedTime = finishTime - startTime;
  
  temp = (float) (elapsedTime);
  temp *= 1e-6;
  freq = 1.0/(temp);
  //freq = 1.0 / ((float (elapsedTime) * 1e-6));  // each tick is 62.5 nS
 
  //Serial.print (freq);
  sprintf(value,"%5.3f",freq);
  lcd_gotoxy(5,2);
  lcd_msg(value);
        
  
  prepareForInterrupts ();
  }

void prepareForInterrupts ()
  {
  // get ready for next time
  INT0IF == 0;// clear flag for interrupt 0
  first = true;
  triggered = false;  // re-arm for next time
       /*
     * Sección para configurar la Interrupcion
     * Externa 0
     */
    INT0IE  = 1;  //habilita las interrupciones 
    INTEDG0 = 1; //Interrupcion en flanco de subida    
  }  // end of prepareForInterrupts