#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int32_t is_invalid(char *pExpr, int32_t base);
static int32_t to_dec(char *pNUm, int32_t base);
static char *to_base(int32_t dec, int32_t base);

// Input:
// char *pExpr: the arithmetic expression.
// int32_t base: the base that is used to show the arithmetic result. (2-16)
// Output:
// char **ppResult: the arithmetic result string.
// Return:
// 0: Success; -1: Error input
int32_t calculate(char *pExpr, int32_t base, char **ppResult);

static int32_t is_invalid(char *pExpr, int32_t base)
{
    char *pCheck = pExpr;
    uint64_t pass = 0;
    sscanf(pExpr, "%*[0-9A-F_+-*]%n", &pass);
    if (pass != strlen(pExpr))
        return -1;
    // space not_same_advance
    while (*pCheck != '\0')
    {
        if (*pCheck == '+' || *pCheck == '-' || *pCheck == '*')
        {
            if (*(pCheck + 1) == ' ')
                return -1;
            if ((pCheck != pExpr) && *(pCheck - 1) == ' ')
                return -1;
        }
        pCheck++;
    }
    if(*pExpr == '+' || *pExpr == '-' || *pExpr == '*')
        return -1;
}

static int32_t to_dec(char *pNum, int32_t base)
{
}
static char *to_base(int32_t dec, int32_t base)
{
}