/*
 * File:   Hola_7.c
 * Author: miguel
 *
 * Created on 15 de enero de 2018, 08:28 AM
 * Este programa lee un pulso emitido por un sensor de proximidad
 * y este valor se ve reflejado en dos leds, usado como contador
 * de 4 estados, al llegar al valor 3 el contador se reiniciara
 * 
 * Definiciones
 * Led0 RD0
 * Led1 RD1
 * Pulso PC2
 * Se realiza la visualización por contador
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


#define Led_0  LATDbits.LATD0  //salida
#define Led_1  LATDbits.LATD1  //salida
#define Pulso PORTCbits.RC0    //Entrada
//configurar entradas y salidas
#define cLed_0 TRISDbits.TRISD0
#define cLed_1 TRISDbits.TRISD1
#define cPulso TRISCbits.TRISC0
 

#include <xc.h>
#define _XTAL_FREQ 20000000

void display(unsigned char valor);


void main(void) {
    
    //variable a utilizar
    unsigned char cont = 0;
    //configurando entradas y salidas
    cLed_0 = 0;
    cLed_1 = 0;
    cPulso = 1;
    
    while(1)
    {
        if(!Pulso)
        {
            __delay_ms(10);
            if(!Pulso)
            { 
                if(cont >= 3)
                {
                    cont = 0;
                }
                else
                {
                    cont++;
                }
                while(!Pulso);
            }    
        }
        display(cont);
    }
    return;
}

void display(unsigned char valor)
{

    if (valor & (1 << 0)) Led_0 = 1;
    else Led_0 = 0;
    
    if (valor & (1 << 1)) Led_1 = 1;
    else Led_1 = 0;
}