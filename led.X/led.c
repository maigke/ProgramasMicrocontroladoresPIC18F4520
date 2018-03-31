/*
 * File:   led.c
 * Author: miguel
 *
 * Created on 3 de diciembre de 2017, 10:12 PM
 */


//#include <xc.h>
#include "config.h"
#include "testF.h"
#include "hd44780.h"

#define  _XTAL_FREQ 20000000   //cristal de 20MHz

void main(void) 
{
    
    unsigned char valu;
    TRISB = 0x00; 
    PORTB = 0x00;
    lcd_init();
    valu = 0;
    
    while(valu < 16)
    {   
        send_data(valu+'A');
        valu++;
    }
    
    lcd_ddram(SECOND_LINE);
    
    valu = 0;
    
    while(valu < 16)
    {   
        send_data(valu+'a');
        valu++;
    }
    
    while(1)
    {
        PORTBbits.RB1 = 1;
        __delay_ms(20);
        PORTBbits.RB1 = 0;
        __delay_ms(20);
    }
    return;
}
