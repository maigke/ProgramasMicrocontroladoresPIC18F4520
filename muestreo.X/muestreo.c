/*
 * File:   muestreo.c
 * Author: miguel
 *
 * Created on 25 de febrero de 2018, 06:30 PM
 */

#define PIC18F45K50
//#define PIC18F4550
//#define PIC18F452    //se define que micro se va a usar

#ifdef PIC18F4550
#include <config.h>
#include <pic18f4550.h>
#endif

#ifdef PIC18F452
#include <config2.h>
#include <pic18f452.h>
#endif 

#ifdef PIC18F45K50
#include <config3.h>
#include <pic18f45k50.h>
#endif


#include <xc.h>
#define _XTAL_FREQ 4000000

//prototipos de funciones
void timer0_setup(char On, 
                  char mode, 
                  char CS, 
                  char PSA, 
                  int prescale);

int val; 
void high_priority interrupt ISR()
{
    if(INTCONbits.T0IF)
    {
        INTCONbits.T0IF = 0;
        
        //lectura ADC en 10 bits
        //escritura a puerto serial de un int
        TMR0L = 177;
    }
}

void main(void) {
  // Configuracion del timer0
  //Timer 0 - cada 10 ms
  //4MHz clock (1 uS por tick) preescaler - prescaler 128
  //incrementa contador cada 128 us
  //si se cuenta 79 veces 128us entonces son 10.112ms

  //Modo de operacion: Overflow 
  //TMR0ON = 0 apagado
  //modo = 1 8 bits
  //CS = 0 Fuente de reloj del Fclk/4
  //PSA = 0 Salida del timer preescalada
  //Factor de preescalamiento: 128
  //Valor de precarga: 177 = 256-79
    timer0_setup(0,1,0,0,128);
    TMR0L = 177; //precarga del timer 0
    //habilitación de las interrupciones del TMR0
    INTCONbits.T0IE = 1;
    INTCON2bits.T0IP = 1; //alta prioridad
    T0CONbits.TMR0ON = 1; //enciende el temporizador
    while(1);
    return;
}
/**
 * 
 * @param mode 0 -- 16bits / 1 -- 8 bits 
 * @param prescale -- Prescalamiento
 * @return 
 */
void timer0_setup(char On, 
                  char mode, 
                  char CS, 
                  char PSA, 
                  int prescale)
{
    unsigned char clock_mode;
    switch(prescale)
    {
        case 2:clock_mode = 0; break;
        case 4:clock_mode = 1; break;
        case 8:clock_mode = 2; break;
        case 16: clock_mode = 3; break;
        case 32: clock_mode = 4; break;
        case 64: clock_mode = 5; break;
        case 128: clock_mode = 6; break;
        case 256: clock_mode = 7; break;
        default:
            if(prescale <0)
                clock_mode = 7;
    }
    T0CON=(On << 7) | (mode << 6) | 
          (CS << 5) | (PSA << 3) | (clock_mode);
}