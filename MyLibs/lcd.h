/*
  File:  fLCDSDCC.h
  Author: Miguel
 * basado en la libreria publicada en el foro de ucontrol por Suky
 * y en la libreria publicada por Felixls en su blog
 * se requiere ademas de la libreria hecha por Junif para los retardos
 *
 * #define FOSC 48000000
 * #include "simple_delay.h"
 * esto en el main

  Created on 28 de agosto de 2012, 02:28 PM
 */

/**
 *  Esta libreria permite conectar cualquier pin del microcontrolador
 *  en interface con una pantalla LCD alfanumerica, dependiendo de las
 *  terminales a utilizar, se requiere que el usuario configure de forma
 *  digital las terminales a utlizar por la libreria
 */
#ifndef fLCDSDCC_h
#define fLCDSDCC_h

#include <xc.h>

//Si no se utliza solo se debe comentar esta linea
//#define USE_RW

/* Definición de las terminales a utilizar
 * LATxbits se usa para escribir al puerto x
 * PORTxbits se usa para leer el estado del puerto
 * TRISxbits se utliza para configurar el puerto
 */
#define avr
#ifdef avr

#define LCD_D4  PORTB
#define D4      4
#define LCD_D5  PORTB
#define D5      5
#define LCD_D6  PORTB
#define D6      6
#define LCD_D7  PORTB
#define D7      7

#define LCD_RS   PORTB
#define P_RS     0
#ifdef  USE_RW  
    #define LCD_RW  PORTB
    #define P_RW    1
#endif
#define LCD_E   PORTB
#define P_E     2

#endif

#ifndef avr 
#define DATA_PIN_7  LATBbits.LATB7
#define DATA_PIN_6  LATBbits.LATB6
#define DATA_PIN_5  LATBbits.LATB5
#define DATA_PIN_4  LATBbits.LATB4

#define READ_PIN_7  PORTBbits.RB7
#define READ_PIN_6  PORTBbits.RB6
#define READ_PIN_5  PORTBbits.RB5
#define READ_PIN_4  PORTBbits.RB4

#define TRIS_DATA_PIN_7    TRISBbits.TRISB7
#define TRIS_DATA_PIN_6    TRISBbits.TRISB6
#define TRIS_DATA_PIN_5    TRISBbits.TRISB5
#define TRIS_DATA_PIN_4    TRISBbits.TRISB4

#define E_PIN  LATBbits.LATB2
#define RS_PIN  LATBbits.LATB0
#ifdef USE_RW
    #define RW_PIN LATBbits.LATB1
#endif

#define TRIS_E  TRISBbits.TRISB2
#define TRIS_RS TRISBbits.TRISB0
#ifdef USE_RW
    #define TRIS_RW TRISBbits.TRISB1
#endif

#endif
/**
 * Definiciones para configurar el LCD
 *
 * Actualizacion: Esta actualizacion es
  19 de mayo de 2011, modifico la libreria para poder usarla con la de hunif
  simple_delay.h
  * Se agregan #define LCD_LINEA 3 y LCD_LINEA_4 para usar una lcd de 4 lineas
  * Se agrega la funcion lcd_gotoxy, adaptada de la version de CCS
  * El encabezado de tu archivo main debe ser el siguiente

  code char at __CONFIG1L CONFIG1L = _PLLDIV_DIVIDE_BY_5__20MHZ_INPUT__1L & _CPUDIV__OSC1_OSC2_SRC___1__96MHZ_PLL_SRC___2__1L & _USBPLL_CLOCK_SRC_FROM_96MHZ_PLL_2_1L;
code char at __CONFIG1H CONFIG1H = _OSC_HS__HS_PLL__USB_HS_1H & _FCMEN_ON_1H & _IESO_ON_1H;
code char at __CONFIG2L CONFIG2L = _PUT_ON_2L & _BODEN_ON_2L & _BODENV_4_2V_2L & _VREGEN_ON_2L;
code char at __CONFIG2H CONFIG2H = _WDT_DISABLED_CONTROLLED_2H & _WDTPS_1_32768_2H;
code char at __CONFIG3H CONFIG3H = _CCP2MUX_RC1_3H & _PBADEN_PORTB_4_0__CONFIGURED_AS_DIGITAL_I_O_ON_RESET_3H & _LPT1OSC_ON_3H & _MCLRE_MCLR_OFF_RE3_ON_3H;
code char at __CONFIG4L CONFIG4L = _STVR_ON_4L & _LVP_OFF_4L & _ENHCPU_OFF_4L & _BACKBUG_OFF_4L;
code char at __CONFIG5L CONFIG5L = _CP_0_OFF_5L & _CP_1_OFF_5L & _CP_2_OFF_5L & _CP_3_OFF_5L;
code char at __CONFIG5H CONFIG5H = _CPB_OFF_5H;
code char at __CONFIG6L CONFIG6L = _WRT_0_OFF_6L & _WRT_1_OFF_6L & _WRT_2_OFF_6L & _WRT_3_OFF_6L;
code char at __CONFIG6H CONFIG6H = _WRTC_OFF_6H & _WRTB_OFF_6H;
code char at __CONFIG7L CONFIG7L = _EBTR_0_OFF_7L & _EBTR_1_OFF_7L & _EBTR_2_OFF_7L & _EBTR_3_OFF_7L;
code char at __CONFIG7H CONFIG7H = _EBTRB_OFF_7H;


  --Autor: Miguel Montiel Martinez
 * Las lineas se agregan el 28 de agosto de 2012
 */

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
#define LCD_LINE1                0x00 // Linea 1 posicion 1 1000 0000
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


