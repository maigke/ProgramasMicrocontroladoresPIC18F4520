/*
 * File:   xtr.c
 * Author: Prof. Aarón r
 * Autor2: M.S.C. Miguel Montiel Martinez
 * Created on 13 de diciembre de 2017, 10:40 PM
 * 
 */
/*
// PIC18LF452 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1H
#pragma config OSC = HS         // Oscillator Selection bits (HS oscillator)
#pragma config OSCS = OFF       // Oscillator System Clock Switch Enable bit (Oscillator system clock switch option is disabled (main oscillator is source))

// CONFIG2L
#pragma config PWRT = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOR = ON         // Brown-out Reset Enable bit (Brown-out Reset enabled)
#pragma config BORV = 42        // Brown-out Reset Voltage bits (VBOR set to 4.2V)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 128      // Watchdog Timer Postscale Select bits (1:128)

// CONFIG3H
#pragma config CCP2MUX = OFF    // CCP2 Mux bit (CCP2 input/output is multiplexed with RB3)

// CONFIG4L
#pragma config STVR = ON        // Stack Full/Underflow Reset Enable bit (Stack Full/Underflow will cause RESET)
#pragma config LVP = ON         // Low Voltage ICSP Enable bit (Low Voltage ICSP enabled)

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000200-001FFFh) not code protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot Block (000000-0001FFh) not code protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000200-001FFFh) not write protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0001FFh) not write protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000200-001FFFh) not protected from Table Reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from Table Reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from Table Reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from Table Reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0001FFh) not protected from Table Reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
*/

#include <xc.h>
//#include<pic18f452.h>
#define _XTAL_FREQ 4000000
#include <pic18f4550.h>
#include <config.h>
#define lcd PORTD           //----Assiging PORTD as lcd
#define rs PORTDbits.RD0    //----rs pin for LCD control
#define en PORTDbits.RD2    //----en pin for LCD control
#define disp PORTB

/*
 * Nombres que se van a mostrar en la LCD
 */
unsigned char p1[]= "JUAN CARLOS";
unsigned char p2[]= "NORMA ISABEL";
unsigned char p3[]= "OSCAR EDUARDO";
unsigned char p4[]= "INTRODUCCION A";

unsigned char b1[]= "RODRIGUEZ PEREZ";
unsigned char b2[]= "HERNANDEZ DIAZ";
unsigned char b3[]= "LUNA MENDOZA";
unsigned char b4[]= "LA PROGRAMACION";

/*Arreglos para colocar los nombres*/
unsigned char *p[]={p1,p2,p3,p4};
unsigned char *b[]={b1,b2,b3,b4};

/*
 *  Tabla de decodificacion para 
 *  display de siete segmentos
 */
char  Digit[10][8] =                
{ 
//   a,b,c,d,e,f,g,dp    
   { 1,1,1,1,1,1,0,0 },    // 0   
   { 0,1,1,0,0,0,0,0 },    // 1
   { 1,1,0,1,1,0,1,0 },    // 2
   { 1,1,1,1,0,0,1,0 },    // 3
   { 0,1,1,0,0,1,1,0 },    // 4
   { 1,0,1,1,0,1,1,0 },    // 5
   { 1,0,1,1,1,1,1,0 },    // 6
   { 1,1,1,0,0,0,0,0 },    // 7
   { 1,1,1,1,1,1,1,0 },    // 8
   { 1,1,1,0,0,1,1,0 }     // 9
};


/*Funciones para incrementos*/
char IncrementaPersona(void);
char IncrementaNumero(void);
void Display(int N);
/*Variables para controlar los conteos*/
volatile int cont = 0;
volatile int contN = 0;

//------Proto-type Decleration------//
void lcd_cmd(unsigned char x);      //----function to send command to lcd
void lcd_cmd_hf(unsigned char x);   //----function to send 4bit command to lcd
void lcd_dwr(unsigned char x);      //----function to send data to lcd
void lcd_msg(unsigned char *c);     //----function to send String of data to lcd
void lcd_init();                    //----lcd initialization
void lcd_lat();                     //----function to latch lcd data
 
void delay(unsigned int ms);        //----Delay
//----------------------------------//



