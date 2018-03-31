/*
 * File:   puertoA.c
 * Author: miguel
 *
 * Created on 6 de febrero de 2018, 04:48 PM
 */


#include <xc.h>

void main(void) {
    
    PORTA = 0x00;
    LATA = 0x00;
    
    //configurar los pines AD como E/S TTL
    ADCON1 = 0x0F;
    //configurar los pines CA como E/S TTL
    CMCON = 0x07;
    //configurar RA<3..0> como entrada
    //configurar RA<5..4> como salida
    TRISA = 0xCF;
    
}
