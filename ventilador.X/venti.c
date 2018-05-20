/*
 * File:   venti.c
 * Author: miguel
 *
 * Created on 14 de mayo de 2018, 07:46 PM
 * Este programa usa la libreria de lógica
 * difusa propuesta por el software XFuzzy 3.3
 * 
 */


#include <xc.h>
#include <ventilador.h>

void main(void)
{
    float in, out;
    ventiladorInferenceEngine(in, &out);
    return;
}
