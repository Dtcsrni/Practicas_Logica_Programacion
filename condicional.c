#include <stdio.h>

int main(){
    int numero;
    printf("--Verificador de Numeros--\n");
    printf("Introduce un numero entero:");
    scanf("%d", &numero); 

    //Verificar signo ingresado
    if(numero > 0){
        printf("El numero %d es POSITIVO", numero);
    }else if(numero < 0){
        printf("El numero %d es NEGATIVO", numero);
    }else{
        printf("El numero es CERO");
    }

    //Verificar si el numero es par o impar
    if(numero % 2 == 0){
        printf("\nEl numero %d es PAR", numero);
    }else{
        printf("\nEl numero %d es IMPAR", numero);
    }

    return 0;
}