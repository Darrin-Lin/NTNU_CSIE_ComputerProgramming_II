#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "myvector.h"

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
static int8_t ytox(int64_t **pVResultPowers, int64_t **pVResultCoefficients, size_t *size, uint32_t *pXPowers, int32_t *pXCoefficients, uint32_t xsize, uint32_t power,int32_t ycof);

static int8_t ytox(int64_t **pVResultPowers, int64_t **pVResultCoefficients, size_t *size, uint32_t *pXPowers, int32_t *pXCoefficients, uint32_t xsize, uint32_t power, int32_t ycof)
{
    if (pVResultPowers == NULL || pVResultCoefficients == NULL)
    {
        return -1;
    }
    if (power == 0)
    {
        return 0;
    }
    int64_t *vecP = Vector_create(0);
    int64_t *vecC = Vector_create(0);
    uint32_t vsize = *size;
    for (uint32_t i = 0; i < power; i++)
    {
        int64_t *ssaveP = Vector_create(0);
        int64_t *ssaveC = Vector_create(0);
        uint32_t CPssize = 0;
        for (uint32_t j = 0; j < *size; j++)
        {
            int64_t *saveP = Vector_create(0);
            int64_t *saveC = Vector_create(0);
            uint32_t CPsize = 0;
            for (uint32_t k = 0; k < xsize; k++)
            {

                Vector_push_back(saveP, *(ssaveP + j) + *(pXPowers), (size_t)CPsize);
                Vector_push_back(saveC, *(ssaveC + j) * (*(pXCoefficients + k)), (size_t)CPsize);
                CPsize++;
            }
            uint32_t remain = CPsize;
            for (uint32_t k = 0; k < CPssize; k++)
            {
                if (*(ssaveP + k) < *(saveP + CPsize - remain))
                {
                    Vector_insert(ssaveP, *(saveP + CPsize - remain), k, (size_t)CPssize);
                    Vector_insert(ssaveC, *(saveP + CPsize - remain), k, (size_t)CPssize);
                    CPssize++;
                    remain--;
                    k--;
                }
                else if (*(ssaveP + k) == *(saveP + CPsize - remain))
                {
                    *(ssaveC + k) += *(saveC + CPsize - remain);
                    remain--;
                }
            }
            for (uint32_t k = 0; k < remain; k++)
            {
                Vector_push_back(ssaveP, *(saveP + CPsize - remain), (size_t)CPssize);
                Vector_push_back(ssaveC, *(saveC + CPsize - remain), (size_t)CPssize);
                CPssize++;
            }
            Vector_free(saveP);
            Vector_free(saveC);
        }
        uint32_t remain = CPssize;
        for (uint32_t j = 0; j < vsize; j++)
        {
            if (*(vecP + j) < *(ssaveP + CPssize - remain))
            {
                Vector_insert(vecP, *(ssaveP + CPssize - remain), j, (size_t)vsize);
                Vector_insert(vecC, *(ssaveC + CPssize - remain)*ycof, j, (size_t)vsize);
                vsize++;
                remain--;
                j--;
            }
            else if (*(vecP + j) == *(ssaveP + CPssize - remain))
            {
                *(vecC + j) += *(ssaveC + CPssize - remain)*ycof;
                remain--;
            }
        }
        for (uint32_t j = 0; j < remain; j++)
        {
            Vector_push_back(vecP, *(ssaveP + CPssize - remain), (size_t)vsize);
            Vector_push_back(vecC, *(ssaveC + CPssize - remain)*ycof, (size_t)vsize);
            vsize++;
        }

        Vector_free(ssaveP);
        Vector_free(ssaveC);
    }
    if(*pVResultCoefficients!=NULL && *pVResultPowers!=NULL){
        uint32_t remain = vsize;
        for(uint32_t i = 0; i < *size; i++){
            if(*(*pVResultPowers + i) < *(vecP + vsize - remain)){
                Vector_insert(*pVResultPowers, *(vecP + vsize - remain), i, (size_t)*size);
                Vector_insert(*pVResultCoefficients, *(vecC + vsize - remain), i, (size_t)*size);
                *size++;
                remain--;
                i--;
            }
            else if(*(*pVResultPowers + i) == *(vecP + vsize - remain)){
                *(*pVResultCoefficients + i) += *(vecC + vsize - remain);
                remain--;
            }
        }
        for(uint32_t i = 0; i < remain; i++){
            Vector_push_back(*pVResultPowers, *(vecP + vsize - remain), (size_t)*size);
            Vector_push_back(*pVResultCoefficients, *(vecC + vsize - remain), (size_t)*size);
            *size++;
        }
    }
    else{
        *pVResultPowers = vecP;
        *pVResultCoefficients = vecC;
        *size = vsize;
    }
    return 0;
}

int32_t chain_rule(sPoly *pResult, const sPoly *pFy, const sPoly *pFx)
{
    if (pFy == NULL || pFx == NULL || pResult == NULL)
    {
        return -1;
    }
    uint32_t ysize = pFy->size;
    int64_t *Vector_yP_powers = Vector_create(ysize);
    int64_t *Vector_yP_coefficients = Vector_create(ysize);
    for (uint32_t i = 0; i < ysize; i++)
    {
        *(Vector_yP_powers + i) = (int64_t) * (pFy->pPowers + i);
        *(Vector_yP_coefficients + i) = (int64_t) * (pFy->pCoefficients + i);
    }
    for (uint32_t i = 0; i < ysize; i++)
    {
        if (Vector_yP_powers[i] == 0)
        {
            Vector_pop_back(Vector_yP_powers, ysize);
            Vector_pop_back(Vector_yP_coefficients, ysize);
            ysize--;
            break;
        }
        Vector_yP_coefficients[i] *= Vector_yP_powers[i];
        Vector_yP_powers[i]--;
    }
    uint32_t xsize = pFx->size;
    int64_t *Vector_xP_powers = Vector_create(xsize);
    int64_t *Vector_xP_coefficients = Vector_create(xsize);
    for (uint32_t i = 0; i < xsize; i++)
    {
        *(Vector_xP_powers + i) = (int64_t) * (pFx->pPowers + i);
        *(Vector_xP_coefficients + i) = (int64_t) * (pFx->pCoefficients + i);
    }
    for (uint32_t i = 0; i < xsize; i++)
    {
        if (Vector_xP_powers[i] == 0)
        {
            Vector_pop_back(Vector_xP_powers, xsize);
            Vector_pop_back(Vector_xP_coefficients, xsize);
            xsize--;
            break;
        }
        Vector_xP_coefficients[i] *= Vector_xP_powers[i];
        Vector_xP_powers[i]--;
    }
    int64_t *Vector_ResultPowers = NULL;
    int64_t *Vector_ResultCoefficients = NULL;
    size_t size = 0;
    for (uint32_t i = 0; i < ysize; i++)
    {
        if (ytox(&Vector_ResultPowers, &Vector_ResultCoefficients, &size, Vector_xP_powers, Vector_xP_coefficients, xsize, Vector_yP_powers[i],Vector_yP_coefficients[i]) == -1)
        {
            return -1;
        }
    }

    // for()

    pResult->size = size;
    pResult->pPowers = (uint32_t *)malloc(size * sizeof(uint32_t));
    pResult->pCoefficients = (int32_t *)malloc(size * sizeof(int32_t));
}