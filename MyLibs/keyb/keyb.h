/**@file keyb.h
 @brief Funciones y macros para leer un teclado matricial simple de 4 x 4 se 
 requiere de poner a 

 @code #include "clk.h" @endcode
 
 Antes de:
 @code #include "keyb.h" @endcode
 
 Para posibilitar la compilacion
 @author Miguel Montiel Mtz                                                                    */
/************************************************************************/

#ifndef KEYB_H
#define KEYB_H


/**
@name Macro Macros para definir las terminales a conectar al teclado de 4 x 4                                                                     
***********************************************************************/
#define PORT_KEY_R1  PORTD/** Puerto R1*/
#define PIN_KEY_R1  0/**Pin R1*/
#define PORT_KEY_R2 PORTD/** Puerto R2*/
#define PIN_KEY_R2  1/**Pin R2*/
#define PORT_KEY_R3 PORTD/** Puerto R3*/
#define PIN_KEY_R3  2/**Pin R3*/
#define PORT_KEY_R4 PORTD/** Puerto R4*/
#define PIN_KEY_R4  3/**Pin R4*/

#define PORT_KEY_C1 PORTB/** Puerto C1*/
#define PIN_KEY_C1  0/**Pin c1*/
#define PORT_KEY_C2 PORTB/** Puerto C2*/
#define PIN_KEY_C2  1/**Pin C2*/
#define PORT_KEY_C3 PORTB/** Puerto C3*/
#define PIN_KEY_C3  3/**Pin C3*/
#define PORT_KEY_C4 PORTB/** Puerto C4*/
#define PIN_KEY_C4  2/**Pin C4*/

/**
@name Macro para El uso del teclado matricial
*/

/**
@name Macro para setear la salidas del teclado matricial e iniciar escaneo de columnas 
*/
#define vSetKeyPadR1() {clrbit(PORT_KEY_R1, PIN_KEY_R1);  setbit(PORT_KEY_R2, PIN_KEY_R2); \
						setbit(PORT_KEY_R3, PIN_KEY_R3);  setbit(PORT_KEY_R4, PIN_KEY_R4);} ///< Poner cero en R1

#define vSetKeyPadR2() {setbit(PORT_KEY_R1, PIN_KEY_R1);  clrbit(PORT_KEY_R2, PIN_KEY_R2); \
						setbit(PORT_KEY_R3, PIN_KEY_R3);  setbit(PORT_KEY_R4, PIN_KEY_R4);} ///< Poner cero en R2
						
#define vSetKeyPadR3() {setbit(PORT_KEY_R1, PIN_KEY_R1);  setbit(PORT_KEY_R2, PIN_KEY_R2); \
						clrbit(PORT_KEY_R3, PIN_KEY_R3);  setbit(PORT_KEY_R4, PIN_KEY_R4);} ///< Poner cero en R3
						
#define vSetKeyPadR4() {setbit(PORT_KEY_R1, PIN_KEY_R1);  setbit(PORT_KEY_R2, PIN_KEY_R2); \
						setbit(PORT_KEY_R3, PIN_KEY_R3);  clrbit(PORT_KEY_R4, PIN_KEY_R4);} ///< Poner cero en R4

#define vSetKeyPad() {setbit(PORT_KEY_R1, PIN_KEY_R1);  setbit(PORT_KEY_R2, PIN_KEY_R2); \
						setbit(PORT_KEY_R3, PIN_KEY_R3);  setbit(PORT_KEY_R4, PIN_KEY_R4);} ///< Poner todos los R en alto

/**
 *  @name Functions Funciones para hacer funcionar al teclado
 */

/**
 @fn vInitKeyPad
 @brief    Funcion para inicializar teclado matricial
 @param    void
 @return   none
 
    Ejemplo:
    \verbatim
    vInitKeyPad();
    \endverbatim
*/
void vInitKeyPad(void);

/**
 @fn ui8_TestKeyPad
 @brief funcion que permite escanear el teclado y devuelve un valor entre 0 y 16
 dependiendo si se presiona alguna tecla o no
 @param none
 @returns devuelve un tipo char sin signo entre 0 a 16, si no se presiona ninguna
 tecla devuelve 16, si se presiona una tecla regresa un valor de 0 a 15
 
   Ejemplo:
   \verbatim
   tecla=ui8_TestKeyPad();
   \endverbatim                                                                     */

unsigned char ui8_TestKeyPad(void);




#endif
