#include "mychain.h"

int main()
{
    sPoly pFy = {1, (uint32_t[]){2}, (int32_t[]){1}};
    sPoly pFx = {2, (uint32_t[]){1,0}, (int32_t[]){4,-3}};
    sPoly pResult;
    chain_rule(&pResult, &pFy, &pFx);
    printf("pResult.size = %d\n", pResult.size);
    for (uint32_t i = 0; i < pResult.size; i++)
    {
        printf("pResult.pPowers[%d] = %d\n", i, pResult.pPowers[i]);
        printf("pResult.pCoefficients[%d] = %d\n", i, pResult.pCoefficients[i]);
    }
    free(pResult.pPowers);
    free(pResult.pCoefficients);
    return 0;
}