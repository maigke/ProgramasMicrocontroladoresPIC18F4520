

/**
Funcion para configurar el baudrate
* los parámetros estan fijado a 9600 bps
* con oscilador de 9600
*/

void sci_conf(void);


/**
 * Lee un dato del puerto serial
 * **/

unsigned char getch(void);

/**
 * Funciones para que trabaje con printf
 * Escribe un valor por el puerto serial
 * */
void putch(unsigned char dato);

/**
 * Lee un dato desde el puerto serial
 * con eco
 * **/
unsigned char getche(void);
