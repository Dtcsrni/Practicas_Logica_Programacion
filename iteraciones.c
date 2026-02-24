#include <stdio.h>
void main(){
    int numero, iteraciones; 
    printf("::Generador de Tablas de Multiplicar::\n");
    printf("Ingrese un número entero\n");
    scanf("%d", &numero);

    printf("\nIngrese la cantidad de iteraciones a ejecutar \n", numero);
    scanf("%d", &iteraciones);

    //Cabecera de la tabla de multiplicar
    printf("\nTabla de multiplicar del %d \n", numero);
    printf("-----------------------------\n");
    //Bucle del 1 al 10 
    for(int i = 1; i<=iteraciones;i++){
        //%2d asegura que el numero se alinee a la derecha con 2 espacios
        int resultado = numero * i;
        printf("%2d x %2d = %3d\n", numero, i, resultado);
    }
    printf("-----------------------------\n");

    }