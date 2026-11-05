#include <stdio.h>

int main(void) {
    int numero = 5;

    if (numero > 0) {
        printf("El numero es positivo\n");
    } else {
        while (numero < 0) {
            numero++;
        }
    }
    return 0;
}
