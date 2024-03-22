#include "mychain.h"

int main()
{
    sPoly pFy; 
    sPoly pFx;
    sPoly pResult;
    uint32_t y_size = 0;
    uint32_t x_size = 0;
    scanf("%d", &y_size);
    pFy.size = y_size;
    scanf("%d", &x_size);
    pFx.size = x_size;
    pFy.pPowers = (uint32_t *)malloc(y_size * sizeof(uint32_t));
    pFy.pCoefficients = (int32_t *)malloc(y_size * sizeof(int32_t));
    pFx.pPowers = (uint32_t *)malloc(x_size * sizeof(uint32_t));
    pFx.pCoefficients = (int32_t *)malloc(x_size * sizeof(int32_t));
    for(uint32_t i = 0; i < y_size; i++)
    {
        scanf("%d", &pFy.pPowers[i]);
    }
    for(uint32_t i = 0; i < y_size; i++)
    {
        scanf("%d", &pFy.pCoefficients[i]);
    }
    for(uint32_t i = 0; i < x_size; i++)
    {
        scanf("%d", &pFx.pPowers[i]);
    }
    for(uint32_t i = 0; i < x_size; i++)
    {
        scanf("%d", &pFx.pCoefficients[i]);
    }
    chain_rule(&pResult, &pFy, &pFx);
    printf("pResult.size = %d\n", pResult.size);
    for (uint32_t i = 0; i < pResult.size; i++)
    {
        printf("pResult.pPowers[%d] = %d\n", i, pResult.pPowers[i]);
        printf("pResult.pCoefficients[%d] = %d\n", i, pResult.pCoefficients[i]);
    }
    free(pResult.pPowers);
    free(pResult.pCoefficients);
    free(pFy.pPowers);
    free(pFy.pCoefficients);
    free(pFx.pPowers);
    free(pFx.pCoefficients);
    return 0;
}