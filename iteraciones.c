#include <stdio.h>
void main()
{
    int numero, iteraciones;
    int condiciones = 0;
    while (condiciones != 2)
    { 
        printf("::Generador de Tablas de Multiplicar::\n");
        printf("Ingrese un número entero\n");
        scanf("%d", &numero);
        if (numero % 2 == 0)
        {
            printf("El numero %d es PAR y es entero\n", numero);
            condiciones++;
        }
        else
        {
            printf("El numero es fraccionario, favor de volver a ingresarlo");
        }
        printf("\nIngrese la cantidad de iteraciones a ejecutar \n", numero);
        scanf("%d", &iteraciones);
        if (iteraciones % 2 == 0)
        {
            printf("La cifra de iteraciones %d es entero\n", numero);
            condiciones++;
        }

        if (condiciones < 2)
        {
            condiciones = 0;
            numero = 0;
            iteraciones = 0;
        }
    }

    // Cabecera de la tabla de multiplicar
    printf("\nTabla de multiplicar del %d \n", numero);
    printf("-----------------------------\n");
    // Bucle del 1 al 10
    for (int i = 1; i <= iteraciones; i++)
    {
        //%2d asegura que el numero se alinee a la derecha con 2 espacios
        int resultado = numero * i;
        printf("%2d x %2d = %3d\n", numero, i, resultado);
    }
    printf("-----------------------------\n");
}