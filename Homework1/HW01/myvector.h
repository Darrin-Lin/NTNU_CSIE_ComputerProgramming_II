#include <stdint.h>
#include <stdlib.h>

#pragma once

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