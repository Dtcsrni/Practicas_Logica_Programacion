#include "consola.h"
#include <stdio.h>
#include <stdlib.h>

void consola_limpiar_simple(void){
    //Se ejecutan 40 saltos de linea para ocultar el frame previo
    for (int i = 0; i < 5; i++){
        putchar('\n');
    }
    system("cls");
}