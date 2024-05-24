#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define Peval(cmd) printf( #cmd ": %g\n", cmd );

int main()
{
    double *plist = (double[]){1, 2, 3};
    double list[] = {1, 2, 3};
    Peval( sizeof( plist ) / ( sizeof( double ) + 0.0 ) );
    Peval( sizeof( list ) / ( sizeof( double ) + 0.0 ) );
    return 0;
}