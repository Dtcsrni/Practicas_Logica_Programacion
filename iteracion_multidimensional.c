#include <stdio.h>
#define FILAS 3
#define COLUMNAS 3

void main()
{
    int numAlumnos, numAsignaturas;
    int matriz[FILAS][COLUMNAS] = {0}; // Inicializar la matriz con ceros
    int sumaFila[FILAS] = {0};
    int sumaColumna[COLUMNAS] = {0};
    int sumaTotal = 0;
    // Mensaje de bienvenida con datos del alumno
    printf("------------------------------------\n");
    printf("-|Nombre de alumno: Alumno-\n");
    printf("-|Grupo: Grupo - \n");

    /// Llenado de matriz
    printf("Ingrese los valores de la matriz %d x %d \n", FILAS, COLUMNAS);
    for (int i = 0; i < FILAS; i++)
    {
        for (int j = 0; j < COLUMNAS; j++)
        {
            printf("Ingrese el valor para la posicion [%d][%d]: ", i, j);
            scanf("%d", &matriz[i][j]);
        }
    }

    // Mostrar matriz a usuario
    printf("Matriz ingresada:\n");
    printf("------------------\n");
    for (int i = 0; i < FILAS; i++)
    {
        for (int j = 0; j < COLUMNAS; j++)
        {
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
    printf("------------------\n");

    // Calculo de sumas por filas, por columna y total
    for (int i = 0; i < FILAS; i++)
    {
        for (int j = 0; j < COLUMNAS; j++)
        {
            sumaFila[i] += matriz[i][j];
            sumaColumna[j] += matriz[i][j];
            sumaTotal += matriz[i][j];
        }
    }

    //Mostrar resultados

    printf("Resultados:\n");
    printf("-------------------\n");
    //Sumas por fila
    for(int i = 0; i < FILAS; i++)
    {
        printf("Suma de la fila %d: %d\n", i, sumaFila[i]);
    }

    //Sumas por columna
    for(int j = 0; j < COLUMNAS; j++)
    {
        printf("Suma de la columna %d: %d\n", j, sumaColumna[j]);
    }

    //Suma total
    printf("Suma total de la matriz: %d\n", sumaTotal);
    printf("-------------------\n");


}