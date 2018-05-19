

#include <stdio.h>
#include <stdlib.h>

#define PIC18F4520
//#define PIC18F4550
//#define PIC18F45K50

#define _XTAL_FREQ 16000000
#include <xc.h>
#include <lcd4a.h>

char value[8];  //guarda espacio para imprimir un numero


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

#define led LATCbits.LATC4

int main()
{
    volatile unsigned char c;
    
    TRISCbits.TRISC4 = 0;
    
    //**********************configuracion del EUSART
    
    //Se utiliza la ecuacion Fosc/[16(n+1)]
    //para calcular el baudaje
    //para una velocidad de 9600 bps
    //SPBRG = Fosc/[16(BaudRate)] - 1
    //SPBRG = 16MHz/[16(9600)] - 1 = 103
    BAUDCONbits.BRG16 = 0; //modo de generacion de 8 bits
    TXSTAbits.SYNC = 0; //modo asincrono
    TXSTAbits.BRGH = 1; //alta velocdad
    SPBRG = 103;
    TXSTAbits.TXEN = 1; //transmision habilitada
    
    RCSTAbits.SPEN = 1; //puerto serie habilitado
    RCSTAbits.CREN = 1; //recepcion de datos habilitada
    TRISCbits.TRISC7 = 1;
    TRISCbits.TRISC6 = 1;
    
    lcd_init();
    lcd_msg("Puerto Serial");
    led=0;
    while(1)
    {
        led^=1;
        lcd_gotoxy(1,2); lcd_msg("hola");
        while(PIR1bits.RCIF == 0);
        c = RCREG;
        led^= 1;
        lcd_gotoxy(1,2); lcd_putc(c);
        __delay_ms(1500);
    }
    
    return ;
}