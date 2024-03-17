#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "myvector.h"

#define fptf fprintf

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
static int8_t ytox(int64_t **pVResultPowers, int64_t **pVResultCoefficients, uint32_t *size, int64_t *pXPowers, int64_t *pXCoefficients, uint32_t xsize, uint32_t power, int32_t ycof, int64_t *pXpPowers, int64_t *pXpCoefficients, uint32_t xpsize);
static int8_t dot(int64_t *pAPowers, int64_t *pACoefficients, uint32_t aSize, int64_t *pBPowers, int64_t *pBCoefficients, uint32_t bSize, int64_t **pResultPowers, int64_t **pResultCoefficients, uint32_t *size);
static int8_t merge(int64_t **pTargetPowers, int64_t **pTargetCoefficients, uint32_t *targetSize);

static int8_t dot(int64_t *pAPowers, int64_t *pACoefficients, uint32_t aSize, int64_t *pBPowers, int64_t *pBCoefficients, uint32_t bSize, int64_t **pResultPowers, int64_t **pResultCoefficients, uint32_t *size)
{
    if (pAPowers == NULL || pACoefficients == NULL || pBPowers == NULL || pBCoefficients == NULL || pResultPowers == NULL || pResultCoefficients == NULL)
    {
        return -1;
    }
    for (uint32_t i = 0; i < aSize; i++)
    {
        for (uint32_t j = 0; j < bSize; j++)
        {
            Vector_push_back(pResultPowers, *(pAPowers + i) + *(pBPowers + j), *size);
            Vector_push_back(pResultCoefficients, *(pACoefficients + i) * *(pBCoefficients + j), *size);
            (*size)++;
        }
    }
    merge(pResultPowers, pResultCoefficients, size);
    return 0;
}
static int8_t merge(int64_t **pTargetPowers, int64_t **pTargetCoefficients, uint32_t *targetSize)
{
    if (pTargetPowers == NULL || pTargetCoefficients == NULL || targetSize == NULL)
    {
        return -1;
    }
    // sort the powers
    for (uint32_t i = 0; i < *targetSize; i++)
    {
        for (uint32_t j = 0; j < *targetSize - i - 1; j++)
        {
            if (*(*pTargetPowers + j) < *(*pTargetPowers + j + 1))
            {
                int64_t temp = *(*pTargetPowers + j);
                *(*pTargetPowers + j) = *(*pTargetPowers + j + 1);
                *(*pTargetPowers + j + 1) = temp;
                temp = *(*pTargetCoefficients + j);
                *(*pTargetCoefficients + j) = *(*pTargetCoefficients + j + 1);
                *(*pTargetCoefficients + j + 1) = temp;
            }
        }
    }
    // merge the same powers
    for (uint32_t i = 0; i < *targetSize; i++)
    {
        for (uint32_t j = i + 1; j < *targetSize; j++)
        {
            if (*(*pTargetPowers + i) == *(*pTargetPowers + j))
            {
                *(*pTargetCoefficients + i) += *(*pTargetCoefficients + j);
                for (uint32_t k = j; k < *targetSize - 1; k++)
                {
                    *(*pTargetPowers + k) = *(*pTargetPowers + k + 1);
                    *(*pTargetCoefficients + k) = *(*pTargetCoefficients + k + 1);
                }
                (*targetSize)--;
                j--;
            }
        }
    }
    return 0;
}

static int8_t ytox(int64_t **pVResultPowers, int64_t **pVResultCoefficients, uint32_t *size, int64_t *pXPowers, int64_t *pXCoefficients, uint32_t xsize, uint32_t power, int32_t ycof, int64_t *pXpPowers, int64_t *pXpCoefficients, uint32_t xpsize)
{
    if (pVResultPowers == NULL || pVResultCoefficients == NULL)
    {
        return -1;
    }
    if (power == 0)
    {
        Vector_push_back(pVResultPowers, 0, *size);
        Vector_push_back(pVResultCoefficients, ycof, *size);
        return 0;
    }
    // if (power == 1)
    // {
    //     for (uint32_t i = 0; i < xsize; i++)
    //     {
    //         Vector_push_back(pVResultPowers, *(pXPowers + i), *size);
    //         Vector_push_back(pVResultCoefficients, *(pXCoefficients + i) * ycof, *size);
    //         (*size)++;
    //     }
    //     return 0;
    // }
    int64_t *vecP = Vector_create(0);
    int64_t *vecC = Vector_create(0);
    uint32_t vsize = 0;
    Vector_push_back(&vecP, 0, vsize);
    Vector_push_back(&vecC, 1, vsize);
    vsize++;
    int8_t flag = 0;
    for (uint32_t i = 0; i < power; i++)
    {
        int64_t *tempP = Vector_create(0);
        int64_t *tempC = Vector_create(0);
        uint32_t tsize = 0;
        dot((int64_t *)pXPowers, (int64_t *)pXCoefficients, xsize, vecP, vecC, vsize, &tempP, &tempC, &tsize);
        for (uint32_t j = 0; j < tsize; j++)
        {
            Vector_push_back(&vecP, *(tempP + j), vsize);
            Vector_push_back(&vecC, *(tempC + j), vsize);
            vsize++;
        }
        Vector_free(tempP);
        Vector_free(tempC);
        if (flag == 0)
        {
            flag = 1;
            Vector_erase(&vecP, 0, vsize);
            Vector_erase(&vecC, 0, vsize);
            vsize--;
        }
        merge(&vecP, &vecC, &vsize);
    }
    int64_t *tempP = Vector_create(0);
    int64_t *tempC = Vector_create(0);
    uint32_t tsize = 0;
    dot(pXpPowers, pXpCoefficients, xpsize, vecP, vecC, vsize, &tempP, &tempC, &tsize);
    Vector_free(vecP);
    Vector_free(vecC);
    vecP = tempP;
    vecC = tempC;
    vsize = tsize;
    for (uint32_t i = 0; i < vsize; i++)
    {
        Vector_push_back(pVResultPowers, *(vecP + i), *size);
        Vector_push_back(pVResultCoefficients, *(vecC + i) * ycof, *size);
        (*size)++;
    }
    Vector_free(vecP);
    Vector_free(vecC);
    return 0;
}

