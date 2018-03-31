/************************************************************************
@file "keyb.c"
Implementación del modulo keyb.h
@author Miguel Montiel
************************************************************************/
//#define PIC18F452
//#define PIC18F4550
//#define PIC18F4450
#define PIC18F45K50
#define _XTAL_FREQ 4000000
#ifdef PIC18F4550
	#include "config.h"
#endif

#ifdef PIC18F452
	#include "config2.h"
#endif

#ifdef PIC18F45K50
#include "config3.h"
#endif

#ifdef PIC18F4450
    #include <config4.h>
#endif


#include "keyb.h"
#include <xc.h>
/**
@details Inicializa como salidas a PORT_KEY_Rx y como entradas a PORT_KEY_Cx
se escribe tambien a PORT_KEY_Cx un valor alto con la finalidad de habilitar las
reistencias de pull up 
*/
void vInitKeyPad(void)
{
	//Se configuran entradas
	setbit(TRIS(PORT_KEY_C1),PIN_KEY_C1);
	setbit(TRIS(PORT_KEY_C2),PIN_KEY_C2);
	setbit(TRIS(PORT_KEY_C3),PIN_KEY_C3);
	setbit(TRIS(PORT_KEY_C4),PIN_KEY_C4);
	//Se configuran salidas
	clrbit(TRIS(PORT_KEY_R1), PIN_KEY_R1);
	clrbit(TRIS(PORT_KEY_R2), PIN_KEY_R2);
	clrbit(TRIS(PORT_KEY_R3), PIN_KEY_R3);
	clrbit(TRIS(PORT_KEY_R4), PIN_KEY_R4);
	

}

/**
@details comienza por asignar a una variable k un cero y la va "girando" por las 
filas y lee el valor de las columnas con la finalidad de saber el valor de k o 
tecla presionada, el cual devuelve.
 * 
 * Organización del teclado keypad-smallcalc
 * 
 * | 7 | 8 | 9 | / |
 * -----------------
 * | 4 | 5 | 6 | X |
 * -----------------
 * | 1 | 2 | 3 | - |
 * -----------------
 * |on | 0 | = | + |
 * 
 * La funcion regresa valores de acuerdo a la siguiente tabla
 * 
 * | 0 | 1 | 2 | 3 |
 * -----------------
 * | 4 | 5 | 6 | 7 |
 * -----------------
 * | 8 | 9 | A | B |
 * -----------------
 * | C | D | E | F |
 * 
*/

unsigned char const KEYS[4][4]={ {'1','2','3','A'},
                        {'4','5','6','B'},
                        {'7','8','9','C'},
                        {'*','0','#','D'}};

unsigned char kbd_getc()
{
    unsigned char kchar,temp;
    
    temp = ui8_TestKeyPad();
    if (temp <=3)            
        kchar = KEYS[0][temp];
    if (temp >=4 || temp <=7)
            kchar = KEYS[1][temp-4];
    if (temp >=8 || temp <=11)
            kchar = KEYS[2][temp-8];
    if (temp >=12 || temp <=15)
            kchar = KEYS[3][temp-12];
    if(temp == 16)
        kchar = '\0';
    return kchar;
}

unsigned char ui8_TestKeyPad()
{
	unsigned char k;
	for(k=0;k<16;k++)
	{
		switch(k)
		{
			case 0: vSetKeyPadR1();
            //__delay_ms(50);
			break;
			case 4: vSetKeyPadR2();
            //__delay_ms(50);
			break;
			case 8: vSetKeyPadR3();
            //__delay_ms(50);
			break;
			case 12: vSetKeyPadR4();
            //__delay_ms(50);
			break;
			default: vSetKeyPad();
		}
		
		
		if (!((PORT_KEY_C1) & (1 << PIN_KEY_C1)))
		{
			//__delay_ms(20);
			//if (!((PORT_KEY_C1) & (1 << PIN_KEY_C1)))
			{vSetKeyPad();
			return k;
			}			
		} k++;
		
		if (!((PORT_KEY_C2) & (1 << PIN_KEY_C2)))
		{
			//__delay_ms(20);
			//if (!((PORT_KEY_C2) & (1 << PIN_KEY_C2)))
			{vSetKeyPad();
				return k;
			}
		}k++;
		
		if (!((PORT_KEY_C3) & (1 << PIN_KEY_C3)))
		{
			//__delay_ms(20);
			//if (!((PORT_KEY_C3) & (1 << PIN_KEY_C3)))
			{vSetKeyPad();
				return k;
			}
		}k++;
		
		if (!((PORT_KEY_C4) & (1 << PIN_KEY_C4)))
		{
			//__delay_ms(20);
			//if (!((PORT_KEY_C4) & (1 << PIN_KEY_C4)))
			{vSetKeyPad();
				return k;
			}
		}
	}
	vSetKeyPad();
	return (16);
}