void main()
{
    char boton= 'x';
    contN=0;
    TRISD = 0x00;                //---PORTD es Salida
    TRISB = 0x00;                //---PORTD es Salida
    TRISCbits.TRISC6=1;  //boton para incrementar personas
    TRISCbits.TRISC7=1;  //boton para incrementar numeros
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;
    lcd_init();                 //---LCD Initialization
    __delay_ms(200);
            lcd_cmd(0x01);      //---Clear LCD Display
          	lcd_msg(p[3]);
            lcd_cmd(0xC0);              //---Command For Next Line in LCD
            lcd_msg(b[3]);  
            Display(0); //prueba de leds
    while(boton == 'x')
    {
        __delay_ms(20);
        if(PORTCbits.RC6 == 1)
        {
            PORTCbits.RC0 = 1;
            PORTCbits.RC1 = 0;
            boton= IncrementaPersona();
        }
        else if(PORTCbits.RC7 == 1)
        {
            PORTCbits.RC0 = 0;
            PORTCbits.RC1 = 1;
            boton = IncrementaNumero();
        }
        
        PORTCbits.RC0 ^= 1;
        PORTCbits.RC1 ^= 1;
        __delay_ms(500);
    }
    
}

/*
 * *************************************
 * Funciones para controlar los conteos  
 */
char IncrementaPersona()
{

         {
            lcd_cmd(0x01);      //---Clear LCD Display
          	lcd_msg(p[cont]);
            lcd_cmd(0xC0);              //---Command For Next Line in LCD
            lcd_msg(b[cont]);
         }
cont++;
if (cont == 4) cont = 0;
return 'x';
}


char IncrementaNumero()
{
    Display(contN);
    contN++;
    if(contN == 10) contN = 0;
    return 'x';
}

void Display(int N)
{   
    int valor,i;
       for (i= 0; i<8; i++)
            {   
             valor = Digit[N][i];
             if(valor == 1)
             { LATB |= (1<<i);} //pone a 1 un bit
             else
             {LATB &= ~(1<<i);} //pone a 0 un bit
            }
}


void lcd_lat()
{
    //---Latching function high to low
    en = 1;    //----Enable Pin is high
    __delay_ms(1);  //----1ms delay
    en = 0;    //----Enable Pin is Low
}
 
void lcd_cmd(unsigned char x)
{
    rs = 0;            //----Register Selected is Command register
    lcd &= 0x0F;       //----Masking Higher 4-bit of PORTD
    lcd |= (x & 0xF0); //----Masking Lower 4-bit of Command
    lcd_lat();         //----Latching it to lcd
 
    lcd &= 0x0F;       //----Masking Higher 4-bit of PORTD
    lcd |= ((x & 0x0F)<<4); //----Masking Higher 4-bit of Command
    lcd_lat();         //----Latching it to lcd
}
 
void lcd_cmd_hf(unsigned char x)
{
    rs = 0;            //----Register Selected is Command register
    lcd &= 0x0F;       //----Masking Higher 4-bit of PORTD
    lcd |= (x & 0xF0); //----Masking Lower 4-bit of Command
    lcd_lat();         //----Latching it to lcd
}
 
void lcd_dwr(unsigned char x)
{
    rs = 1;              //----Register Select Pin is set to Data Register
    lcd &= 0x0F;         //----Masking Higher 4-bit of PORTD
    lcd |= ((x & 0xF0)); //----Masking Lower 4-bit of Data
    lcd_lat();           //----Latching it to lcd
 
    lcd &= 0x0F;         //----Masking Higher 4-bit of PORTD
    lcd |= ((x & 0x0F)<<4); //----Masking Higher 4-bit of Data
    lcd_lat();           //----Latching it to lcd
}
 
void lcd_msg(unsigned char *c)
{
    while(*c != 0)
    {
        lcd_dwr(*c++);
    }
}
 
void delay(unsigned int ms)
{
    int i,j;
    for(i=0;i<=ms;i++)
        for(j=0;j<=120;j++);
}
 
void lcd_init()
{
    lcd_cmd_hf(0x30);   //---Sending Sequence Command
    lcd_cmd_hf(0x20);   //---Sending Sequence Command
    lcd_cmd(0x28);      //---Command to Select 4-bit LCD
    lcd_cmd(0x0E);      //---Cusor Blinking
    lcd_cmd(0x01);      //---Clear LCD Display
    lcd_cmd(0x06);      //---Auto-Increment LCD
    lcd_cmd(0x80);      //---Location address
}

