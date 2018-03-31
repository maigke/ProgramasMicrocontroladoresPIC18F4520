
//#define PIC18F45K50
#define PIC18F4550

#ifdef PIC18F45k50
#include <config3.h>
#include <pic18f45k50.h>
#endif
#ifdef PIC18F4550
#include <config.h>
#include <pic18f4550.h>
#endif


#include "lcd.h"
#include <stdint.h>
#define _XTAL_FREQ 4000000

/**
 *  void lcd_send quartet
 *  Esta función permite el envio del nibble MS a traves de las terminales
 *  definidas como salidas, requiere de los parametros
 * @param mode Se requiere indicar si es un comando o un caracter el que se
 *              envia
 * @param dato Aquí se coloca el dato a enviar a la LCD 1 comando 0 caracter
 */

void lcd_send_quartet(char mode, unsigned char dato)
{
    unsigned char temp=0u;
    if(mode == 1)setbit(LAT(LCD_RS),P_RS);
    else    clrbit(LAT(LCD_RS),P_RS);
   
    if(dato & (1 << 4)) setbit(LAT(LCD_D4),D4);
    else clrbit(LAT(LCD_D4),D4);
    if(dato & (1 << 5)) setbit(LAT(LCD_D5),D5);
    else clrbit(LAT(LCD_D5),D5);
    if(dato & (1 << 6)) setbit(LAT(LCD_D6),D6);
    else clrbit(LAT(LCD_D6),D6);
    if(dato & (1 << 7)) setbit(LAT(LCD_D7),D7);
    else clrbit(LAT(LCD_D7),D7);
    
    lcd_lat();
}


/**
 * unsigned lcd_read_nibble(void)
 *  Esta funcion permite leer un nibble a través de la unión conformada
 *  por la estructura retval_byte
 * @return ret_val_byte
 */
#ifdef USE_RW
unsigned char lcd_read_nibble(void)
{
    union{
        unsigned char retval_byte;
        struct{
            unsigned char ret_val_0:1;
            unsigned char ret_val_1:1;
            unsigned char ret_val_2:1;
            unsigned char ret_val_3:1;
        };
    }retval;

    retval.retval_byte=0;
    E_PIN = 1;
    __delay_ms(1);
    retval.ret_val_0 = READ_PIN_4;
    retval.ret_val_1 = READ_PIN_5;
    retval.ret_val_2 = READ_PIN_6;
    retval.ret_val_3 = READ_PIN_7;
    E_PIN = 0;
    return (retval.retval_byte);
}
#endif  //lcd_read_nibble(void)

/**
 * unsigned char lcd_read_byte(void)
 * Esta funcion se habilita si se ha definido el utilizar el pinRW
 * leyendo dos nibbles y devolviendo la union de ambos nibbles
 * @return high<<4 | low
 */
#ifdef USE_RW
unsigned char lcd_read_byte(void)
{
    unsigned char low;
    unsigned char high;

    TRIS_DATA_PIN_4 = 1;
    TRIS_DATA_PIN_5 = 1;
    TRIS_DATA_PIN_6 = 1;
    TRIS_DATA_PIN_7 = 1;

    RW_PIN = 1;
    __delay_ms(1);
    high = lcd_read_nibble();
    low  = lcd_read_nibble();

    return ((high << 4) | low);
}
#endif //lcd_read_byte

/**
 * void lcd_send(char mode char dato)
 * @param mode parametro para diferenciar entre comando o dato
 * @param dato dato o comando a enviar
 */

void lcd_send(char mode, char dato)
{
    unsigned char temp = dato;
    clrbit(LAT(LCD_RS),P_RS);
    

#ifdef USE_RW
    __delay_ms(10);
    while((lcd_read_byte() & 0x80) == 0x80);
#else
            __delay_ms(10);
#endif
            clrbit(TRIS(LCD_D4),D4);
            clrbit(TRIS(LCD_D5),D5);
            clrbit(TRIS(LCD_D6),D6);
            clrbit(TRIS(LCD_D7),D7);
            
            if(mode == 1)setbit(LAT(LCD_RS),P_RS);
            else    clrbit(LAT(LCD_RS),P_RS);
            __delay_ms(1);
            clrbit(LAT(LCD_E),P_E);
            __delay_ms(1);
#ifdef USE_RW
            RW_PIN = 0;
            __delay_ms(5);
#endif
            lcd_send_quartet(mode, (unsigned char)temp & 0xf0u);
            __delay_ms(1);
            lcd_send_quartet(mode, (unsigned char)(temp << 4u) & 0xf0u);

            lcd_lat();
}
/**
 * lcd_init()  funcion para inicializar la LCD
 */
