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

#ifdef DEBUG
#define debug_print(fmt, ...)                     \
    fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
            __LINE__, __func__, ##__VA_ARGS__);
#else
#define debug_print(fmt, ...) \
    {                         \
    }
#endif

#define HELP_MSG "Usage: fin01 [options] file\n\
  -r, --resolution=widthxheight Setup the resolution. Default: 1024x768.\n\
  -p, --prefix=str Setup the file name prefix. Default: output.\n\
  -h, --help Display this information and exit.\n"

int main(int argc, char *argv[])
{

    int32_t width = 1024;
    int32_t height = 768;
    char prefix[1024] = "output";

    int opt;
    struct option longopts[] = {
        {"help", no_argument, NULL, 'h'},
        {"resolution", required_argument, NULL, 'r'},
        {"prefix", required_argument, NULL, 'p'},
        {0, 0, 0, 0}};
    while ((opt = getopt_long(argc, argv, "hr:p:", longopts, NULL)) != -1)
    {
        if (opt == 'h')
        {
            fprintf(stdout, HELP_MSG);
            return 0;
        }
        else if (opt == 'r')
        {
            int32_t tmp_width = -1, tmp_height = -1;
            sscanf(optarg, "%dx%d", &tmp_width, &tmp_height);
            if (tmp_width <= 0 || tmp_height <= 0)
            {
                fprintf(stderr, "Invalid resolution.\n");
                return -1;
            }
            width = tmp_width;
            height = tmp_height;
        }
        else if (opt == 'p')
        {
            strncpy(prefix, optarg, 1024);
        }
        else
        {
            fprintf(stdout, "Unknown option:\n");
            fprintf(stdout, HELP_MSG);
            return -1;
        }
    }
    if (width <= 0 || height <= 0)
    {
        fprintf(stderr, "Invalid resolution.\n");
        return -1;
    }
    if (optind >= argc)
    {
        fprintf(stderr, "No input file.\n");
        return -1;
    }
    char *filename = argv[optind];
    debug_print("Resolution: %dx%d\n", width, height);
    debug_print("Prefix: %s\n", prefix);
    debug_print("Filename: %s\n", filename);
    FILE *MBMP = fopen(filename, "rb");
    if (MBMP == NULL)
    {
        fprintf(stderr, "Cannot open file.\n");
        return -1;
    }
    uint64_t size = 0;
    int32_t count = 0;
    while (!feof(MBMP))
    {
        debug_print("Size: %d\n", size);
        if (size != 0)
        {
            int is_eof = 0;
            fseek(MBMP, size, SEEK_SET);
            if (fread(&is_eof, 1, 1, MBMP) != 1)
            {
                debug_print("EOF\n");
                break;
            }
        }
        fseek(MBMP, size, SEEK_SET);
        if (feof(MBMP))
        {
            debug_print("EOF\n");
            break;
        }
        sBmpHeader header = read_header(MBMP);
        if (header.size == 0)
        {
            debug_print("EOF\n");
            break;
        }
        size += header.size;
        debug_print("Size: %d\n", size);
        debug_print("Width: %d\n", header.width);
        count++;
        FILE *output = NULL;
        char output_filename[1024];
        snprintf(output_filename, 1024, "%s_%d.bmp", prefix, count);
        output = fopen(output_filename, "wb");
        if (output == NULL)
        {
            fprintf(stderr, "Cannot open file.\n");
            return -1;
        }
        fseek(MBMP, size - header.size, SEEK_SET);
        for (int i = 0; i < header.size; i++)
        {
            uint8_t tmp;
            fread(&tmp, 1, 1, MBMP);
            fwrite(&tmp, 1, 1, output);
            if (i >= header.size - 1)
                debug_print("%d ", i);
        }
        fclose(output);
    }
    fclose(MBMP);
    int32_t len_of_count = 0;
    char count_str[1024];
    snprintf(count_str, 1024, "%d", count);
    len_of_count = strlen(count_str);
    for (int32_t i = 1; i <= count; i++)
    {
        char result_filename[1024] = {0};
        snprintf(count_str, 1024, "%d", i);
        int32_t i_len = strlen(count_str);
        snprintf(result_filename, 1024, "%s", prefix);
        if (i_len < len_of_count)
        {
            for (int32_t j = 0; j < len_of_count - i_len; j++)
            {
                strncat(result_filename, "0", 1024);
            }
        }
        strncat(result_filename, count_str, 1024);
        strncat(result_filename, ".bmp", 1024);
        char output_filename[1024] = {0};
        snprintf(output_filename, 1024, "%s_%d.bmp", prefix, i);
        FILE *output = fopen(output_filename, "rb");
        FILE *result = fopen(result_filename, "wb");
        if (scale_image(output, result, width, height))
        {
            fprintf(stdout, "error when output.\n");
        }
        fclose(output);
        fclose(result);
        remove(output_filename);
    }

    return 0;
}