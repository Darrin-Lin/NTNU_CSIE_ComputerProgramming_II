#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "myvector.h"

#define fptf fprintf

static int32_t is_invalid(char *pExpr);
static char *to_base(int32_t dec, int32_t base);

// Input:
// char *pExpr: the arithmetic expression.
// int32_t base: the base that is used to show the arithmetic result. (2-16)
// Output:
// char **ppResult: the arithmetic result string.
// Return:
// 0: Success; -1: Error input
int32_t calculate(char *pExpr, int32_t base, char **ppResult);

static int32_t is_invalid(char *pExpr)
{
    char *pCheck = pExpr;
    uint64_t pass = 0;
    // sscanf(pExpr, "%*[0-9A-F_+-*]%ln", &pass);
    for (size_t i = 0; i < strlen(pExpr); i++)
    {
        if (strchr("0123456789ABCDEF_+-* ", pExpr[i]) != NULL)
        {
            pass++;
        }
    }
    if (pass != strlen(pExpr))
        return -1;
    // space not_same_advance
    while (*pCheck != '\0')
    {
        if (*pCheck == '+' || *pCheck == '-' || *pCheck == '*')
        {
            if (pCheck <= 0 || *(pCheck + 1) != ' ')
                return -1;
            if (pCheck >= strlen(pExpr) - 1 || *(pCheck - 1) != ' ')
                return -1;
        }
        pCheck++;
    }
    if (*pExpr == '+' || *pExpr == '-' || *pExpr == '*')
        return -1;
    return 0;
}

static char *to_base(int32_t dec, int32_t base)
{
    int8_t neg = 0;
    if (dec == 0)
    {
        char *pZero = calloc(1, sizeof(char));
        pZero[0] = '0';
        size_t zero_size = 1;
        Vector_push_back_char(&pZero, '_', zero_size);
        zero_size++;
        if (base < 10)
        {
            Vector_push_back_char(&pZero, base + '0', zero_size);
            zero_size++;
        }
        else
        {
            Vector_push_back_char(&pZero, '1', zero_size);
            zero_size++;
            Vector_push_back_char(&pZero, (base % 10) + '0', zero_size);
            zero_size++;
        }
        Vector_push_back_char(&pZero, '\0', zero_size);
        return pZero;
    }
    if (dec < 0)
    {
        neg = 1;
    }
    char *pResult = Vector_create_char(0);
    size_t result_size = 0;
    while (dec != 0)
    {
        int32_t remain = dec % base;
        if (remain < 0)
            remain = -remain;
        if (remain < 10)
        {
            Vector_push_back_char(&pResult, remain + '0', result_size);
            result_size++;
        }
        else
        {
            Vector_push_back_char(&pResult, remain - 10 + 'A', result_size);
            result_size++;
        }
        dec = dec / base;
    }
    if (neg)
    {
        Vector_push_back_char(&pResult, '-', result_size);
        result_size++;
    }
    for (size_t i = 0; i < result_size / 2; i++)
    {
        char temp = pResult[i];
        pResult[i] = pResult[result_size - i - 1];
        pResult[result_size - i - 1] = temp;
    }
    Vector_push_back_char(&pResult, '_', result_size);
    result_size++;
    if (base < 10)
    {
        Vector_push_back_char(&pResult, base + '0', result_size);
        result_size++;
    }
    else
    {
        Vector_push_back_char(&pResult, '1', result_size);
        result_size++;
        Vector_push_back_char(&pResult, (base % 10) + '0', result_size);
        result_size++;
    }
    Vector_push_back_char(&pResult, '\0', result_size);
    return pResult;
}

