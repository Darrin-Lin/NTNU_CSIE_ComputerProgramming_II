#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// #include "gguf.h"

#define DEBUG 0
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
enum ggml_type
{
    GGML_TYPE_F32 = 0,
    GGML_TYPE_F16 = 1,
    GGML_TYPE_Q4_0 = 2,
    GGML_TYPE_Q4_1 = 3,
    // GGML_TYPE_Q4_2 = 4, support has been removed
    // GGML_TYPE_Q4_3 (5) support has been removed
    GGML_TYPE_Q5_0 = 6,
    GGML_TYPE_Q5_1 = 7,
    GGML_TYPE_Q8_0 = 8,
    GGML_TYPE_Q8_1 = 9,
    // k-quantizations
    GGML_TYPE_Q2_K = 10,
    GGML_TYPE_Q3_K = 11,
    GGML_TYPE_Q4_K = 12,
    GGML_TYPE_Q5_K = 13,
    GGML_TYPE_Q6_K = 14,
    GGML_TYPE_Q8_K = 15,
    GGML_TYPE_I8,
    GGML_TYPE_I16,
    GGML_TYPE_I32,
    GGML_TYPE_COUNT,
};
typedef struct _Sgguf_header
{
    uint32_t magic_number;
    uint32_t version;
    uint64_t tenson_count;
    uint64_t metadata_kv_count;

} __attribute__((__packed__)) Sgguf_header;

static int8_t get_string(FILE *file, char str[]);
static uint8_t get_value(FILE *file, uint32_t type, int8_t print);
static int8_t count_parameters(FILE *file, Sgguf_header gguf_header);
static int8_t sort_tensor();
static int32_t mylog10(int32_t x);

uint64_t parameter = 0;

