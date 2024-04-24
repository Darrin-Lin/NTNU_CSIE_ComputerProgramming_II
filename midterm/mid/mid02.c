#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char file_name[5000];
    printf("Codebook: ");
    fgets(file_name, 5000, stdin);
    file_name[strlen(file_name) - 1] = '\0';
    FILE *codebook = fopen(file_name, "r");
    if (codebook == NULL)
    {
        printf("No file.\n");
        return -1;
    }
    printf("Input File: ");
    fgets(file_name, 5000, stdin);
    file_name[strlen(file_name) - 1] = '\0';
    FILE *input = fopen(file_name, "rb");
    if (input == NULL)
    {
        printf("No file.\n");
        return -1;
    }
    printf("Output File: ");
    fgets(file_name, 5000, stdin);
    file_name[strlen(file_name) - 1] = '\0';
    FILE *output = fopen(file_name, "wb");
    if (output == NULL)
    {
        printf("No file.\n");
        return -1;
    }

    int32_t encode[29];
    int32_t encode_len[29] = {0};
    for (int32_t i = 0; i < 29; i++)
    {
        encode[i] = -1;
    }
    char code[100];
    while (!feof(codebook))
    {
        fgets(code, 100, codebook);
        if (code[0] == '\n')
        {
            continue;
            ;
        }
        if (code[1] == ':')
        {
            if (code[0] >= 'a' && code[0] <= 'z')
            {
                encode[code[0] - 'a'] = strtol(code + 3, NULL, 2);
                encode_len[code[0] - 'a'] = strlen(code + 3) - 1;
            }
            else
            {
                printf("Invalid codebook.\n");
                goto terminate;
            }
        }
        else
        {
            if (strstr(code, "space:"))
            {
                encode[26] = strtol(strchr(code, ':') + 2, NULL, 2);
                encode_len[26] = strlen(strchr(code, ':') + 2) - 1;
            }
            else if (strstr(code, "comma:"))
            {
                encode[27] = strtol(strchr(code, ':') + 2, NULL, 2);
                encode_len[27] = strlen(strchr(code, ':') + 2) - 1;
            }
            else if (strstr(code, "period:"))
            {
                encode[28] = strtol(strchr(code, ':') + 2, NULL, 2);
                encode_len[28] = strlen(strchr(code, ':') + 2) - 1;
            }
            else
            {
                printf("Invalid codebook.\n");
                goto terminate;
            }
        }
    }
    fclose(codebook);
    int8_t temp = 0;
    int8_t temp_len = 0;
    while (!feof(input))
    {

        char c = fgetc(input);
        if (c == EOF)
        {
            break;
        }
        int32_t code_now = 0;
        int32_t code_now_len = 0;
        if (c >= 'a' && c <= 'z')
        {
            code_now = encode[c - 'a'];
            code_now_len = encode_len[c - 'a'];
        }
        else if (c == ' ')
        {
            code_now = encode[26];
            code_now_len = encode_len[26];
        }
        else if (c == ',')
        {
            code_now = encode[27];
            code_now_len = encode_len[27];
        }
        else if (c == '.')
        {
            code_now = encode[28];
            code_now_len = encode_len[28];
        }
        else
        {
            printf("Invalid input.\n");
            goto terminate;
        }
        if (code_now == -1)
        {
            printf("Invalid codebook.\n");
            goto terminate;
        }
        // fprintf(stderr, "*%d %d\n", code_now, code_now_len);
        while (temp_len + code_now_len > 8)
        {
            temp = (temp << (8 - temp_len)) | (code_now >> (code_now_len - (8 - temp_len)));
            // fprintf(stderr, "%d ", temp);
            fwrite(&temp, 1, 1, output);
            code_now = code_now & ((1 << (code_now_len - (8 - temp_len))) - 1);
            code_now_len = code_now_len - (8 - temp_len);
            temp = 0;
            temp_len = 0;
        }
        temp = temp << code_now_len | code_now;
        temp_len += code_now_len;
        // fprintf(stderr, "(%d %d\n", temp, temp_len);
        if (temp_len == 8)
        {
            // fprintf(stderr, "a%d ", temp);
            fwrite(&temp, 1, 1, output);
            temp = 0;
            temp_len = 0;
        }
    }

    if (temp_len != 0)
    {
        temp = temp << (8 - temp_len);
        temp += 1 << (8 - temp_len - 1);
        fwrite(&temp, 1, 1, output);
    }
    else
    {
        temp = 0b10000000;
        fwrite(&temp, 1, 1, output);
    }
    fclose(input);
    fclose(output);
    return 0;
terminate:
    fclose(codebook);
    fclose(input);
    fclose(output);
    return -1;
}