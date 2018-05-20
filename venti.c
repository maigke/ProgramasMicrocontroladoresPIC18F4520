#include <stdio.h>
#include "ventilador.h"

int main(void)
{
double vel;

ventiladorInferenceEngine(27,vel);
printf("La velocidad de salida es: %f",vel);
return 0;
}
