//lcd-sdcc.h


//--------------------------- DEFINICIONES -------------------------------
//------------------------------------------------------------------------
#define write  RW = 0; pausa(); DATOT &= 0x0F; pausa()
#define read   DATOT |= 0xF0; pausa(); RW = 1; pausa()
#define EN0 EN = 0; pausa()
#define EN1 EN = 1; pausa()

#define clear         		0x01 // Clear Display
#define home       		0x02 // Cursor a Home
#define normal			0x06 // Normal

#define d4_bit			0x28 // 4 bit 2 lineas ( 5x7 font )
#define reset			0x30 // Reset
#define dis_on        		0x0C // Display on modo 2 lineas
#define dis_off       		0x08 // Display off
#define linea1      		0x80 // Linea 1 posicion 1
#define linea2      		0xC0 // Line 2 posicion 1

#define FREQ 8000		// 8 Mhz

//----------------------- CONFIGURAR VARIABLES ---------------------------
//------------------------------------------------------------------------
unsigned char d;
unsigned int loop;
unsigned int coms;
unsigned int co;
//------------------------------------------------------------------------
//------------------------------------------------------------------------

void tiempo()
{
	coms = FREQ / 4000;
	coms = coms * 1000 / 7;
}

void pausa( void )
{
	__asm
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop	
	__endasm;
}

void delay_1ms( unsigned int co )
{
	while (co > 0 ){ co--; }
}

void delay_ms( unsigned int ms )
{
	while (ms > 0 )
	{
		co = coms;
		delay_1ms(co);
		ms--;
	}
}

char lcd_read()
{
  delay_ms(20);
	read;
	pausa();
	EN = 1;
	pausa();
	d = DATOP & 0xF0;
	EN = 0;
	pausa();
	EN = 1;
	pausa();
	d |= DATOP >> 4;
	EN = 0;
	pausa();
	return d;
}
	
char lcd_busy( void )
{
	pausa();
	RS = 0;
	lcd_read();
	d = d & 0x80;
	if(d == 0x80){
		return(1);
	}
	else
	{
		return(0);	
	}
}	

void write_n( char dat )
{
	write;
	pausa();	
	DATOP &= 0x0F;
	DATOP |= dat & 0xF0;
	EN = 1;
	pausa();
	EN = 0;
	pausa();
	pausa();
	
	dat <<= 4;
	
	DATOP &= 0x0F;
	DATOP |= dat & 0xF0;
	EN = 1;
	pausa();
	EN = 0;
	pausa();
	pausa();
}

void send_dato( char dato )
{
	while( lcd_busy() == 1 );
	RS = 1;
	write_n( dato );
}

void send_comand( char comand )
{
	loop = 0;
	while(loop < 100){ loop++; }              //AQUI NO ME FUNCIONA delay_ms()... no se porqué.
	RS = 0;
	write_n( comand );
}

void lcd_clear()
{
	send_comand( clear );
	send_comand( home);
}

void lcd_init( void )
{
	tiempo();
	DATOT = 0;
	DATOP = 0;
	delay_ms(20);
	write_n( reset );
	delay_ms(5);
	write_n( reset );
	delay_ms(1);
	write_n( reset );
	pausa();
	send_comand( d4_bit );
	delay_ms(10);
	send_comand( normal );
	delay_ms(10);
	send_comand( dis_on );
	delay_ms(10);
	send_comand( linea1 );
	delay_ms(10);
}

void lprintf( const char *frase )
{
	while ( *frase )
	{
		send_dato( *frase++ );
	}
}