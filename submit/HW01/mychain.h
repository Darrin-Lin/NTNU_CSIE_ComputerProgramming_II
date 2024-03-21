#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdint.h>

#pragma once

// Polynomial structures
// Example:
// f(x) = x^3-2x+1
// size = 3
// pPowers: [3,1,0]
// pCoefficients: [1,-2,1]
typedef struct _sPoly
{
    uint32_t size;
    uint32_t *pPowers;
    int32_t *pCoefficients;
} sPoly;
// Input:
// *pFy: z = f1(y);
// *pFx: y = f2(x);
// Note that pPower can be in any orders.
// Output:
// *pResult = dz/dx
// Note that pPower should be in the descending order.
// Return:
// 0: Success; -1: Error input
int32_t chain_rule(sPoly *pResult, const sPoly *pFy, const sPoly *pFx);