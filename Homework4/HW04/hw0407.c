#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// #include <math.h>
#include <tgmath.h>

int main()
{
    printf("%ld\n",sizeof(pow(2.0F, 3.0F))); // float
    printf("%ld\n",sizeof(pow(2L, 3.0F))); // double
    printf("%ld\n",sizeof(pow(2.0, 3))); // double
    printf("%ld\n",sizeof(pow(2.0F, 3.0L))); // long double
    printf("%ld\n",sizeof(pow(2.0L, 3))); // long double
    return 0;
}