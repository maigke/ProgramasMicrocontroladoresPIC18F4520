/*****************************************************************************
******************************************************************************
****                                                                      ****
****             --- DRIVER LDC 44780 4-BITS para PIC16F ---              ****
****                                                                      ****
******************************************************************************
******************************************************************************

                            FUNCIONES DISPONIBLES: 
------------------------------------------------------------------------------
lcd_car();			//envia un caracter, ejemplo: lcd_car("A");
lcd_dato();		//envía byte, ejemplo: char a= 0xE5; lcd_dato(a); lcd_dato(0xE5)
lcd_comand();		//envía comando, ejemplo: lcd_comand(linea2); lcd_comand(0x0E)
lcd_num();			//envía número entero, ejemplo: lcd_num(1756);
lcd_clear();		//borra display.
lcd_init();		//inicializa lcd.
espeara_ms();		//delay milisegundos, ejemplo: espera_ms(10);
pausa();			//delay 4xValor+1 µs, ejemplo: pausa(2); pausa de 9 µs.

********************************************************************************
********************************************************************************

        EN EL PROGRAMA PRINCIPAL DONDE VAYA A USAR LAS FUNCIONES LCD
INSERTAR LAS SIGUIENTES LINEAS CON LAS DEFINICIONES DE PUERTO Y RELOJ USADOS:

//---------------------------- PUERTO LCD --------------------------------
//------------------------------------------------------------------------

#define RS RC1			//PIN PARA RS.
#define RW RC2			//PIN PARA RW.
#define EN RC3			//PIN PARA ENABLE.
#define DATOT TRISC		//TRIS DE DATOS, PINES 4, 5, 6, 7 DEL PUERTO.
#define DATOP PORTC		//PORT DE DATOS, MISMO PUERTO.

#define FREQ 8000		// 8000 KHz = 8 Mhz, LA FRECUENCIA EN KHz.

#include "lcd_sdcc_sgr.h"


********************************************************************************
*******************************************************************************/


//----------------------------- ACCIONES ---------------------------------
//------------------------------------------------------------------------
#define write  RW = 0; pausa(u9); DATOT &= 0x0F; pausa(u9) 
#define read   DATOT |= 0xF0; pausa(u9); RW = 1; pausa(u9)
#define EN0 EN = 0; pausa(u9)
#define EN1 EN = 1; pausa(u9)

//--------------------------- COMANDOS LCD -------------------------------
//------------------------------------------------------------------------
#define clear         		0x01 // Clear Display
#define home       		0x02 // Cursor a Home 
#define normal			0x06 // Normal 
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
//----------------------- CONFIGURAR VARIABLES ---------------------------
//------------------------------------------------------------------------
unsigned char d;
unsigned char n;
unsigned char coms = FREQ / 4000;
unsigned char u9 = FREQ / 2000;
unsigned char pa = FREQ / 400;

//----------------------------- FUNCIONES --------------------------------
//------------------------------------------------------------------------

void pausa( unsigned char a )			
{ 	a;				//tiempo = (4a + 1)µs a>=2, para 4MHz
	__asm__ ("\tsublw 1\n\
	bucle:\n\t addlw 1 \n\t btfss STATUS,0 \n\t goto bucle");
	
}

void espera_ms( unsigned int ms ) 
{
	ms = ms * coms;
	while (ms > 0 )
	{ 
		pausa(247); 
		ms--;
	}
}

char lcd_read()
{
	read;				//modo lectura
	
	EN1;
	espera_ms(1);
	d = DATOP & 0xF0;		//lee nibble alto
	espera_ms(1);
	EN0;
	
	EN1;
	espera_ms(1);
	d |= DATOP >> 4;		//lee nibble bajo
	espera_ms(1);
	EN0;
	
	return d;
}
	
char lcd_busy()
{
	RS = 0;
	espera_ms(10);
	espera_ms(10);
	lcd_read();
	
	d = d & 0x80;			//aisla bit busy 
	
	if( d == 0x80 ) 		//comprueba si está ocupado
		return(1);
	else	
		return(0);
}	

void write_n( char dat )
{
	write;				//modo escritura
	
	DATOP &= 0x0F;
	DATOP |= dat & 0xF0;		//escribe nibble alto
	EN1;
	espera_ms(1);
	EN0;
	espera_ms(1);
	dat <<= 4;
	
	DATOP &= 0x0F;
	DATOP |= dat & 0xF0;		//escribe nibble bajo
	EN1;
	espera_ms(1);
	EN0;
	espera_ms(1);
	pausa(pa);
}

void lcd_dato( char dato )
{
	espera_ms(10);
	while( lcd_busy() == 1 )	//leer bit busy hasta que sea 0
	{
	  espera_ms(2);
	}
	RS = 1;	
	pausa(pa);//modo datos
	write_n( dato );
}

void lcd_comand( char comand )
{
	RS = 0;				//modo comandos
	write_n( comand );
}

void lcd_clear()
{
	lcd_comand( clear );
	lcd_comand( home );
}

void lcd_init( void )
{
	DATOT = 0;			//todo a salidas
	DATOP = 0;			//todo a 0
	espera_ms(20);
	write_n( reset );		//secuancia de reset 
	espera_ms(10);
	write_n( reset );
	pausa(pa);
	write_n( reset );
	pausa(u9);			//secuencia de inicio:
	lcd_comand( d4_bit );		//0x28 modo 4 bits 2 lineas
	lcd_comand( normal );		//0x06 entrada normal hacia adelante
	lcd_comand( dis_on );		//0x0c display on
	lcd_comand( linea1 );		//0x80 linea 1 posicion 1
}

void lcdh( char a )
{
	if (a > 9)
		a += 55;
	else
		a += 48;
	lcd_dato(a);
}


void lcd_hex( char nb )
{
	char n;
	n = ( nb >> 4 ) & 0x0f;
	lcdh(n);
	n = nb & 0x0f;
	lcdh(n);
}

void lcd_num( int num )
{
	lcd_hex( num >> 8 );
	lcd_hex( num & 0xff );
}

void lcd_car( unsigned char *car )
{
	char n = 0;
	while( car[n])
	{
		lcd_dato( car[n] );
		n++;
	}
}
