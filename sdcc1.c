/* ----------------------------------------------------------------------- */
/* Plantilla generada por Piklab */
#include <pic18fregs.h>
#include <delay.h>
#include <stdint.h>

//#pragma config XINST=OFF
#pragma config WDT=OFF
#pragma config LVP=OFF


void HighPriorityISR() __interrupt 1 {
// Insert high priority interrupt code here
}

void LowPriorityISR() __interrupt 2 {
// Insert low priority interrupt code here
}


uint8_t number_to_7seg(uint8_t number)
{
	switch (number) {
		case 0:
			return 0x3F;
		case 1:
			return 0x06;
		case 2:
			return 0x5B;
		case 3:
			return 0x4F;
		case 4:
			return 0x66;
		case 5:
			return 0x6D;
		case 6:
			return 0x7D;
		case 7:
			return 0x07;
		case 8:
			return 0x7F;
		case 9:
			return 0x6F;
		default:
			return 0;
	}
}


void main() {
    /* << agregue el código >> */
    uint8_t i;
    
    TRISB= 0;
    LATB= 0;
    while(1)
    {
      for(i=0;i<10;i++)
      {
	LATB = number_to_7seg(i);
	//delay1ktcy(250);
      }
    }
      
}
