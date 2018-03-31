/************************************************************************
@file "sevseg.c"
Implementación del modulo sevseg.h
@author Miguel Montiel
************************************************************************/
#include <avr/io.h>
#include "clk.h"
#include "sevseg.h"



/**
@details Configura como salidas los pines y puertos definidos en sevseg.h
*/
void InitSegSeg()
{
	setbit(DDR(pseg_a), seg_a);
	setbit(DDR(pseg_b), seg_b);
	setbit(DDR(pseg_c), seg_c);
	setbit(DDR(pseg_d), seg_d);
	setbit(DDR(pseg_e), seg_e);
	setbit(DDR(pseg_f), seg_f);
	setbit(DDR(pseg_g), seg_g);
	#ifdef USE_DP
		setbit(DDR(pdp), dp);
	#endif
}

/**
@details Recibe un numero en la variable value, lo clasifica y analiza para sacarlo por los
pines seleccionados en el modulo sevseg.h sin importar su orden de conexion, por lo cual 
este modulo es flexible
*/
void Print(unsigned char value)
{
	unsigned char out;
	
	switch(value)
	{
		case 0:
		out =  0x3f;
		break;
		case 1:
		out = 0x06;
		break;
		case 2:
		out = 0xdb;
		break;
		case 3:
		out = 0x4f;
		break;
		case 4:
		out = 0x66;
		break;
		case 5:
		out = 0x6d;
		break;
		case 6:
		out = 0x7d;
		break;
		case 7:
		out = 0x07;
		break;
		case 8:
		out = 0x7f;
		break;
		case 9:
		out = 0x6f;
		break;
		case 10:
		out = 0x77;
		break;
		case 11:
		out = 0x7c;
		break;
		case 12:
		out = 0x58;
		break;
		case 13:
		out = 0x5e;
		break;
		case 14:
		out = 0x7b;
		break;
		case 15:
		out = 0x71;
		break;
		case 16:
		out = 0x00; //apaga los leds
	}
	//se inicia prueba de bits
	if (out & (1<< 0)) setbit(pseg_a, seg_a);
	else clrbit(pseg_a, seg_a);
	
	if (out & (1<< 1)) setbit(pseg_b, seg_b);
	else clrbit(pseg_b, seg_b);
	
	if (out & (1<< 2)) setbit(pseg_c, seg_c);
	else clrbit(pseg_c, seg_c);
	
	if (out & (1<< 3)) setbit(pseg_d, seg_d);
	else clrbit(pseg_d, seg_d);
	
	if (out & (1<< 4)) setbit(pseg_e, seg_e);
	else clrbit(pseg_e, seg_e);
	
	if (out & (1<< 5)) setbit(pseg_f, seg_f);
	else clrbit(pseg_f, seg_f);
	
	if (out & (1<< 6)) setbit(pseg_g, seg_g);
	else clrbit(pseg_g, seg_g);
	
	#ifdef USE_DP
		if (out & (1<< 7)) setbit(pdp, dp);
		else clrbit(pdp,dp);
    #endif

}