int32_t calculate(char *pExpr, int32_t base, char **ppResult)
{
    if (pExpr == NULL || ppResult == NULL)
        return -1;
    if (base < 2 || base > 16)
        return -1;
    int64_t *pVector_num = NULL;
    int64_t *pVector_op = NULL;
    char **pVector_ptr = NULL;
    char *pString = calloc(strlen(pExpr) + 1, sizeof(char));
    for (size_t i = 0; i < strlen(pExpr); i++)
    {
        pString[i] = pExpr[i];
    }
    if (is_invalid(pString) == -1)
        goto err_free;
    pVector_ptr = (char **)Vector_create_ptr(0);
    pVector_num = Vector_create(0);
    pVector_op = Vector_create(0);
    char *pToken = strtok(pString, " ");
    size_t Vector_num_size = 0;
    size_t Vector_op_size = 0;
    size_t Vector_ptr_size = 0;
    while (pToken != NULL)
    {
        Vector_push_back_ptr((void ***)&pVector_ptr, pToken, Vector_ptr_size);
        Vector_ptr_size++;
        pToken = strtok(NULL, " ");
    }
    for (size_t i = 0; i < Vector_ptr_size; i++)
    {
        if (pVector_ptr[i][0] == '+' || pVector_ptr[i][0] == '-' || pVector_ptr[i][0] == '*')
        {
            Vector_push_back(&pVector_op, pVector_ptr[i][0], Vector_op_size);
            Vector_op_size++;
            if (pVector_num == 0)
            {
                goto err_free;
            }
        }
        else
        {
            char *pNum = pVector_ptr[i];
            int64_t count__ = 0;
            for (size_t j = 0; j < strlen(pNum); j++)
            {

                if (pNum[j] == '_')
                {
                    if (j == 0 || j == strlen(pNum) - 1)
                        goto err_free;
                    count__++;
                }
            }
            if (count__ != 1)
                goto err_free;
            char *pBase = strtok(pNum, "_");
            if (pBase == NULL)
                goto err_free;
            pBase = strtok(NULL, "_");
            if (pBase == NULL)
                goto err_free;

            char *pRemain = NULL;
            int64_t inside_base = strtol(pBase, &pRemain, 10);
            if (*pRemain != '\0')
                goto err_free;
            if (inside_base < 2 || inside_base > 16)
                goto err_free;
            pRemain = NULL;
            int64_t inside_num = strtol(pNum, &pRemain, inside_base);
            if (*pRemain != '\0')
                goto err_free;
            Vector_push_back(&pVector_num, inside_num, Vector_num_size);
            Vector_num_size++;
        }
    }
    if (Vector_op_size == 0)
    {
        Vector_free(pVector_op);
        pVector_op = NULL;
    }
    // for (size_t i = 0; i < Vector_num_size; i++)
    // {
    //     fptf(stderr, "%ld\n", pVector_num[i]);
    // }
    for (size_t i = 0; i < Vector_op_size; i++)
    {
        if (pVector_op[i] == '*')
        {
            pVector_num[i] = pVector_num[i] * pVector_num[i + 1];
            Vector_erase(&pVector_num, i + 1, Vector_num_size);
            Vector_erase(&pVector_op, i, Vector_op_size);
            Vector_num_size--;
            Vector_op_size--;
            i--;
        }
    }
    for (size_t i = 0; i < Vector_op_size; i++)
    {
        if (pVector_op[i] == '+')
        {
            pVector_num[i] = pVector_num[i] + pVector_num[i + 1];
            Vector_erase(&pVector_num, i + 1, Vector_num_size);
            Vector_erase(&pVector_op, i, Vector_op_size);
            Vector_num_size--;
            Vector_op_size--;
            i--;
        }
        else if (pVector_op[i] == '-')
        {
            pVector_num[i] = pVector_num[i] - pVector_num[i + 1];
            Vector_erase(&pVector_num, i + 1, Vector_num_size);
            Vector_erase(&pVector_op, i, Vector_op_size);
            Vector_num_size--;
            Vector_op_size--;
            i--;
        }
    }
    if (Vector_num_size != 1 || Vector_op_size != 0)
        goto err_free;
    *ppResult = to_base(pVector_num[0], base);
    if (pVector_num != NULL)
        Vector_free(pVector_num);
    if (pVector_op != NULL && Vector_op_size != 0)
        Vector_free(pVector_op);
    if (pVector_ptr != NULL)
        Vector_free_ptr((void **)pVector_ptr);
    if (pString != NULL)
        free(pString);
    // fptf(stderr, "end\n");
    return 0;
err_free:
    if (pVector_num != NULL)
        Vector_free(pVector_num);
    if (pVector_op != NULL)
        Vector_free(pVector_op);
    if (pVector_ptr != NULL)
        Vector_free_ptr((void **)pVector_ptr);
    if (pString != NULL)
        free(pString);
    return -1;
}