void lcd_init(void)
{
    unsigned char i;

    clrbit(TRIS(LCD_RS),P_RS);
    clrbit(TRIS(LCD_E),P_E);
    

#ifdef USE_RW
    TRIS_RW = 0;
#endif

    clrbit(LAT(LCD_RS),P_RS);
    clrbit(LAT(LCD_E),P_E);
#ifdef USE_RW
    RW_PIN = 0;
#endif

            clrbit(TRIS(LCD_D4),D4);
            clrbit(TRIS(LCD_D5),D5);
            clrbit(TRIS(LCD_D6),D6);
            clrbit(TRIS(LCD_D7),D7);

    __delay_ms(35);

    lcd_send_quartet(LCD_COMMANDMODE,LCD_RESET);
    __delay_ms(5);
    lcd_send_quartet(LCD_COMMANDMODE,LCD_RESET);
    __delay_ms(5);
    lcd_send_quartet(LCD_COMMANDMODE,LCD_RESET);
    __delay_ms(5);
    lcd_send(LCD_COMMANDMODE,LCD_D4_BIT_CONF);
    __delay_us(500);
    lcd_send(LCD_COMMANDMODE,LCD_D4_BIT);
    __delay_us(25);
    lcd_send(LCD_COMMANDMODE, LCD_CLEAR);
    __delay_ms(2);
    lcd_send(LCD_COMMANDMODE, LCD_CURSOR_OFF);
    __delay_ms(2);
    lcd_send(LCD_COMMANDMODE, LCD_NORMAL);
    __delay_ms(2);
    lcd_send(LCD_COMMANDMODE, LCD_DIS_ON);
    __delay_us(10);
    lcd_send(LCD_COMMANDMODE, 0x80);
}

/**
 * funcion para posicionar el cursor del LCD en un localidad (x,y) particular
 * la funcion esta preparada para trabajar con una LCD de 20 x 4
 * @param x columna del lcd
 * @param y fila del LCD
 */
void lcd_gotoxy(unsigned char x, unsigned char y)
{
    unsigned char ADDRESS= 0x00U;

    switch(y)
    {
        case 1:
            ADDRESS  = LCD_LINE1;
            break;
        case 2:
            ADDRESS = LCD_LINE2;
            break;
        case 3:
            ADDRESS = LCD_LINE3;
            break;
        case 4:
            ADDRESS = LCD_LINE4;
            break;
        default:
            ADDRESS = LCD_LINE1;
            break;

    }
    ADDRESS = (unsigned char)((unsigned char)(ADDRESS + x) - (1U));
    lcd_send(LCD_COMMANDMODE, ADDRESS);
}

/**
 * Funcion que envia un caracter a la lcd o puede enviar caracteres especiales
 * @param c caracter a enviar
 * Si \f se limpia la LCD
 * Si \n es avance de linea
 * Si \b se envia un retroceso
 */
void lcd_putc(unsigned char c)
{
    switch(c)
    {
        case '\f':
            lcd_send(LCD_COMMANDMODE, LCD_CLEAR);
            break;
        case '\n':
            lcd_send(LCD_COMMANDMODE, 2);
            break;
        case '\b':
            lcd_send(LCD_COMMANDMODE, LCD_CURSOR_IZQ);
            break;
        default:
            lcd_send(LCD_CHARMODE,c);
            break;
    }
}

/**
 * Funcion que devuelve un caracter impreso en la lcd en una posicion (x,y)
 * @param x
 * @param y
 * @return value
 */
#ifdef USE_RW
unsigned char lcd_getc(unsigned char x, unsigned char y)
{
    unsigned char value;
    lcd_gotoxy(x,y);
    while((lcd_read_byte() & 0x80)==0x80);
    RS_PIN = 1;
    value = lcd_read_byte();
    RS_PIN = 0;

    return (value);
}
#endif  //unsigned char lcd_getc(unsigned char x, unsigned char y)

/**
 * Funcion que permite personalizar la presntacion del cursor en el LCD
 * @param visible
 * @param blink
 *
 * Si visible  = 1, el cursor estará visible
 * Si blink = 1, el cursor estará parpadeante
 */
void lcd_setcursor_vb(unsigned char visible, unsigned char blink)
{
    lcd_send(LCD_COMMANDMODE, (unsigned char)(LCD_DIS_ON | (visible << (1U)) | blink));
}

/**
 * La funcion puts pone en cola de impresion a lcd una cadena de caracteres
 * @param ptr Puntero de la cadena, el cual va señalando el turno de caracter
 * para ser impreso
 */
void lcd_puts(const char *ptr)
{
    while(*ptr)
    {
        lcd_putc(*ptr++);
    }
}

char lcd_hexa(char a)
{
    if(a > 9)
        a+= 55;
    else
        a+= 48;
    return a;
}

void lcd_lat()
{
    
    setbit(LAT(LCD_E),P_E);
    __delay_ms(1);  
    clrbit(LAT(LCD_E),P_E);
    __delay_ms(1);
}