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
 * 
 * Se realiza una calibracion de acuerdo a lo que se muestra ne la
 * siguiente página
 * 
 * https://arduinoplusplus.wordpress.com/2015/07/15/tcs230tcs3200-sensor-calibration/
 * 
 * los valores obtenidos despues de 10 lecturas, eliminando los valores 
 * máximos y mínimos son
 * 
 * Para color negro mate
 * 
 * fd(blanco) = 588.5
 * fd(rojo) = 743.4
 * fd(verde) = 505.7
 * fd(azul) = 1883
 * 
 * y para color blanco
 * fw(blanco)= 1821
 * fw(rojo) = 1928
 * fw(verde)= 1498
 * fw(azul) = 4824
 * 
 * La ecuacion para obtener el RGB correspondiente viene dado por
 * 
 * RGB = 255(fo - fd)/(fw - fd), donde fo es el valor leido
 */



#include <stdio.h>
#include <stdlib.h>

#define PIC18F4520
//#define PIC18F4550
//#define PIC18F45K50

#define _XTAL_FREQ 16000000
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


#define MAX 30
#define Pled PORTD
#define led  4


//definiciones para de la calibración
#define fwb 1821
#define fwr 1928
#define fwv 1498
#define fwa 4824

#define fdb 588.5
#define fdr 743.4
#define fdv 505.7
#define fda 1883



typedef enum{

    false = 0,
    true = 1
} boolean;

//para pasar argumentos de color al sensor a traves de S2 y S3
//S2 es el bit mas significativo
typedef enum{ rojo,azul,sf,verde} color;
typedef enum{total, media, baja,apagado} escala;


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


/* Prototipos*/
float calcula();
void prepareForInterrupts ();
void seleccionarEscala(escala a);
void seleccionarColor(color a);
float calibra(float valor, float fd, float fw);

/**   Pines para conexion del sensor    ************/
#define Ps3 PORTD
#define s3  3
#define Ps2 PORTD
#define s2  2
#define Ps1 PORTD
#define s1  1
#define Ps0 PORTD
#define s0  0

void main(void)
{
    char i;
    float freq=0.0;
    float r,v,a,brillo;
    //Zona de configuraciones
    clrbit(TRIS(Ps3),s3);
    clrbit(TRIS(Ps2),s2);
    clrbit(TRIS(Ps1),s1);
    clrbit(TRIS(Ps0),s0);
    clrbit(TRIS(Pled),led);
    
    lcd_init();
    lcd_msg("Detector de color");
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
    
    //define el rango de operacion
    //Por las características del reloj se recomienda usar 
    //los rangos de operacion mas bajos 2%
    // que vande 10 a 12 kHz
    
    seleccionarEscala(baja);
    //seleccionarColor(rojo);
    
   
    while(1)
    {
        /* Se lee el brillo*/
        tglbit(LAT(Pled),led);
        seleccionarColor(sf);
        for (i = 0 ;i < MAX; i++)
        {   
            __delay_ms(10);
            freq+= calcula();
        }
        freq/=MAX;
        brillo = freq;
        freq = calibra(brillo,fdb,fwb);
        sprintf(value,"%5.3f",freq);
        lcd_gotoxy(1,2);
        lcd_msg("B= ");
        lcd_msg(value);
        freq=0.0;
        __delay_ms(1000);
        
        
        /** Se lee el color Rojo*/
        tglbit(LAT(Pled),led);
        seleccionarColor(rojo);
        for (i = 0 ;i < MAX; i++)
        {   
            __delay_ms(10);
            freq+= calcula();
        }
        freq/=MAX;
        r=freq;
        freq = calibra(r,fdr,fwr);
        sprintf(value,"%5.3f",freq);
        lcd_gotoxy(1,2);
        lcd_msg("R= ");
        lcd_msg(value);
        
        freq=0.0;
        __delay_ms(1000);
       
       /** Se lee el color Verde*/
        tglbit(LAT(Pled),led);
        seleccionarColor(verde);
        for (i = 0 ;i < MAX; i++)
        {   
            __delay_ms(10);
            freq+= calcula();
        }
        freq/=MAX;
        v=freq;
        freq = calibra(v,fdv,fwv);
        sprintf(value,"%5.3f",freq);
        lcd_gotoxy(1,2);
        lcd_msg("V= ");
        lcd_msg(value);
        freq=0.0;
        __delay_ms(1000);
        
        /** Se lee el color Azul*/
        tglbit(LAT(Pled),led);
        seleccionarColor(azul);
        for (i = 0 ;i < MAX; i++)
        {   
            __delay_ms(10);
            freq+= calcula();
        }
        freq/=MAX;
        a=freq;
        freq = calibra(a,fda,fwa);
        sprintf(value,"%5.3f",freq);
        lcd_gotoxy(1,2);
        lcd_msg("A= ");
        lcd_msg(value);
        freq=0.0;
        __delay_ms(1000);
        
        freq=min(a,r);
    }
    
    return;
}


float  calcula()
{
    float freq = 0.0;
    float temp = 0.0;
    if (!triggered)
    {
    //lcd_gotoxy(5,2);
    //lcd_msg(":)");
    return;
    }

  unsigned long elapsedTime = finishTime - startTime;
  
  temp = (float) (elapsedTime);
  temp *= 250e-9;  //cada tick es de 250ns para Xt = 16MHz
  freq = 1.0/(temp);
  prepareForInterrupts ();
  return freq;
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


/**
 * seleccionarEscala: Permite seleccionar la escala de frecuencia de salida
 * Para el TCS2300 de acuerdo al valor pasado a la variable tipo escala
 * de acuerdo a la siguiente enumeracion
 * 
 * typedef enum{total, media, baja,apagado} escala;
 * 
 * los valores que asignan son referidos de acuerdo a la hoja de datos
 * 
 * @param a  
 */
void seleccionarEscala (escala a)
{
    switch(a)
    {
    case apagado:
        clrbit(LAT(Ps0),s0);
        clrbit(LAT(Ps1),s1);
        break;
    case baja:
        clrbit(LAT(Ps0),s0);
        setbit(LAT(Ps1),s1);
        break;
    case media:
        setbit(LAT(Ps0),s0);
        clrbit(LAT(Ps1),s1);
        break;
    case total:
        setbit(LAT(Ps0),s0);
        setbit(LAT(Ps1),s1);
        break;
    }
}

/**
 * seleccionarColor Permite seleccionar el filtro de color para deteccion
 * Para el TCS2300 de acuerdo al valor pasado a la variable tipo color 
 * de acuerdo a la siguiente enumeracion:
 * 
 * typedef enum{ rojo,azul,sf,verde} color;
 * 
 * donde sf es sin filtro
 * @param col
 * 
 * si se llama con otro valor, por defecto se activa la opcion sin filtro
 */
void seleccionarColor(color col)
{
    switch(col)
    {
    case rojo:
        clrbit(LAT(Ps2),s2);
        clrbit(LAT(Ps3),s3);
        break;
    case azul:
        clrbit(LAT(Ps2),s2);
        setbit(LAT(Ps3),s3);
        break;
    case sf:
        setbit(LAT(Ps2),s2);
        clrbit(LAT(Ps3),s3);
        break;
    case verde:
        setbit(LAT(Ps2),s2);
        setbit(LAT(Ps3),s3);
        break;
    default:
        setbit(LAT(Ps2),s2);
        clrbit(LAT(Ps3),s3);
    }
}

float calibra(float valor, float fd, float fw)
{
    return (255*(valor-fd))/(fw-fd);
}