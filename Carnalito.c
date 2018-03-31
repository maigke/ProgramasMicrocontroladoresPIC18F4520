// PIC16F876A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <pic16f876a.h>
#define lcd PORTB           //----Assiging PORTD as lcd
#define rs PORTBbits.RB0    //----rs pin for LCD control
#define en PORTBbits.RB2    //----en pin for LCD control

//------Proto-type Decleration------//
void lcd_cmd(unsigned char x); //----function to send command to lcd
void lcd_cmd_hf(unsigned char x); //----function to send 4bit command to lcd
void lcd_dwr(unsigned char x); //----function to send data to lcd
void lcd_msg(unsigned char *c); //----function to send String of data to lcd
void lcd_init(); //----lcd initialization
void lcd_lat(); //----function to latch lcd data

void delay(unsigned int ms); //----Delay
//----------------------------------//

void main() {
    /* << agregue el c?digo >> */
    TRISB = 0x00;

    lcd_init();
    lcd_cmd(0x01);
    lcd_msg("hola hermano");
    while (1);
    while (1) {
        ;
    }
}

void lcd_lat() {
    //---Latching function high to low
    en = 1; //----Enable Pin is high
    delay(1); //----1ms delay
    en = 0; //----Enable Pin is Low
}

void lcd_cmd(unsigned char x) {
    rs = 0; //----Register Selected is Command register
    lcd &= 0x0F; //----Masking Higher 4-bit of PORTD
    lcd |= (x & 0xF0); //----Masking Lower 4-bit of Command
    lcd_lat(); //----Latching it to lcd

    lcd &= 0x0F; //----Masking Higher 4-bit of PORTD
    lcd |= ((x & 0x0F) << 4); //----Masking Higher 4-bit of Command
    lcd_lat(); //----Latching it to lcd
}

void lcd_cmd_hf(unsigned char x) {
    rs = 0; //----Register Selected is Command register
    lcd &= 0x0F; //----Masking Higher 4-bit of PORTD
    lcd |= (x & 0xF0); //----Masking Lower 4-bit of Command
    lcd_lat(); //----Latching it to lcd
}

void lcd_dwr(unsigned char x) {
    rs = 1; //----Register Select Pin is set to Data Register
    lcd &= 0x0F; //----Masking Higher 4-bit of PORTD
    lcd |= ((x & 0xF0)); //----Masking Lower 4-bit of Data
    lcd_lat(); //----Latching it to lcd

    lcd &= 0x0F; //----Masking Higher 4-bit of PORTD
    lcd |= ((x & 0x0F) << 4); //----Masking Higher 4-bit of Data
    lcd_lat(); //----Latching it to lcd
}

void lcd_msg(unsigned char *c) {
    while (*c != 0) {
        lcd_dwr(*c++);
    }
}

void delay(unsigned int ms) {
    int i, j;
    for (i = 0; i <= ms; i++)
        for (j = 0; j <= 120; j++);
}

void lcd_init() {
    lcd_cmd_hf(0x30); //---Sending Sequence Command
    lcd_cmd_hf(0x20); //---Sending Sequence Command
    lcd_cmd(0x28); //---Command to Select 4-bit LCD
    lcd_cmd(0x0E); //---Cusor Blinking
    lcd_cmd(0x01); //---Clear LCD Display
    lcd_cmd(0x06); //---Auto-Increment LCD
    lcd_cmd(0x80); //---Location address
}