int32_t chain_rule(sPoly *pResult, const sPoly *pFy, const sPoly *pFx)
{
    if (pFy == NULL || pFx == NULL || pResult == NULL)
    {
        return -1;
    }
    uint32_t ypsize = pFy->size;
    int64_t *Vector_yP_powers = Vector_create(ypsize);
    int64_t *Vector_yP_coefficients = Vector_create(ypsize);
    for (uint32_t i = 0; i < ypsize; i++)
    {
        *(Vector_yP_powers + i) = (int64_t) * (pFy->pPowers + i);
        *(Vector_yP_coefficients + i) = (int64_t) * (pFy->pCoefficients + i);
    }
    merge(&Vector_yP_powers, &Vector_yP_coefficients, &ypsize);
    for (uint32_t i = 0; i < ypsize; i++)
    {
        if (Vector_yP_powers[i] == 0)
        {
            Vector_pop_back(&Vector_yP_powers, ypsize);
            Vector_pop_back(&Vector_yP_coefficients, ypsize);
            ypsize--;
            break;
        }
        Vector_yP_coefficients[i] *= Vector_yP_powers[i];
        Vector_yP_powers[i]--;
    }
    uint32_t xpsize = pFx->size;
    int64_t *Vector_xP_powers = Vector_create(xpsize);
    int64_t *Vector_xP_coefficients = Vector_create(xpsize);
    for (uint32_t i = 0; i < xpsize; i++)
    {
        *(Vector_xP_powers + i) = (int64_t) * (pFx->pPowers + i);
        *(Vector_xP_coefficients + i) = (int64_t) * (pFx->pCoefficients + i);
    }
    merge(&Vector_xP_powers, &Vector_xP_coefficients, &xpsize);
    int64_t *Vector_x_powers = Vector_create(xpsize);
    int64_t *Vector_x_coefficients = Vector_create(xpsize);
    uint32_t xsize = xpsize;
    for (uint32_t i = 0; i < xpsize; i++)
    {
        *(Vector_x_powers + i) = *(Vector_xP_powers + i);
        *(Vector_x_coefficients + i) = *(Vector_xP_coefficients + i);
    }
    for (uint32_t i = 0; i < xpsize; i++)
    {
        if (Vector_xP_powers[i] == 0)
        {
            Vector_pop_back(&Vector_xP_powers, xpsize);
            Vector_pop_back(&Vector_xP_coefficients, xpsize);
            xpsize--;
            break;
        }
        Vector_xP_coefficients[i] *= Vector_xP_powers[i];
        Vector_xP_powers[i]--;
    }

    int64_t *Vector_ResultPowers = Vector_create(0);
    int64_t *Vector_ResultCoefficients = Vector_create(0);
    uint32_t size = 0;
    for (uint32_t i = 0; i < ypsize; i++)
    {
        if (ytox(&Vector_ResultPowers, &Vector_ResultCoefficients, &size, Vector_x_powers, Vector_x_coefficients, xsize, Vector_yP_powers[i], Vector_yP_coefficients[i], Vector_xP_powers, Vector_xP_coefficients, xpsize) == -1)
        {
            return -1;
        }
        merge(&Vector_ResultPowers, &Vector_ResultCoefficients, &size);
    }
    for (uint32_t i = 0; i < size; i++)
    {
        if (Vector_ResultCoefficients[i] == 0)
        {
            Vector_erase(&Vector_ResultPowers, i, size);
            Vector_erase(&Vector_ResultCoefficients, i, size);
            size--;
        }
    }
    // copy to uint32_t Power
    uint32_t answerPower[size];
    int32_t answerCof[size];
    for (uint32_t i = 0; i < size; i++)
    {
        answerPower[i] = (uint32_t)Vector_ResultPowers[i];
        answerCof[i] = (int32_t)Vector_ResultCoefficients[i];
    }
    Vector_free(Vector_yP_powers);
    Vector_free(Vector_yP_coefficients);
    Vector_free(Vector_xP_powers);
    Vector_free(Vector_xP_coefficients);
    Vector_free(Vector_x_powers);
    Vector_free(Vector_x_coefficients);
    Vector_free(Vector_ResultPowers);
    Vector_free(Vector_ResultCoefficients);
    pResult->size = size;
    pResult->pPowers = answerPower;
    pResult->pCoefficients = answerCof;
    return 0;
}