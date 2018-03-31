/*****************************************************************************
******************************************************************************
****                           COMPILADOR SDCC                           ****
****                 Libreria Lcd 4Bit para LDC 44780                    ****
****                         V.01                                        ****
/ This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//------------------------------------------------------------------------
*************Historial de Cambios.********************************************
1- Reescrito y Modifcador por RiSanti ,foro Piclinux
2- Version Original por arcachofo  ,foro Piclinux
******************************************************************************
************************Ejemplo de Uso****************************************

#define RS RC1			//PIN PARA RS.
#define RW RC2			//PIN PARA RW.
#define EN RC3			//PIN PARA ENABLE.
#define lcdPortC	        // Para usar Portc, para datos e 4Bit (ultimos 4bits)
#define FREQ 4000		// 4000 KHz = 4 Mhz, LA FRECUENCIA EN KHz.

********************************************************************************
****************************Definiciones por defecto****************************/
   //Frecuencia de Operacion de 4MHZ
#ifndef FREQ
#define   FREQ  4000
#endif

#ifdef   lcdPortB
#define   lcdPort  TRISB
#define   lcdDato  PORTB
#endif
#ifdef   lcdPortC
#define   lcdPort  TRISC
#define   lcdDato  PORTC
#endif
#ifdef   lcdPortD
#define   lcdPort  TRISD
#define   lcdDato  PORTD
#endif
   //Por defecto definicion del Puerto B
#ifndef   lcdPort
#define   lcdPort  TRISB
#define   lcdDato  PORTB
#endif
  //Definicion de Pines de Control RS,RW,EN
#ifndef   RS
#define   RS  RB1
#endif
#ifndef   RW
#define   RW  RB2
#endif
#ifndef   EN
#define   EN  RB3
#endif
  //Defincion de Registro de GRam del Lcd
#define LCDLinea2 0x40
#define LCDLinea3 0x10
#define LCDLinea4 0x50
//>>>>>>>>>>>>>>Señales de Control>>>>>>>>>>>>>>//
#define EN1    EN=1; __asm nop   __endasm
#define EN0    EN=0; __asm nop  __endasm
#define RW1    RW=1; __asm nop   __endasm
#define RW0    RW=0; __asm nop  __endasm

//------------------ COMANDOS LCD -------------------------------
//---------------------------------------------------------------
#define clear         		0x01 // Clear Display
#define home       		0x02 // Cursor a Home 
#define normal	 		0x06 // Normal 
#define rev			0x04 // Normal-reverse
#define scroll 			0x07 // con scroll
#define scroll_rev		0x05 // Reverse 
#define d8_bit			0x38 // 8 bit 2 lineas ( 5x7 font )
#define d4_bit			0x28 // 4 bit 2 lineas ( 5x7 font )
#define reset			0x30 // Reset 
#define dis_on        		0x0C // Display on modo 2 lineas 
#define dis_off       		0x08 // Display off
#define linea1      		0x80 // Linea 1 posicion 1
#define linea2      		0xC0 // Line 2 posicion 1
#define cursor_on         	0x0E // Cursor on
#define cursor_off        	0x0C // Cursor off
#define blink_on   		0x0F // Cursor blink
#define cursor_der		0x14 // Mover cursor derecha
#define cursor_izq 		0x10 // Mover cursor izquierda
#define display__der		0x1C // Scroll display derecha
#define display__izq		0x18 // Scroll display izquierda
#define cg_addr			0x40 //
//----------------------------- Funciones Disponibles --------------------------------
//------------------------------------------------------------------------
void espera(char _ms)
{
  while (--_ms)
   {
  __asm
	nop
  __endasm;
      }
}
void espera_ms(unsigned int ms)
{
  	ms = ms * (FREQ / 4000);  
    while (ms > 0 )
    {
        espera(108);
        ms--;
    }
}

unsigned char lcd_leer()
{
unsigned _data=0;
lcdPort |= 0xf0;         	 //Los ultimos 4bit son Entradas
RW1; 				 //Modo Lectura
EN1;				//   Flaco de Subida Para pin EN
_data=lcdDato & 0xf0;  		//Lectura de Nibble alto
EN0;				//   Flaco de Bajada Para pin EN
EN1;				//   Flaco de Bajada Para pin EN
_data|=((lcdDato &0x0f) >>4);		 //Lectura de Nibble Bajo
EN0;				//   Flaco de Bajada Para pin EN
RW0;
return (_data);
}

void lcd_escribir(unsigned char c)
{
lcdPort &= 0x0f; 		//Los ultimos 4bit son salidas
lcdDato=(lcdDato & 0x0f)|(c<<4);
EN1;
EN0;
}

void lcd_enviardato(unsigned char _ValorDato,unsigned char _TipoDato )
{
RS=0;
while ((lcd_leer())&0x80);		//leer Bit busy, espera a que lcd este libre.
if(_TipoDato>0) 
      { 
       RS=1;
         }
lcd_escribir((_ValorDato&0xf0)>>4);	//Enviar los El nibble mas significativo del Byte C
lcd_escribir((_ValorDato&0x0f)) ;	//Enviar los El nibble menos Singificativo del Byte C
}

void lcd_comando(unsigned char comando )
{
	lcd_enviardato(comando,0);
}

void lcd_borrar()
{
	lcd_comando( clear );
	lcd_comando( home );
}



void lcd_imprimir( unsigned char *car )
{
	char n = 0;
	while( car[n])
	{
		lcd_enviardato( car[n] ,1);
		n++;
	}
}


void lcd_posicion(unsigned char x ,unsigned char y)
{
switch(y)
  {
    case 1:
	       y=0;
      		break;
    case 2:
		y=LCDLinea2;
	 	break;
   case 3:
		y=LCDLinea3;
	 	break;
   case 4:
		y=LCDLinea4;
	 	break;
    default:
   	       y=0;
      		break;
       }
lcd_comando( (0X80|(x+y-1)) );
}

void lcd_iniciar()
{
RS=0;
EN=0;
RW=0;
espera_ms(50); 			// Esperar 50ms retardo durante el encendido.
//Enviar 3 veces 0x03 //
lcd_escribir(3);
espera_ms(5);
lcd_escribir(3);
espera_ms(5);
lcd_escribir(3);
espera_ms(5);
//Enviar 1 vez 0x02 //
lcd_escribir(2);
//Iniciacion 4bit 2 lineas //
lcd_comando( d4_bit );		//0x28 modo 4 bits 2 lineas
lcd_comando( dis_on );		//0x0c display on
lcd_comando( normal );		//0x06 entrada normal hacia adelante
lcd_comando( linea1 );		//0x80 linea 1 posicion 1
}

