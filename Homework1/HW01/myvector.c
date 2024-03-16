#include <stdint.h>
#include <stdlib.h>

int64_t *Vector_create(size_t size);
void Vector_free(int64_t *pVector);
int8_t Vector_push_back(int64_t **pVector, int64_t value, size_t size);
int8_t Vector_pop_back(int64_t **pVector, size_t size);
int8_t Vector_insert(int64_t **pVector, int64_t value, size_t index, size_t size);
int8_t Vector_erase(int64_t **pVector, size_t index, size_t size);
int8_t Vector_resize(int64_t **pVector, size_t size);

double *Vector_create_double(size_t size);
void Vector_free_double(double *pVector);
int8_t Vector_push_back_double(double **pVector, double value, size_t size);
int8_t Vector_pop_back_double(double **pVector, size_t size);
int8_t Vector_insert_double(double **pVector, double value, size_t index, size_t size);
int8_t Vector_erase_double(double **pVector, size_t index, size_t size);
int8_t Vector_resize_double(double **pVector, size_t size);

void **Vector_create_ptr(size_t size);
void Vector_free_ptr(void **pVector);
int8_t Vector_push_back_ptr(void ***pVector, void *value, size_t size);
int8_t Vector_pop_back_ptr(void ***pVector, size_t size);
int8_t Vector_insert_ptr(void ***pVector, void *value, size_t index, size_t size);
int8_t Vector_erase_ptr(void ***pVector, size_t index, size_t size);
int8_t Vector_resize_ptr(void ***pVector, size_t size);

char *Vector_create_char(size_t size);
void Vector_free_char(char *pVector);
int8_t Vector_push_back_char(char **pVector, char value, size_t size);
int8_t Vector_pop_back_char(char **pVector, size_t size);
int8_t Vector_insert_char(char **pVector, char value, size_t index, size_t size);
int8_t Vector_erase_char(char **pVector, size_t index, size_t size);
int8_t Vector_resize_char(char **pVector, size_t size);

int64_t *Vector_create(size_t size)
{
    int64_t *pVector = (int64_t *)calloc(size, sizeof(int64_t));
    if (pVector == NULL)
        return NULL;
    return pVector;
}
void Vector_free(int64_t *pVector)
{
    free(pVector);
}
int8_t Vector_push_back(int64_t **pVector, int64_t value, size_t size)
{
    int64_t *pNew = (int64_t *)realloc(*pVector, sizeof(int64_t) * (size + 1));
    if (pNew == NULL)
        return -1;
    *pVector = pNew;
    (*pVector)[size] = value;
    return 0;
}
int8_t Vector_pop_back(int64_t **pVector, size_t size)
{
    int64_t *pNew = (int64_t *)realloc(*pVector, sizeof(int64_t) * (size - 1));
    if (pNew == NULL)
        return -1;
    *pVector = pNew;
    return 0;
}
int8_t Vector_insert(int64_t **pVector, int64_t value, size_t index, size_t size)
{
    int64_t *pNew = (int64_t *)realloc(*pVector, sizeof(int64_t) * (size + 1));
    if (pNew == NULL)
        return -1;
    *pVector = pNew;
    for (int64_t i = size; i > index; i--)
    {
        (*pVector)[i] = (*pVector)[i - 1];
    }
    (*pVector)[index] = value;
    return 0;
}
int8_t Vector_erase(int64_t **pVector, size_t index, size_t size)
{
    for (int64_t i = index; i < size; i++)
    {
        (*pVector)[i] = (*pVector)[i + 1];
    }
    int64_t *pNew = (int64_t *)realloc(*pVector, sizeof(int64_t) * (size - 1));
    if (pNew == NULL)
        return -1;
    *pVector = pNew;
    return 0;
}
int8_t Vector_resize(int64_t **pVector, size_t size)
{
    int64_t *pNew = (int64_t *)realloc(*pVector, sizeof(int64_t) * size);
    if (pNew == NULL)
        return -1;
    *pVector = pNew;
    return 0;
}

double *Vector_create_double(size_t size)
{
    double *pVector = (double *)calloc(size, sizeof(double));
    if (pVector == NULL)
        return NULL;
    return pVector;
}
void Vector_free_double(double *pVector)
{
    free(pVector);
}
int8_t Vector_push_back_double(double **pVector, double value, size_t size)
{
    double *pNew = (double *)realloc(*pVector, sizeof(double) * (size + 1));
    if (pNew == NULL)
        return -1;
    *pVector = pNew;
    (*pVector)[size] = value;
    return 0;
}