/** Seccion de funciones
 *  En esta seccion se encuentran las funciones necesarias para implementar
 *  la interfaz entre una pantalla LCD y un microcontrolador PIC18
 */

//Cadena para enviar la inicializacion de la LCD
const char LCD_INIT_STRING[]=
{
    LCD_D4_BIT, LCD_CLEAR, LCD_CURSOR_OFF, LCD_NORMAL,
    LCD_DIS_ON
};

/**
 *  void lcd_send quartet
 *  Esta función permite el envio de un nibble a través de las terminales
 *  definidas como salidas, requiere de los parametros
 * @param mode Se requiere indicar si es un comando o un caracter el que se
 *              envia
 * @param dato Aquí se coloca el dato a enviar a la LCD
 */

void lcd_send_quartet(char mode, unsigned char dato);

/**
 * unsigned lcd_read_nibble(void)
 *  Esta funcion permite leer un nibble a través de la unión conformada
 *  por la estructura retval_byte
 * @return ret_val_byte
 */
unsigned char lcd_read_nibble(void);

/**
 * unsigned char lcd_read_byte(void)
 * Esta funcion se habilita si se ha definido el utilizar el pinRW
 * leyendo dos nibbles y devolviendo la union de ambos nibbles
 * @return high<<4 | low
 */
unsigned char lcd_read_byte(void);

/**
 * void lcd_send(char mode char dato)
 * @param mode parametro para diferenciar entre comando o dato
 * @param dato dato o comando a enviar
 */

void lcd_send(char mode, char dato);

/**
 * lcd_init()  funcion para inicializar la LCD
 */
void lcd_init(void);

/**
 * funcion para posicionar el cursor del LCD en un localidad (x,y) particular
 * la funcion esta preparada para trabajar con una LCD de 20 x 4
 * @param x columna del lcd
 * @param y fila del LCD
 */
void lcd_gotoxy(unsigned char x, unsigned char y);

/**
 * Funcion que envia un caracter a la lcd o puede enviar caracteres especiales
 * @param c caracter a enviar
 * Si \f se limpia la LCD
 * Si \n es avance de linea
 * Si \b se envia un retroceso
 */
void lcd_putc(unsigned char c);

/**
 * Funcion que devuelve un caracter impreso en la lcd en una posicion (x,y)
 * @param x
 * @param y
 * @return value
 */
unsigned char lcd_getc(unsigned char x, unsigned char y);

/**
 * Funcion que permite personalizar la presntacion del cursor en el LCD
 * @param visible
 * @param blink
 *
 * Si visible  = 1, el cursor estará visible
 * Si blink = 1, el cursor estará parpadeante
 */
void lcd_setcursor_vb(unsigned char visible, unsigned char blink);

/**
 * La funcion puts pone en cola de impresion a lcd una cadena de caracteres
 * @param ptr Puntero de la cadena, el cual va señalando el turno de caracter
 * para ser impreso
 */
void lcd_puts(const char *ptr);

char lcd_hexa(char a);
void lcd_lat(void);
#endif // fLCDSDCC_h
