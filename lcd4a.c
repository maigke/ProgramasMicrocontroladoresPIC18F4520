#include <xc.h>
#include <pic16f88.h>
#include "lcd4a.h"

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
