#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#pragma once

// Input:
// char *pExpr: the arithmetic expression.
// int32_t base: the base that is used to show the arithmetic result. (2-16)
// Output:
// char **ppResult: the arithmetic result string.
// Return:
// 0: Success; -1: Error input
int32_t calculate( char *pExpr , int32_t base, char **ppResult );