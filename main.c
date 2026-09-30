#include <stdio.h>
#include "conversor.h"

int main(void){
    float metros;
    printf("Digite o valor em metros: ");
    scanf("%f", &metros);

    float cm = MparaC(metros);
    float km = MparaQ(metros);
    float mm = MparaMili(metros);

    printf("Centimetros: %f\n", cm);
    printf("Quilometros: %f\n", km);
    printf("Milimetros:  %f\n", mm);

    return 0;
}