int main()
{
    fprintf(stdout, "GGUF: ");
    FILE *gguf_read = fopen("model.gguf", "rb");
    if (gguf_read == NULL)
    {
        fprintf(stdout, "false\n");
        return -1;
    }
    Sgguf_header gguf_header;
    uint32_t type = 0;
    fread(&gguf_header, sizeof(Sgguf_header), 1, gguf_read);

    if (gguf_header.magic_number == 0x46554747)
        fprintf(stdout, "true\n");
    else
    {
        fprintf(stdout, "false\n");
        fclose(gguf_read);
        return -1;
    }
    count_parameters(gguf_read, gguf_header);
    fprintf(stdout, "Parameters: ");
    char num[100] = {0};
    sprintf(num, "%lu", parameter);
    int64_t len = strlen(num);
    for (int64_t i = 0; i < len; i++)
    {
        fprintf(stdout, "%c", num[i]);
        if ((len - i - 1) % 3 == 0 && i != len - 1)
        {
            fprintf(stdout, ",");
        }
    }
    fprintf(stdout, "\n");

    fseek(gguf_read, 0, SEEK_SET);
    fread(&gguf_header, sizeof(Sgguf_header), 1, gguf_read);
    fprintf(stdout, "\n");
    fprintf(stdout, "Metadata                                Value\n");
    fprintf(stdout, "Version:                                %u\n", gguf_header.version);
    fprintf(stdout, "tensor_count:                           %lu\n", gguf_header.tenson_count);
    fprintf(stdout, "kv_count:                               %lu\n", gguf_header.metadata_kv_count);

    for (uint64_t i = 0; i < gguf_header.metadata_kv_count; i++)
    {
        char title[100] = {0};
        get_string(gguf_read, title);
        fprintf(stdout, "%s:", title);
        for (size_t i = 0; i < 39 - strlen(title); i++)
        {
            fprintf(stdout, " ");
        }
        fread(&type, sizeof(uint32_t), 1, gguf_read);
        get_value(gguf_read, type, 1);
        fprintf(stdout, "\n");
    }
    fprintf(stdout, "\n");
    fprintf(stdout, "Tensors                                 Shape            Precision\n");
    FILE *tensor = fopen("tensor.txt", "w");
    for (uint64_t i = 0; i < gguf_header.tenson_count; i++)
    {

        char name[200] = {0};
        get_string(gguf_read, name);
        fprintf(tensor, "%s:", name);
        for (size_t i = 0; i < 39 - strlen(name); i++)
        {
            fprintf(tensor, " ");
        }
        uint32_t n_dim = 0;
        fread(&n_dim, sizeof(uint32_t), 1, gguf_read);
        fprintf(tensor, "[");
        int64_t strlen_count = 0;
        strlen_count++;
        uint64_t count = 1;
        for (uint32_t j = 0; j < n_dim; j++)
        {
            uint64_t dimensions = 0;
            fread(&dimensions, sizeof(uint64_t), 1, gguf_read);
            fprintf(tensor, "%lu", dimensions);
            char len_ct[100] = {0};
            sprintf(len_ct, "%lu", dimensions);
            count *= dimensions;
            strlen_count += strlen(len_ct);
            if (j != n_dim - 1)
            {
                fprintf(tensor, ",");
                strlen_count++;
            }
        }
        parameter += count;
        fprintf(tensor, "]");
        strlen_count++;
        for (int64_t i = 0; i < 17 - strlen_count; i++)
        {
            fprintf(tensor, " ");
        }
        uint32_t precision = 0;
        fread(&precision, sizeof(uint32_t), 1, gguf_read);
        switch (precision)
        {
        case GGML_TYPE_F32:
            fprintf(tensor, "F32");
            break;
        case GGML_TYPE_F16:
            fprintf(tensor, "F16");
            break;
        case GGML_TYPE_Q4_0:
            fprintf(tensor, "Q4_0");
            break;
        case GGML_TYPE_Q4_1:
            fprintf(tensor, "Q4_1");
            break;
        case GGML_TYPE_Q5_0:
            fprintf(tensor, "Q5_0");
            break;
        case GGML_TYPE_Q5_1:
            fprintf(tensor, "Q5_1");
            break;
        case GGML_TYPE_Q8_0:
            fprintf(tensor, "Q8_0");
            break;
        case GGML_TYPE_Q8_1:
            fprintf(tensor, "Q8_1");
            break;
        case GGML_TYPE_Q2_K:
            fprintf(tensor, "Q2_K");
            break;
        case GGML_TYPE_Q3_K:
            fprintf(tensor, "Q3_K");
            break;
        case GGML_TYPE_Q4_K:
            fprintf(tensor, "Q4_K");
            break;
        case GGML_TYPE_Q5_K:
            fprintf(tensor, "Q5_K");
            break;
        case GGML_TYPE_Q6_K:
            fprintf(tensor, "Q6_K");
            break;
        case GGML_TYPE_Q8_K:
            fprintf(tensor, "Q8_K");
            break;
        case GGML_TYPE_I8:
            fprintf(tensor, "I8");
            break;
        case GGML_TYPE_I16:
            fprintf(tensor, "I16");
            break;
        case GGML_TYPE_I32:
            fprintf(tensor, "I32");
            break;
        default:
            break;
        }
        uint64_t offset = 0;
        fread(&offset, sizeof(uint64_t), 1, gguf_read);
        // don't know what to do with offset
        fprintf(tensor, "\n");
    }
    fclose(tensor);
    sort_tensor();
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
    if (DEBUG)
    {
        fptf(stderr, "%ld\n", string_len);
    }
    uint64_t max_len = 50;
    if (string_len > max_len)
    {
        for (uint64_t i = 0; i < max_len; i++)
        {
            char char_read = 0;
            fread(&char_read, sizeof(char), 1, file);
            str[i] = char_read;
        }
        str[max_len] = '.';
        str[max_len + 1] = '.';
        str[max_len + 2] = '.';
        str[max_len + 3] = '\0';
        for (uint64_t i = max_len; i < string_len; i++)
        {
            char char_read = 0;
            fread(&char_read, sizeof(char), 1, file);
        }
    }
    else
    {
        fread(str, sizeof(char), string_len, file);
    }
    str[strlen(str)] = '\0';
    return -1;
}
static uint8_t get_value(FILE *file, uint32_t type, int8_t print)
{
    switch (type)
    {
    case GGUF_METADATA_VALUE_TYPE_UINT8:
        uint8_t value = 0;
        fread(&value, sizeof(uint8_t), 1, file);
        if (print)
            fprintf(stdout, "%d", value);
        break;
    case GGUF_METADATA_VALUE_TYPE_INT8:
        int8_t value1 = 0;
        fread(&value1, sizeof(int8_t), 1, file);
        if (print)
            fprintf(stdout, "%d", value1);
        break;
    case GGUF_METADATA_VALUE_TYPE_UINT16:
        uint16_t value2 = 0;
        fread(&value2, sizeof(uint16_t), 1, file);
        if (print)
            fprintf(stdout, "%d", value2);
        break;
    case GGUF_METADATA_VALUE_TYPE_INT16:
        int16_t value3 = 0;
        fread(&value3, sizeof(int16_t), 1, file);
        if (print)
            fprintf(stdout, "%d", value3);
        break;
    case GGUF_METADATA_VALUE_TYPE_UINT32:
        uint32_t value4 = 0;
        fread(&value4, sizeof(uint32_t), 1, file);
        if (print)
            fprintf(stdout, "%d", value4);
        break;
    case GGUF_METADATA_VALUE_TYPE_INT32:
        int32_t value5 = 0;
        fread(&value5, sizeof(int32_t), 1, file);
        if (print)
            fprintf(stdout, "%d", value5);
        break;
    case GGUF_METADATA_VALUE_TYPE_FLOAT32:
        float value6 = 0;
        fread(&value6, sizeof(float), 1, file);
        if (print)
            fprintf(stdout, "%f", value6);
        break;
    case GGUF_METADATA_VALUE_TYPE_BOOL:
        uint8_t value7 = 0;
        fread(&value7, sizeof(uint8_t), 1, file);
        if (print)
        {
            if (value7 == 0)
                fprintf(stdout, "false");
            else if (value7 == 1)
                fprintf(stdout, "true");
            else
                fprintf(stdout, "invalid");
        }
        break;
    case GGUF_METADATA_VALUE_TYPE_STRING:
        char value8[100] = {0};
        get_string(file, value8);
        if (print)
        {
            fprintf(stdout, "%s", value8);
        }

        break;
    case GGUF_METADATA_VALUE_TYPE_ARRAY:
        uint32_t array_type = 0;
        fread(&array_type, sizeof(uint32_t), 1, file);
        uint64_t array_len = 0;
        fread(&array_len, sizeof(uint64_t), 1, file);
        if (print)
            fprintf(stdout, "[");
        for (uint64_t i = 0; i < array_len; i++)
        {
            if (array_len > 3 && i >= 3)
            {
                if (i == array_len - 1)
                {
                    if (print)
                        fprintf(stdout, "...(%lu)", array_len - 3);
                }
                get_value(file, array_type, 0);
            }
            else
            {
                get_value(file, array_type, print);
                if (print)
                {
                    if (i != array_len - 1)
                        fprintf(stdout, ", ");
                }
            }
        }
        if (print)
            fprintf(stdout, "]");
        break;
    case GGUF_METADATA_VALUE_TYPE_UINT64:
        uint64_t value9 = 0;
        fread(&value9, sizeof(uint64_t), 1, file);
        if (print)
            fprintf(stdout, "%lu", value9);
        break;
    case GGUF_METADATA_VALUE_TYPE_INT64:
        int64_t value10 = 0;
        fread(&value10, sizeof(int64_t), 1, file);
        if (print)
            fprintf(stdout, "%ld", value10);
        break;
    case GGUF_METADATA_VALUE_TYPE_FLOAT64:
        double value11 = 0;
        fread(&value11, sizeof(double), 1, file);
        if (print)
            fprintf(stdout, "%f", value11);
        break;
    default:
        break;
    }
    return 0;
}
static int8_t count_parameters(FILE *file, Sgguf_header gguf_header)
{
    uint32_t type = 0;
    for (uint64_t i = 0; i < gguf_header.metadata_kv_count; i++)
    {
        char title[100] = {0};
        get_string(file, title);
        fread(&type, sizeof(uint32_t), 1, file);
        get_value(file, type, 0);
    }
    for (uint64_t i = 0; i < gguf_header.tenson_count; i++)
    {

        char name[200] = {0};
        get_string(file, name);
        uint32_t n_dim = 0;
        fread(&n_dim, sizeof(uint32_t), 1, file);
        uint64_t count = 1;
        for (uint32_t j = 0; j < n_dim; j++)
        {
            uint64_t dimensions = 0;
            fread(&dimensions, sizeof(uint64_t), 1, file);

            count *= dimensions;
        }
        parameter += count;

        uint32_t precision = 0;
        fread(&precision, sizeof(uint32_t), 1, file);

        uint64_t offset = 0;
        fread(&offset, sizeof(uint64_t), 1, file);
        // don't know what to do with offset
    }
    return 0;
}
static int8_t sort_tensor()
{

    FILE *fTensors = fopen("tensor.txt", "r");
    if (fTensors == NULL)
    {
        return -1;
    }
    FILE *f_blk_sort = fopen("blk_sort.txt", "w");
    FILE *fOther_sort = fopen("other_sort.txt", "w");
    int32_t number_max = 0;
    while (!feof(fTensors))
    {
        char tensor[200] = {0};
        fgets(tensor, 200, fTensors);
        if (strstr(tensor, "blk") != NULL) // sort by number
        {
            fprintf(f_blk_sort, "%s", tensor);
            int32_t number = 0;
            number = strtol(strchr(tensor, '.') + 1, NULL, 10);
            if (number > number_max)
            {
                number_max = number;
            }
        }
        else
        {
            fprintf(fOther_sort, "%s", tensor);
        }
    }

    fclose(fTensors);
    remove("tensor.txt");
    fclose(f_blk_sort);
    fclose(fOther_sort);
    FILE *f_blk_sort_read = fopen("blk_sort.txt", "r");
    // FILE *temp = fopen("temp.txt", "w");
    for (int32_t i = 0; i <= number_max; i++)
    {
        fprintf(stdout, "blk.%d\n", i);
        while (!feof(f_blk_sort_read))
        {
            char tensor[200] = {0};
            fgets(tensor, 200, f_blk_sort_read);
            if (strstr(tensor, ".") != NULL) // sort by number
            {
                int32_t number = 0;
                number = strtol(strchr(tensor, '.') + 1, NULL, 10);
                if (number == i)
                {
                    // add 2 spaces before [
                    char print_tensor[200] = {0};
                    strcpy(print_tensor, strchr(strchr(tensor, '.') + 1, '.'));
                    strtok(print_tensor, "[");
                    fprintf(stdout, "    %s", strtok(print_tensor, "["));
                    for (int32_t j = 0; j < mylog10(i); j++)
                    {
                        fprintf(stdout, " ");
                    }
                    fprintf(stdout, "%s", strchr(tensor, '['));
                }
            }
        }
        fseek(f_blk_sort_read, 0, SEEK_SET);
    }
    fclose(f_blk_sort_read);
    remove("blk_sort.txt");
    // sort by alphabet
    FILE *fOther_sort_read = fopen("other_sort.txt", "r");
    char words[100][200];
    while (!feof(fOther_sort_read))
    {

        int32_t count = 0;
        while (!feof(fOther_sort_read))
        {
            char line[200] = {0};
            fgets(line, 200, fOther_sort_read);
            if (line[0] == '\0')
                break;
            strcpy(words[count], line);
            count++;
        }
        for (int32_t i = 0; i < count; i++)
        {
            for (int32_t j = i + 1; j < count; j++)
            {
                int32_t len1 = strlen(words[i]);
                int32_t len2 = strlen(words[j]);
                int32_t len = len1 > len2 ? len2 : len1;
                for (int32_t k = 0; k < len; k++)
                {
                    if (words[i][k] > words[j][k])
                    {
                        char temp[200] = {0};
                        strcpy(temp, words[i]);
                        strcpy(words[i], words[j]);
                        strcpy(words[j], temp);
                        break;
                    }
                    else if (words[i][k] < words[j][k])
                    {
                        break;
                    }
                }
            }
        }
        for (int32_t i = 0; i < count; i++)
        {
            fprintf(stdout, "%s", words[i]);
        }
    }
    fclose(fOther_sort_read);
    remove("other_sort.txt");

    return 0;
}
static int32_t mylog10(int32_t x)
{
    int32_t count = 0;
    if (x == 0)
    {
        return 1;
    }
    while (x > 0)
    {
        x /= 10;
        count++;
    }
    return count;
}