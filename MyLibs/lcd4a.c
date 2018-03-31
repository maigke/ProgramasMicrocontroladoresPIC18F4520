//#define PIC18F45K50
//#define PIC18F4550
#define PIC18F4520

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


#include "lcd4a.h"
#include <stdint.h>
#define _XTAL_FREQ 4000000

unsigned char lcd;


void lcd_lat()
{
    //---Latching function high to low
    
    setbit(LAT(lcd_en),p_en);
    //en = 1;    //----Enable Pin is high
    __delay_ms(2);  //----1ms delay
    clrbit(LAT(lcd_en),p_en);
    //en = 0;    //----Enable Pin is Low
}
 
void lcd_cmd(unsigned char x)
{
    lcd = 0;
    clrbit(LAT(lcd_rs),p_rs);            //----Register Selected is Command register
    lcd &= 0x0F;       //----Masking Higher 4-bit of PORTD
    lcd |= (x & 0xF0); //----Masking Lower 4-bit of Command
    manda(lcd);
    lcd_lat();         //----Latching it to lcd
    __delay_ms(3);
 
    lcd &= 0x0F;       //----Masking Higher 4-bit of PORTD
    lcd |= ((x & 0x0F)<<4); //----Masking Higher 4-bit of Command
    manda(lcd);
    lcd_lat();         //----Latching it to lcd
}
 
void lcd_cmd_hf(unsigned char x)
{
    clrbit(LAT(lcd_rs),p_rs);            //----Register Selected is Command register
    lcd &= 0x0F;       //----Masking Higher 4-bit of PORTD
    lcd |= (x & 0xF0); //----Masking Lower 4-bit of Command
    manda(lcd);
    lcd_lat();         //----Latching it to lcd
}
 

void lcd_putc(unsigned char c)
{
    switch(c)
    {
        case '\f':
            lcd_cmd(LCD_CLEAR);
            break;
        case '\n':
            lcd_cmd(2);
            break;
        case '\b':
            lcd_cmd(LCD_CURSOR_IZQ);
            break;
        default:
            lcd_dwr(c);
            break;
    }
}


void lcd_dwr(unsigned char x)
{
    setbit(LAT(lcd_rs),p_rs);              //----Register Select Pin is set to Data Register
    lcd &= 0x0F;         //----Masking Higher 4-bit of PORTD
    lcd |= ((x & 0xF0)); //----Masking Lower 4-bit of Data
    manda(lcd);
    lcd_lat();           //----Latching it to lcd
    __delay_ms(3);
 
    lcd &= 0x0F;         //----Masking Higher 4-bit of PORTD
    lcd |= ((x & 0x0F)<<4); //----Masking Higher 4-bit of Data
    manda(lcd);
    lcd_lat();           //----Latching it to lcd
}
 
void lcd_msg(unsigned char *c)
{
    while(*c != 0)
    {
        lcd_putc(*c++);
    }
}

/*
 * Refierase a la pagina 33 punto 3.1.8
 * Manual LCD Total
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
        default:
            ADDRESS = LCD_LINE1;
            break;

    }
    ADDRESS = (unsigned char)((unsigned char)(ADDRESS + x) - (1U));
    lcd_cmd(ADDRESS);
}

void lcd_init()
{
    /*
     * Dependiendo del microcontrolador
     * se debe configurar primero los puertos
     * en caso de ser analógicos
     * y posteriormente ejecutar esta rutina
     */
    clrbit(TRIS(LCD_D7),D7);
    clrbit(TRIS(LCD_D6),D6);
    clrbit(TRIS(LCD_D5),D5);
    clrbit(TRIS(LCD_D4),D4);
    clrbit(TRIS(lcd_rs),p_rs);
#ifdef USE_RW
    clrbit(TRIS(lcd_rw),p_rw);
#endif
    clrbit(TRIS(lcd_en),p_en);
    
    lcd_cmd_hf(LCD_RESET);   //---Sending Sequence Command
    __delay_ms(15);
    lcd_cmd_hf(LCD_RESET);
    __delay_ms(1);
    lcd_cmd_hf(LCD_RESET);
    __delay_ms(15);
    lcd_cmd_hf(LCD_D4_BIT_CONF);   //---Sending Sequence Command
    __delay_ms(5);
    lcd_cmd(LCD_D4_BIT);      //---Command to Select 4-bit LCD
    __delay_ms(5);
    lcd_cmd(LCD_CURSOR_ON);      //---Cusor Blinking
    __delay_ms(5);
    lcd_cmd(LCD_CLEAR);      //---Clear LCD Display
    __delay_ms(5);
    lcd_cmd(LCD_NORMAL);      //---Auto-Increment LCD
    __delay_ms(5);
    lcd_cmd(LCD_LINE1);      //---Location address
}


void manda(unsigned char dato)
{
    if(dato & (1 << 4)) setbit(LAT(LCD_D4),D4);
    else clrbit(LAT(LCD_D4),D4);
    if(dato & (1 << 5)) setbit(LAT(LCD_D5),D5);
    else clrbit(LAT(LCD_D5),D5);
    if(dato & (1 << 6)) setbit(LAT(LCD_D6),D6);
    else clrbit(LAT(LCD_D6),D6);
    if(dato & (1 << 7)) setbit(LAT(LCD_D7),D7);
    else clrbit(LAT(LCD_D7),D7);
}

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
    lcd_cmd((unsigned char)(LCD_DIS_ON | (visible << (1U)) | blink));
}

char lcd_hexa(char a)
{
    if(a > 9)
        a+= 55;
    else
        a+= 48;
    return a;
}