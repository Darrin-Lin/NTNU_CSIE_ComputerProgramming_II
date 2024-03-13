#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


// Input:
// char *pExpr: the arithmetic expression.
// int32_t base: the base that is used to show the arithmetic result. (2-16)
// Output:
// char **ppResult: the arithmetic result string.
// Return:
// 0: Success; -1: Error input
int32_t calculate( char *pExpr , int32_t base, char **ppResult );

static int32_t is_invalid( char *pExpr, int32_t base );
static int32_t to_dec( char *pExpr, int32_t base );
static char *to_base( int32_t dec, int32_t base );

