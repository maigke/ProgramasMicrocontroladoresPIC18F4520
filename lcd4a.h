

#ifndef lcd4a_h
#define lcd4a_h

#define lcd PORTB           //----Assiging PORTD as lcd
#define rs PORTBbits.RB0    //----rs pin for LCD control
#define en PORTBbits.RB2    //----en pin for LCD control

//------Proto-type Decleration------//
void lcd_cmd(unsigned char x);      //----function to send command to lcd
void lcd_cmd_hf(unsigned char x);   //----function to send 4bit command to lcd
void lcd_dwr(unsigned char x);      //----function to send data to lcd
void lcd_msg(unsigned char *c);     //----function to send String of data to lcd
void lcd_init();                    //----lcd initialization
void lcd_lat();                     //----function to latch lcd data

void delay(unsigned int ms);        //----Delay
#endif
