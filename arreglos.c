#include <stdio.h>
#define NUM_MATERIAS 5 // Se define constante simbolica

void main()
{
    // Definición de variables
    float calificaciones[NUM_MATERIAS];
    float calificaciones_ordenadas[NUM_MATERIAS];
    char nombre[20];
    float suma = 0.0;
    float promedio, max_calificacion, min_calificacion;

    // Ingresar datos de materias
    printf("|||Promedio de Calificaciones|||\n");
    printf("Ingrese las %d calificaciones:\n", NUM_MATERIAS);

    // Bucle para ingresar datos y calcular suma, max y min
    for (int i = 0; i < NUM_MATERIAS; i++)
    {
        printf("Nombre del alumno: ");
        scanf("%s", nombre); // Se lee el nombre del alumno (no se usa en cálculos)
        printf("Calificacion %d: ", i + 1);
        scanf("%f", &calificaciones[i]);

        suma += calificaciones[i]; // Suma de calificaciones
        // Si es el primer elemento, inicializamos max y min con ese valor
        if (i == 0)
        {
            max_calificacion = calificaciones[i];
            min_calificacion = calificaciones[i];
        }
        else
        {
            if (calificaciones[i] > max_calificacion)
            {
                max_calificacion = calificaciones[i]; // Actualiza max
                calificaciones_ordenadas[i] = calificaciones[i]; // Guarda en arreglo ordenado (aunque no se ordena realmente)
            }
            if (calificaciones[i] < min_calificacion)
            {
                min_calificacion = calificaciones[i]; // Actualiza min
            }
        }
    }
    // Calcular promedio
    promedio = suma / NUM_MATERIAS;

    // Mostrar resultados
    printf("\n RESULTADOS\n");
    printf("======================\n");
    printf("Calificaciones ingresadas:\n");
    for (int i = 0; i < NUM_MATERIAS; i++)
    {
        printf("Materia %d: %.2f \n", i + 1, calificaciones[i]);
    }
    printf("---------------------------------\n");
    printf("Promedio: %.2f\n", promedio);
    printf("Calificacion mas alta: %.2f\n", max_calificacion);
    printf("Calificacion mas baja: %.2f\n", min_calificacion);
    printf("======================\n");
}
