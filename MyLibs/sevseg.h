/** @file sevseg.h
@brief Libreria para siete segmentos de catodo comun
   Es posible usar anodo comun a traves de interfaz con ULN2803 o ULN2003                                                                    

@author Miguel Montiel Mtz*/
#ifndef SEVSEG_H
#define SEVSEG_H

/************************************************************************/

/************************************************************************/

#define pseg_a PORTB
#define seg_a 6

#define pseg_b PORTB
#define seg_b 5

#define pseg_c PORTD
#define seg_c 7

#define pseg_d PORTD
#define seg_d 6

#define pseg_e PORTD
#define seg_e 4

#define pseg_f PORTB
#define seg_f 4

#define pseg_g PORTD
#define seg_g 5

//#define USE_DP 1
#ifdef USE_DP
#define pdp
#define dp 
#endif

/**
@name Functions
*/
/**
@fn InitSegSeg
@brief Funcion que inicializa al display

@param none
@return none

@verbatim
InitSegSev();
@endverbatim
*/

void InitSegSeg(void);
/**
 @fn Print
*@brief Funcion que permite imprimir un numero pasado como binario a 7 segmentos
 en el formato Xgfedcba
 
 @param numero tipo char sin signo que se pretende imprimir valido del 0x00 al 0x0F
 
 @return none.- Esta funcion no devuelve valores
 
 @verbatim
 
 unsigned char valor = 0x0A;
 Print(valor);
 
 @endverbatim
*/
void Print(unsigned char value);
#endif