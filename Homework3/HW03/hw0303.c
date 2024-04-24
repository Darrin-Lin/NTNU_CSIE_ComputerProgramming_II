#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <getopt.h>
#include "mybmp.h"

int main(int argc, char *argv[])
{
    struct option longopts[] = {
        {"write", no_argument, NULL, 'w'},
        {"extract", no_argument, NULL, 'e'},
        {"bits", required_argument, NULL, 'b'},
        {0, 0, 0, 0}};
    int32_t opt = 0;
    int8_t w = 0, e = 0;
    int32_t inp_bits = 1;
    while ((opt = getopt_long(argc, argv, "web:", longopts, NULL)) != -1)
    {
        switch (opt)
        {
        case 'w':
            if (e)
            {
                printf("Error: -w and -e cannot be used together\n");
                goto err_case;
            }
            w = 1;
            break;
        case 'e':
            if (w)
            {
                printf("Error: -w and -e cannot be used together\n");
                goto err_case;
            }
            e = 1;
            break;
        case 'b':
            char *tmp;
            inp_bits = strtol(optarg, &tmp, strlen(optarg));
            if (*tmp != '\0' || (inp_bits > 8 || inp_bits < 1))
            {
                printf("Error: -b must be a number between 1 and 8\n");
                goto err_case;
            }
            break;
        default:
            break;
        }
    }
    uint8_t bits = (uint8_t)inp_bits;
    uint8_t shift = 0;
    for (int32_t i = 0; i < bits; i++)
    {
        shift = shift << 1;
        shift = shift | 1;
    }
    shift = ~shift;

    if (w)
    {
        FILE *cover_bmp = NULL;
        cover_bmp = fopen(argv[argc - 2], "rb");
        if (cover_bmp == NULL)
        {
            printf("Error: %s not found\n", argv[argc - 2]);
            goto err_case;
        }
        FILE *secret_data = NULL;
        secret_data = fopen(argv[argc - 1], "rb");
        if (secret_data == NULL)
        {
            printf("Error: %s not found\n", argv[argc - 1]);
            goto err_case;
        }
        sBmpHeader bmp_header;
        bmp_header = read_header(cover_bmp);
        struct stat st;
        fstat(fileno(secret_data), &st);
        if (st.st_size * 8 > bmp_header.width * bmp_header.height * 3 * bits)
        {
            printf("Error: %s is too large to be hidden in %s\n", argv[argc - 1], argv[argc - 2]);
            goto err_case;
        }
        FILE *tmp = fopen("tmp.bmp", "wb");
        if (tmp == NULL)
        {
            printf("Error: tmp cannot be created\n");
            goto err_case;
        }
        bmp_header.reserve = st.st_size;
        int32_t size = st.st_size;
        fwrite(&bmp_header, sizeof(sBmpHeader), 1, tmp);
        int32_t end_pixel = count_end_byte(bmp_header.width, bmp_header.bpp);
        uint8_t tmp_byte = 0;
        uint8_t tmp_len = 8;
        for (int32_t i = 0; i < bmp_header.height; i++)
        {
            for (int32_t j = 0; j < bmp_header.width; j++)
            {
                sBmpPixel24 pixel = read_pixel(cover_bmp, bmp_header, i, j, end_pixel);
                if (size > 0)
                {
                    if (tmp_len >= bits)
                    {
                        pixel.b &= shift;
                        pixel.b |= (tmp_byte >> (tmp_len - bits));
                        tmp_byte = tmp_byte << bits;
                        tmp_len -= bits;
                        if (tmp_len == 0)
                        {
                            fread(&tmp_byte, sizeof(uint8_t), 1, secret_data);
                            size--;
                            tmp_len = 8;
                        }
                    }
                    else
                    {
                        pixel.b &= shift;
                        pixel.b |= (tmp_byte >> (tmp_len - bits));
                        tmp_byte = tmp_byte << bits;
                        tmp_len -= bits;

                        fread(&tmp_byte, sizeof(uint8_t), 1, secret_data);
                        pixel.b |= (tmp_byte >> 8 + (tmp_len));
                        size--;
                        tmp_len += 8;
                    }
                }
                else if (tmp_len > 0)
                {
                    pixel.b &= shift;
                    pixel.b |= (tmp_byte >> (tmp_len - bits));
                    tmp_byte = tmp_byte << bits;
                    tmp_len -= bits;
                }
                if (size > 0)
                {
                    if (tmp_len >= bits)
                    {
                        pixel.g &= shift;
                        pixel.g |= (tmp_byte >> (tmp_len - bits));
                        tmp_byte = tmp_byte << bits;
                        tmp_len -= bits;
                        if (tmp_len == 0)
                        {
                            fread(&tmp_byte, sizeof(uint8_t), 1, secret_data);
                            size--;
                            tmp_len = 8;
                        }
                    }
                    else
                    {
                        pixel.g &= shift;
                        pixel.g |= (tmp_byte >> (tmp_len - bits));
                        tmp_byte = tmp_byte << bits;
                        tmp_len -= bits;

                        fread(&tmp_byte, sizeof(uint8_t), 1, secret_data);
                        pixel.g |= (tmp_byte >> 8 + (tmp_len));
                        size--;
                        tmp_len += 8;
                    }
                }
                else if (tmp_len > 0)
                {
                    pixel.g &= shift;
                    pixel.g |= (tmp_byte >> (tmp_len - bits));
                    tmp_byte = tmp_byte << bits;
                    tmp_len -= bits;
                }
                if (size > 0)
                {
                    if (tmp_len >= bits)
                    {
                        pixel.r &= shift;
                        pixel.r |= (tmp_byte >> (tmp_len - bits));
                        tmp_byte = tmp_byte << bits;
                        tmp_len -= bits;
                        if (tmp_len == 0)
                        {
                            fread(&tmp_byte, sizeof(uint8_t), 1, secret_data);
                            size--;
                            tmp_len = 8;
                        }
                    }
                    else
                    {
                        pixel.r &= shift;
                        pixel.r |= (tmp_byte >> (tmp_len - bits));
                        tmp_byte = tmp_byte << bits;
                        tmp_len -= bits;

                        fread(&tmp_byte, sizeof(uint8_t), 1, secret_data);
                        pixel.r |= (tmp_byte >> 8 + (tmp_len));
                        size--;
                        tmp_len += 8;
                    }
                }
                else if (tmp_len > 0)
                {
                    pixel.r &= shift;
                    pixel.r |= (tmp_byte >> (tmp_len - bits));
                    tmp_byte = tmp_byte << bits;
                    tmp_len -= bits;
                }
                fwrite(&pixel, sizeof(sBmpPixel24), 1, tmp);
            }
            write_edge_pixel(bmp_header.width, tmp);
        }
        fclose(tmp);
        fclose(cover_bmp);
    }
    else if (e)
    {
        FILE *cover_bmp = NULL;
        cover_bmp = fopen(argv[argc - 2], "rb");
        if (cover_bmp == NULL)
        {
            printf("Error: %s not found\n", argv[argc - 2]);
            goto err_case;
        }
        sBmpHeader bmp_header;
        bmp_header = read_header(cover_bmp);
        FILE *secret_data_out = fopen(argv[argc - 1], "wb");
        if (secret_data_out == NULL)
        {
            printf("Error: secret_data cannot be created\n");
            goto err_case;
        }
        int32_t size = 0;
        size = bmp_header.reserve;
        uint8_t tmp_byte = 0;
        uint8_t tmp_len = 8;
        for (int32_t i = 0; i < bmp_header.height; i++)
        {
            for (int32_t j = 0; j < bmp_header.width; j++)
            {
                sBmpPixel24 pixel = read_pixel(cover_bmp, bmp_header, i, j, count_end_byte(bmp_header.width, bmp_header.bpp));
                if (size > 0)
                {
                    if (tmp_len + bits <= 8)
                    {
                        tmp_byte = tmp_byte << bits;
                        tmp_byte |= pixel.b & ~shift;
                        tmp_len -= bits;
                        if (tmp_len == 0)
                        {
                            fwrite(&tmp_byte, sizeof(uint8_t), 1, secret_data_out);
                            tmp_byte = 0;
                            tmp_len = 8;
                            size--;
                        }
                    }
                    else
                    {
                        tmp_byte = tmp_byte << bits - (8 - tmp_len);
                        uint8_t tmp_shift = ~shift;
                        for (int32_t k = 0; k < bits - 8 - tmp_len; k++)
                        {
                            tmp_shift |= 1 << k;
                        }
                        tmp_byte |= (pixel.b & tmp_shift) >> bits - (8 - tmp_len);
                        tmp_len -= bits;
                        fwrite(&tmp_byte, sizeof(uint8_t), 1, secret_data_out);
                        tmp_byte = 0;
                        tmp_len += 8;
                        size--;
                    }
                }
                else if (tmp_len > 0)
                {
                    tmp_byte = tmp_byte << bits;
                    tmp_byte |= pixel.b & ~shift;
                    tmp_len -= bits;
                    if (tmp_len == 0)
                    {
                        fwrite(&tmp_byte, sizeof(uint8_t), 1, secret_data_out);
                        tmp_byte = 0;
                        tmp_len = 8;
                        size--;
                    }
                }
                if (size > 0)
                {
                    if (tmp_len + bits <= 8)
                    {
                        tmp_byte = tmp_byte << bits;
                        tmp_byte |= pixel.g & ~shift;
                        tmp_len -= bits;
                        if (tmp_len == 0)
                        {
                            fwrite(&tmp_byte, sizeof(uint8_t), 1, secret_data_out);
                            tmp_byte = 0;
                            tmp_len = 8;
                            size--;
                        }
                    }
                    else
                    {
                        tmp_byte = tmp_byte << bits - (8 - tmp_len);
                        uint8_t tmp_shift = ~shift;
                        for (int32_t k = 0; k < bits - 8 - tmp_len; k++)
                        {
                            tmp_shift |= 1 << k;
                        }
                        tmp_byte |= (pixel.g & tmp_shift) >> bits - (8 - tmp_len);
                        tmp_len -= bits;
                        fwrite(&tmp_byte, sizeof(uint8_t), 1, secret_data_out);
                        tmp_byte = 0;
                        tmp_len += 8;
                        size--;
                    }
                }
                else if (tmp_len > 0)
                {
                    tmp_byte = tmp_byte << bits;
                    tmp_byte |= pixel.g & ~shift;
                    tmp_len -= bits;
                    if (tmp_len == 0)
                    {
                        fwrite(&tmp_byte, sizeof(uint8_t), 1, secret_data_out);
                        tmp_byte = 0;
                        tmp_len = 8;
                        size--;
                    }
                }
                if (size > 0)
                {
                    if (tmp_len + bits <= 8)
                    {
                        tmp_byte = tmp_byte << bits;
                        tmp_byte |= pixel.r & ~shift;
                        tmp_len -= bits;
                        if (tmp_len == 0)
                        {
                            fwrite(&tmp_byte, sizeof(uint8_t), 1, secret_data_out);
                            tmp_byte = 0;
                            tmp_len = 8;
                            size--;
                        }
                    }
                    else
                    {
                        tmp_byte = tmp_byte << bits - (8 - tmp_len);
                        uint8_t tmp_shift = ~shift;
                        for (int32_t k = 0; k < bits - 8 - tmp_len; k++)
                        {
                            tmp_shift |= 1 << k;
                        }
                        tmp_byte |= (pixel.r & tmp_shift) >> bits - (8 - tmp_len);
                        tmp_len -= bits;
                        fwrite(&tmp_byte, sizeof(uint8_t), 1, secret_data_out);
                        tmp_byte = 0;
                        tmp_len += 8;
                        size--;
                    }
                }
                else if (tmp_len > 0)
                {
                    tmp_byte = tmp_byte << bits;
                    tmp_byte |= pixel.r & ~shift;
                    tmp_len -= bits;
                    if (tmp_len == 0)
                    {
                        fwrite(&tmp_byte, sizeof(uint8_t), 1, secret_data_out);
                        tmp_byte = 0;
                        tmp_len = 8;
                        size--;
                    }
                }
            }
        }
    }
    else
    {
        printf("Error: -w or -e must be used\n");
        goto err_case;
    }
    return 0;
err_case:
    return -1;
}