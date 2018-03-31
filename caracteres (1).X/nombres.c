/*
 * File:   xtr.c
 * Author: Prof. Aarón r
 * Autor2: M.S.C. Miguel Montiel Martinez
 * Created on 13 de diciembre de 2017, 10:40 PM
 * 
 */


#include <xc.h>
#include<pic18f4550.h>
#define _XTAL_FREQ 4000000
#define lcd PORTD           //----Assiging PORTD as lcd
#define rs PORTDbits.RD0    //----rs pin for LCD control
#define en PORTDbits.RD1    //----en pin for LCD control
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
            Display(8); //prueba de leds
    while(1)
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
       for (i= 0 ; i<8 ; i++)
            {   valor = Digit[N][i] ;
                LATBbits.LATB0=Digit[N][0];
                LATBbits.LATB1=Digit[N][1];
                LATBbits.LATB2=Digit[N][2];
                LATBbits.LATB3=Digit[N][3];
                LATBbits.LATB4=Digit[N][4];
                LATBbits.LATB5=Digit[N][5];
                LATBbits.LATB6=Digit[N][6];
            }
   }


void lcd_lat()
{
    //---Latching function high to low
    en = 1;    //----Enable Pin is high
    delay(1);  //----1ms delay
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

