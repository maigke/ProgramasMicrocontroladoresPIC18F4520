

#ifndef lcd4a_h
#define lcd4a_h

/*
 * Macros para definir la configuracion
 * de los pines a utilizar
 */
//Comentar si se usa el rw
//#define USE_RW

#define LCD_D7 PORTD
#define D7 7
#define LCD_D6 PORTD
#define D6 6
#define LCD_D5 PORTD
#define D5 5
#define LCD_D4 PORTD
#define D4 4

#define lcd_rs PORTD
#define p_rs 2
#ifdef USE_RW
#define lcd_rw PORTD
#define p_rw 6
#endif
#define lcd_en PORTD
#define p_en 3

#define LCD_CLEAR                0x01 // Clear Display
#define LCD_HOME                0x02 // Cursor a Home
#define LCD_NORMAL              0x06 // Cursor en modo incrementar.
#define LCD_REV                  0x04 // Normal-reverse
#define LCD_SCROLL              0x07 // Usa scroll
#define LCD_SCROLL_REV          0x05 // Reverse
#define LCD_D8_BIT              0x38 // 8 bit 2 lineas ( 5x7 font )
#define LCD_D4_BIT_CONF          0x20 // 4 bit
#define LCD_D4_BIT              0x28 // 4 bit 2 lineas ( 5x7 font )
#define LCD_RESET                0x30 // Reset
#define LCD_DIS_ON              0x0C // Display on modo 2 lineas
#define LCD_DIS_OFF              0x08 // Display off
#define LCD_LINE1                0x80 // Linea 1 posicion 1 1000 0000
#define LCD_LINE2                0xC0 // Linea 2 posicion 1 1010 0000
#define LCD_LINE3        0x14 //Linea 3 posicion  1
#define LCD_LINE4        0x54 //Linea 4 posicion  1
#define LCD_CURSOR_ON            0x0E // Cursor on
#define LCD_CURSOR_OFF          0x0C // Cursor off
#define LCD_BLINK_ON            0x0F // Cursor blink
#define LCD_CURSOR_DER          0x14 // Mover cursor derecha
#define LCD_CURSOR_IZQ          0x10 // Mover cursor izquierda
#define LCD_DISPLAY__DER        0x1C // Scroll display derecha
#define LCD_DISPLAY__IZQ        0x18 // Scroll display izquierda
#define LCD_CHARMODE            0x01 //envia caracter
#define LCD_COMMANDMODE          0x00 //envia comando
#define LCD_CGRAM1        0x40 //primera direccion para CGRAM
#define LCD_CGRAM2        0x48 //segunda direccion para CGRAM

//------Proto-type Decleration------//
void lcd_cmd(unsigned char x);      //----function to send command to lcd
void lcd_cmd_hf(unsigned char x);   //----function to send 4bit command to lcd
void lcd_putc(unsigned char c);     //----funcion para enviar caracter
void lcd_dwr(unsigned char x);      //----function to send data to lcd
void lcd_msg(unsigned char *c);     //----function to send String of data to lcd
void lcd_init();                    //----lcd initialization
void lcd_lat();                     //----function to latch lcd data
void lcd_gotoxy(unsigned char x, unsigned char y);
void manda(unsigned char dato);     //envia los datos a los pines configurados
void lcd_setcursor_vb(unsigned char visible, unsigned char blink);
char lcd_hexa(unsigned char a);
#endif
