#include <xc.h>
#include "usart.h"

void sci_conf(void)
{
        //**********************configuracion del EUSART
    
    //Se utiliza la ecuacion Fosc/[16(n+1)]
    //para calcular el baudaje
    //para una velocidad de 9600 bps
    //SPBRG = Fosc/[16(BaudRate)] - 1
    //SPBRG = 16MHz/[16(9600)] - 1 = 103
    BAUDCONbits.BRG16 = 0; //modo de generacion de 8 bits
    
    TXSTAbits.SYNC = 0; //modo asincrono
    TXSTAbits.BRGH = 1; //alta velocdad
    SPBRG = 103;  //valor para transmitir a 9600 bps
    TXSTAbits.TXEN = 1; //transmision habilitada
    
    RCSTAbits.SPEN = 1; //puerto serie habilitado
    RCSTAbits.CREN = 1; //recepcion de datos habilitada
    //configuracion de los pines de comunicacion
    TRISCbits.TRISC7 = 1; //RXD
    TRISCbits.TRISC6 = 0; //TXD
}

unsigned char getch(void)
{
    while(!PIR1bits.RCIF)
        continue;
    return RCREG;
}

void putch(unsigned char dato)
{
    while(PIR1bits.TXIF == 0);
    TXREG = dato;
}

unsigned char getche(void)
{
    unsigned char c;
    putch(c = getch());
    return c;
}