int8_t Vector_pop_back_double(double **pVector, size_t size)
{
    double *pNew = (double *)realloc(*pVector, sizeof(double) * (size - 1));
    if (pNew == NULL)
        return -1;
    *pVector = pNew;
    return 0;
}
int8_t Vector_insert_double(double **pVector, double value, size_t index, size_t size)
{
    double *pNew = (double *)realloc(*pVector, sizeof(double) * (size + 1));
    if (pNew == NULL)
        return -1;
    *pVector = pNew;
    for (int64_t i = size; i > index; i--)
    {
        (*pVector)[i] = (*pVector)[i - 1];
    }
    (*pVector)[index] = value;
    return 0;
}
int8_t Vector_erase_double(double **pVector, size_t index, size_t size)
{
    for (int64_t i = index; i < size; i++)
    {
        (*pVector)[i] = (*pVector)[i + 1];
    }
    double *pNew = (double *)realloc(*pVector, sizeof(double) * (size - 1));
    if (pNew == NULL)
        return -1;
    *pVector = pNew;
    return 0;
}
int8_t Vector_resize_double(double **pVector, size_t size)
{
    double *pNew = (double *)realloc(*pVector, sizeof(double) * size);
    if (pNew == NULL)
        return -1;
    *pVector = pNew;
    return 0;
}

void **Vector_create_ptr(size_t size)
{
    void **pVector = (void **)calloc(size, sizeof(void *));
    if (pVector == NULL)
        return NULL;
    return pVector;
}
void Vector_free_ptr(void **pVector)
{
    free(pVector);
}
int8_t Vector_push_back_ptr(void ***pVector, void *value, size_t size)
{
    void **pNew = (void **)realloc(*pVector, sizeof(void *) * (size + 1));
    if (pNew == NULL)
        return -1;
    *pVector = pNew;
    (*pVector)[size] = value;
    return 0;
}
int8_t Vector_pop_back_ptr(void ***pVector, size_t size)
{
    void **pNew = (void **)realloc(*pVector, sizeof(void *) * (size - 1));
    if (pNew == NULL)
        return -1;
    *pVector = pNew;
    return 0;
}
int8_t Vector_insert_ptr(void ***pVector, void *value, size_t index, size_t size)
{
    void **pNew = (void **)realloc(*pVector, sizeof(void *) * (size + 1));
    if (pNew == NULL)
        return -1;
    *pVector = pNew;
    for (int64_t i = size; i > index; i--)
    {
        (*pVector)[i] = (*pVector)[i - 1];
    }
    (*pVector)[index] = value;
    return 0;
}
int8_t Vector_erase_ptr(void ***pVector, size_t index, size_t size)
{
    for (int64_t i = index; i < size; i++)
    {
        (*pVector)[i] = (*pVector)[i + 1];
    }
    void **pNew = (void **)realloc(*pVector, sizeof(void *) * (size - 1));
    if (pNew == NULL)
        return -1;
    *pVector = pNew;
    return 0;
}
int8_t Vector_resize_ptr(void ***pVector, size_t size)
{
    void **pNew = (void **)realloc(*pVector, sizeof(void *) * size);
    if (pNew == NULL)
        return -1;
    *pVector = pNew;
    return 0;
}

char *Vector_create_char(size_t size)
{
    char *pVector = (char *)calloc(size, sizeof(char));
    if (pVector == NULL)
        return NULL;
    return pVector;
}
void Vector_free_char(char *pVector)
{
    free(pVector);
}
int8_t Vector_push_back_char(char **pVector, char value, size_t size)
{
    char *pNew = (char *)realloc(*pVector, sizeof(char) * (size + 1));
    if (pNew == NULL)
        return -1;
    *pVector = pNew;
    (*pVector)[size] = value;
    return 0;
}
int8_t Vector_pop_back_char(char **pVector, size_t size)
{
    char *pNew = (char *)realloc(*pVector, sizeof(char) * (size - 1));
    if (pNew == NULL)
        return -1;
    *pVector = pNew;
    return 0;
}
int8_t Vector_insert_char(char **pVector, char value, size_t index, size_t size)
{
    char *pNew = (char *)realloc(*pVector, sizeof(char) * (size + 1));
    if (pNew == NULL)
        return -1;
    *pVector = pNew;
    for (int64_t i = size; i > index; i--)
    {
        (*pVector)[i] = (*pVector)[i - 1];
    }
    (*pVector)[index] = value;
    return 0;
}
int8_t Vector_erase_char(char **pVector, size_t index, size_t size)
{
    for (int64_t i = index; i < size; i++)
    {
        (*pVector)[i] = (*pVector)[i + 1];
    }
    char *pNew = (char *)realloc(*pVector, sizeof(char) * (size - 1));
    if (pNew == NULL)
        return -1;
    *pVector = pNew;
    return 0;
}
int8_t Vector_resize_char(char **pVector, size_t size)
{
    char *pNew = (char *)realloc(*pVector, sizeof(char) * size);
    if (pNew == NULL)
        return -1;
    *pVector = pNew;
    return 0;
}
