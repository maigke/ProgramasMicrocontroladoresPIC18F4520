/*
 * File:   tmrlt4.c
 * Author: miguel
 *
 * Created on 29 de marzo de 2018, 09:53 AM
 * Este programa incrementa el valor PWM 
 * por medio de un pulsador y envia el valor
 * a un display LCD
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


#define ESTADO_ON   0
#define ESTADO_OFF  1


//Valores para PWM
unsigned char estado=0;
unsigned char T0ini;
unsigned char Ton = 0;

char value[8];  //guarda espacio para imprimir un numero

//Interrupciones

void low_priority interrupt low_isr()
{


}

void interrupt high_priority high_isr()
{

    if(INTCONbits.T0IF == 1)
    {
        
        if(estado == ESTADO_ON)
        {
            setbit(LAT(PledVerde),ledVerde);
            T0ini = 255-Ton;
            estado = ESTADO_OFF;
        }
        else
        {
            clrbit(LAT(PledVerde),ledVerde);
            T0ini = Ton;
            estado = ESTADO_ON;
        }
        TMR0=T0ini;
        TMR0IF = 0;
        tglbit(LAT(PledRojo),ledRojo);
        return;
    }

}

void main(void) {
    
    unsigned char val=0;
    Ton = 0;
    clrbit(TRIS(PledVerde),ledVerde);
    clrbit(TRIS(PledRojo),ledRojo);
    setbit(TRIS(PBoton),Boton);
   
   
    /*********************************
     Sección para configurar la interrupcion
     */
    
    //configuracion del Timer 0
    TMR0IE = 1; //habilita interrupciones del TMR0
    T0CS = 0;
    PSA = 0;
    TMR0ON = 0;
    T08BIT = 1; //timer a 8 bits
    T0PS2 = 0; //prescaler a 256
    T0PS1 = 1;
    T0PS0 = 1;
    TMR0IP = 1; //activo en alto para alta prioridad
    
    RCONbits.IPEN = 1; //habilita la prioridad en interrupciones
    GIEH = 1; //habilita las interrupciones de alta prioridad
    GIEL = 0; //deshabilita las interrupciones de baja prioridad
    /*********************************/
   
    lcd_init();
    /*Importante esta linea si esta conectada al display*/ 
    clrbit(LAT(lcd_rw),p_rw);
    lcd_cmd(0x01);
    lcd_gotoxy(1,1);
    lcd_msg("Control PWM");
    lcd_gotoxy(1,2);
    lcd_msg("PWM=");
    sprintf(value,"%3d",val);
    lcd_msg(value);
    
    clrbit(LAT(PledVerde),ledVerde);
    clrbit(LAT(PledRojo),ledRojo);
    TMR0ON = 1;
    while(1)
    {
        if(!(PBoton & (1<<Boton))){
            if(Ton >= 255)
            {
                Ton = 0;//borra precarga
                //borra el buffer
                for(int i = 0; i<sizeof(value)/sizeof(char);i++)
                    value[i]=0;
            }
            else
            {
                Ton++;
            }
            val = Ton;
            sprintf(value,"%3d",val);
            lcd_gotoxy(6,2);
            lcd_msg(value);
        }
        else
        {
            tglbit(LAT(PledRojo),ledRojo);
        }

        
    }
    return;
}
