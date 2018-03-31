#define PIC18F4520
//#define PIC18F4550
//#define PIC18F45K50

#define _XTAL_FREQ 4000000
#include <xc.h>

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

#define PledVerde  PORTC
#define ledVerde   2
#define PledRojo  PORTC
#define ledRojo   3
#define PBoton      PORTC
#define Boton       1


#define ESTADO_ON   0
#define ESTADO_OFF  1



unsigned char estado=0;
unsigned char T0ini;
unsigned char Ton = 0;
volatile unsigned char cont = 0;
#include <xc.h>


//Interrupciones

void low_priority interrupt low_isr()
{
  

}

void interrupt high_priority high_isr()
{
    if(INTCONbits.T0IF == 1)
    {
        
        if(estado == ESTADO_ON)
        {
            setbit(LAT(PledVerde),ledVerde);
            T0ini = 255-Ton;
            estado = ESTADO_OFF;
        }
        else
        {
            clrbit(LAT(PledVerde),ledVerde);
            T0ini = Ton;
            estado = ESTADO_ON;
            cont++;
        }
        TMR0=T0ini;
        TMR0IF = 0;
        return;
    }


}

void main(void) {
    
    clrbit(TRIS(PledVerde),ledVerde);
    clrbit(TRIS(PledRojo),ledRojo);
    setbit(TRIS(PBoton),Boton);
    /*********************************
     Sección para configurar la interrupcion
     */
    //INTCON = 0b00000000; //valores del INTCON modificar
    //ADCON0bits.ADON = 0;
    //configuracion del Timer 0
    TMR0IE = 1; //habilita interrupciones del TMR0
    T0CS = 0;
    PSA = 0;
    TMR0ON = 0;
    T08BIT = 1; //timer a 8 bits
    T0PS2 = 0; //prescaler a 256
    T0PS1 = 0;
    T0PS0 = 1;
    
    
    
    TMR0IP = 1; //activo en bajo para baja prioridad
    
    
    RCONbits.IPEN = 1;
    GIEH = 1; //habilita las interrupciones de alta prioridad
    GIEL = 0; //deshabilita las interrupciones de baja prioridad
    /*********************************/
    
    clrbit(LAT(PledVerde),ledVerde);
    clrbit(LAT(PledRojo),ledRojo);
    for( int i = 0 ;i < 10; i++)
            __delay_ms(100);
    
    TMR0ON = 1;
    while(1)
    {
        for(int i=255; i>0;i--)
        {
            Ton = i;
            cont = 0;
            while( cont < 10);
        }
        if(!(PBoton & (1<<Boton))){
            setbit(LAT(PledRojo),ledRojo);
        }
        else
        {clrbit(LAT(PledRojo),ledRojo);}

        
    }
    return;
}
