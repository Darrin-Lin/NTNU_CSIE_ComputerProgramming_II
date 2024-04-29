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

#define DEBUG 1
#define HELP_MSG "\
-i, - -input, mandatory: input file path\n\
-o, - -output, mandatory: output file path\n\
-w, - -width, mandatory: output bmp file widths\n\
-h, - -height, mandatory: output bmp file height\n\
-l, - -line, mandatory: the radius of line\n\
-H, - -help, option: show help message\n\
"

enum option_index
{
    i_opt = 0,
    o_opt,
    w_opt,
    h_opt,
    l_opt,
    H_opt
};

int main(int argc, char *argv[])
{
    int64_t r_count[256] = {0};
    int64_t g_count[256] = {0};
    int64_t b_count[256] = {0};
    struct option longopts[] = {
        {"input", required_argument, NULL, 'i'},
        {"output", required_argument, NULL, 'o'},
        {"width", required_argument, NULL, 'w'},
        {"height", required_argument, NULL, 'h'},
        {"line", required_argument, NULL, 'l'},
        {"help", no_argument, NULL, 'H'},
        {0, 0, 0, 0}};
    int8_t option[6] = {0};
    int32_t opt = 0;
    char inp_name[1024];
    char out_name[1024];
    int32_t out_width = 0;
    int32_t out_height = 0;
    int32_t line_radius = 0;
    while ((opt = getopt_long(argc, argv, "i:o:w:h:l:H", longopts, NULL)) != -1)
    {
            char *num_err = NULL;

        switch (opt)
        {
        case 'i':
            option[i_opt]++;
            if (option[i_opt] > 1)
            {
                fprintf(stdout, "Error: -i can only be used once.\n");
                goto err_arg;
            }
            strcpy(inp_name, optarg);
            break;
        case 'o':
            option[o_opt]++;
            if (option[o_opt] > 1)
            {
                fprintf(stdout, "Error: -o can only be used once.\n");
                goto err_arg;
            }
            strcpy(out_name, optarg);
            break;
        case 'w':
            option[w_opt]++;
            if (option[w_opt] > 1)
            {
                fprintf(stdout, "Error: -w can only be used once.\n");
                goto err_arg;
            }
            num_err=NULL;
            out_width = strtol(optarg, &num_err, 10);
            if(DEBUG)
            {
                fprintf(stderr,"%s",num_err);
            }
            if (*num_err != '\0')
            {
                fprintf(stdout, "Error: %s is not a number", optarg);
                goto err_arg;
            }
            break;
        case 'h':
            option[h_opt]++;
            if (option[h_opt] > 1)
            {
                fprintf(stdout, "Error: -h can only be used once.\n");
                goto err_arg;
            }
            num_err = NULL;
            out_height = strtol(optarg, &num_err, 10);
            if (*num_err != '\0')
            {
                fprintf(stdout, "Error: %s is not a number", optarg);
                goto err_arg;
            }
            break;
        case 'l':
            option[l_opt]++;
            if(option[l_opt]>1)
            {
                fprintf(stdout, "Error: -l can only be used once.\n");
                goto err_arg;
            }
            num_err = NULL;
        
            line_radius=strtol(optarg, &num_err,10);
            
            if(*num_err != '\0')
            {
                fprintf(stdout, "Error: %s is not a number", optarg);
                goto err_arg;
            }
            break;
        case 'H':
            option[h_opt]++;
            fprintf(stdout, HELP_MSG);
            return 0;
        default:
            fprintf(stdout, "Error: Invalid argument.\n");
            goto err_arg;
            break;
        }
    }
    for (int32_t i = 0; i < 5; i++)
    {
        if (option[i] == 0)
        {
            fprintf(stdout, "Error: Missing argument.");
            goto err_arg;
        }
    }
    FILE *inp_bmp = NULL;
    inp_bmp = fopen(inp_name, "rb");
    if (inp_bmp == NULL)
    {
        fprintf(stdout, "Error: Can't open %s.", inp_name);
        goto err_arg;
    }
    sBmpHeader input_header = read_header(inp_bmp);
    int32_t end_byte = count_end_byte(input_header.width, 24);
    for (int32_t i = 0; i < abs(input_header.height); i++)
    {
        for (int32_t j = 0; j < abs(input_header.width); j++)
        {
            sBmpPixel24 inp_pixel;
            inp_pixel = read_pixel(inp_bmp, input_header, i, j, end_byte);
            r_count[inp_pixel.r]++;
            g_count[inp_pixel.g]++;
            b_count[inp_pixel.b]++;
        }
    }
    if (DEBUG)
    {
        for (int32_t i = 0; i < 256; i++)
        {
            fprintf(stderr, "%d\n", i);
            fprintf(stderr, "  r: %ld\n", r_count[i]);
            fprintf(stderr, "  g: %ld\n", g_count[i]);
            fprintf(stderr, "  b: %ld\n", b_count[i]);
        }
    }
    return 0;
err_arg:
    return -1;
}