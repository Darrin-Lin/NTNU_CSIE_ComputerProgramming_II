#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// #include "gguf.h"

#define DEBUG 1
#define fptf fprintf

enum gguf_metadata_value_type // uin32_t
{
    // The value is a 8-bit unsigned integer.
    GGUF_METADATA_VALUE_TYPE_UINT8 = 0,
    // The value is a 8-bit signed integer.
    GGUF_METADATA_VALUE_TYPE_INT8 = 1,
    // The value is a 16-bit unsigned little -endian integer.
    GGUF_METADATA_VALUE_TYPE_UINT16 = 2,
    // The value is a 16-bit signed little -endian integer.
    GGUF_METADATA_VALUE_TYPE_INT16 = 3,
    // The value is a 32-bit unsigned little -endian integer.
    GGUF_METADATA_VALUE_TYPE_UINT32 = 4,
    // The value is a 32-bit signed little -endian integer.
    GGUF_METADATA_VALUE_TYPE_INT32 = 5,
    // The value is a 32-bit IEEE754 floating point number.
    GGUF_METADATA_VALUE_TYPE_FLOAT32 = 6,
    // The value is a boolean.
    // 1-byte value where 0 is false and 1 is true.
    // Anything else is invalid , and should be treated as either the model being invalid or the reader being buggy.
    GGUF_METADATA_VALUE_TYPE_BOOL = 7,
    // The value is a UTF -8 non-null -terminated string , with length prepended.
    GGUF_METADATA_VALUE_TYPE_STRING = 8,
    // The value is an array of other values , with the length and type prepended.
    ///
    // Arrays can be nested , and the length of the array is the number of elements in the array , not the number of bytes.
    GGUF_METADATA_VALUE_TYPE_ARRAY = 9,
    // The value is a 64-bit unsigned little -endian integer.
    GGUF_METADATA_VALUE_TYPE_UINT64 = 10,
    // The value is a 64-bit signed little -endian integer.
    GGUF_METADATA_VALUE_TYPE_INT64 = 11,
    // The value is a 64-bit IEEE754 floating point number.
    GGUF_METADATA_VALUE_TYPE_FLOAT64 = 12,
};
typedef struct _Sgguf_header
{
    uint32_t magic_number;
    uint32_t version;
    uint64_t tenson_count;
    uint64_t metadata_kv_count;

} __attribute__((__packed__)) Sgguf_header;

static int8_t get_string(FILE *file, char str[]);
static uint8_t get_value(FILE *file, uint32_t type);

int main()
{
    FILE *gguf_read = fopen("model.gguf", "rb");
    if (gguf_read == NULL)
    {
        return -1;
    }
    Sgguf_header gguf_header;
    uint32_t type = 0;
    fread(&gguf_header, sizeof(Sgguf_header), 1, gguf_read);
    printf("GGUF: ");
    if (gguf_header.magic_number == 0x46554747)
        printf("true\n");
    else
    {
        printf("false\n");
        fclose(gguf_read);
        return -1;
    }

    printf("Metadata                                Value\n");
    printf("Version:                                %u\n", gguf_header.version);
    printf("tensor_count:                           %lu\n", gguf_header.tenson_count);
    printf("kv_count:                               %lu\n", gguf_header.metadata_kv_count);

    for (uint64_t i = 0; i < gguf_header.metadata_kv_count; i++)
    {
        char title[100] = {0};
        get_string(gguf_read, title);
        printf("%s:", title);
        for (size_t i = 0; i < 39 - strlen(title); i++)
        {
            printf(" ");
        }
        fread(&type, sizeof(uint32_t), 1, gguf_read);
        get_value(gguf_read, type);
        printf("\n");
    }
    printf("\n");
    printf("Tensors                                 Shape            Precision\n");
    for (uint64_t i = 0; i < gguf_header.tenson_count; i++)
    {
        char name[200] = {0};
        // get_string(gguf_read, name);
        // printf("%s:", name);
        // printf("\n");
    }
    fclose(gguf_read);

    return 0;
}
static int8_t get_string(FILE *file, char str[])
{
    uint64_t string_len = 0;
    fread(&string_len, sizeof(uint64_t), 1, file);
    if (string_len == 0)
    {
        str[0] = '\0';
        return 0;
    }
    fread(str, sizeof(char), string_len, file);
    str[strlen(str)] = '\0';
    return -1;
}
static uint8_t get_value(FILE *file, uint32_t type)
{
    switch (type)
    {
    case GGUF_METADATA_VALUE_TYPE_UINT8:
        uint8_t value = 0;
        fread(&value, sizeof(uint8_t), 1, file);
        printf("%d", value);
        break;
    case GGUF_METADATA_VALUE_TYPE_INT8:
        int8_t value1 = 0;
        fread(&value1, sizeof(int8_t), 1, file);
        printf("%d", value1);
        break;
    case GGUF_METADATA_VALUE_TYPE_UINT16:
        uint16_t value2 = 0;
        fread(&value2, sizeof(uint16_t), 1, file);
        printf("%d", value2);
        break;
    case GGUF_METADATA_VALUE_TYPE_INT16:
        int16_t value3 = 0;
        fread(&value3, sizeof(int16_t), 1, file);
        printf("%d", value3);
        break;
    case GGUF_METADATA_VALUE_TYPE_UINT32:
        uint32_t value4 = 0;
        fread(&value4, sizeof(uint32_t), 1, file);
        printf("%d", value4);
        break;
    case GGUF_METADATA_VALUE_TYPE_INT32:
        int32_t value5 = 0;
        fread(&value5, sizeof(int32_t), 1, file);
        printf("%d", value5);
        break;
    case GGUF_METADATA_VALUE_TYPE_FLOAT32:
        float value6 = 0;
        fread(&value6, sizeof(float), 1, file);
        printf("%f", value6);
        break;
    case GGUF_METADATA_VALUE_TYPE_BOOL:
        uint8_t value7 = 0;
        fread(&value7, sizeof(uint8_t), 1, file);
        if (value7 == 0)
            printf("false");
        // else if (value7 == 1)
        else
            printf("true");
        // else
        //     printf("invalid");
        break;
    case GGUF_METADATA_VALUE_TYPE_STRING:
        char value8[1000] = {0};
        get_string(file, value8);
        printf("%s", value8);
        break;
    case GGUF_METADATA_VALUE_TYPE_ARRAY:
        uint32_t array_type = 0;
        fread(&array_type, sizeof(uint32_t), 1, file);
        uint64_t array_len = 0;
        fread(&array_len, sizeof(uint64_t), 1, file);
        printf("[");
        for (uint64_t i = 0; i < array_len; i++)
        {
            get_value(file, array_type);
            if (i != array_len - 1)
                printf(", ");
        }
        printf("]");
        break;
    case GGUF_METADATA_VALUE_TYPE_UINT64:
        uint64_t value9 = 0;
        fread(&value9, sizeof(uint64_t), 1, file);
        printf("%lu", value9);
        break;
    case GGUF_METADATA_VALUE_TYPE_INT64:
        int64_t value10 = 0;
        fread(&value10, sizeof(int64_t), 1, file);
        printf("%ld", value10);
        break;
    case GGUF_METADATA_VALUE_TYPE_FLOAT64:
        double value11 = 0;
        fread(&value11, sizeof(double), 1, file);
        printf("%f", value11);
        break;
    default:
        break;
    }
    return 0;
}