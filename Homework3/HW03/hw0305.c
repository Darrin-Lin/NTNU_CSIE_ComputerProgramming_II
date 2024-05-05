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
#include "myvector.h"

#define DEBUG 1
#define HELP_MSG "\
-i, --input, mandatory: input file path\n\
-o, --output, mandatory: output file path\n\
-w, --width, mandatory: output bmp file widths\n\
-h, --height, mandatory: output bmp file height\n\
-l, --line, mandatory: the radius of line\n\
-H, --help, option: show help message\n\
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

int32_t draw_circle(int32_t x, int32_t y, int32_t r, int8_t rgb, sBmpPixel24 *out_img, int32_t out_width, int32_t out_height);

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
            num_err = NULL;
            out_width = strtol(optarg, &num_err, 10);
            if (DEBUG)
            {
                fprintf(stderr, "%s", num_err);
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
            if (option[l_opt] > 1)
            {
                fprintf(stdout, "Error: -l can only be used once.\n");
                goto err_arg;
            }
            num_err = NULL;

            line_radius = strtol(optarg, &num_err, 10);

            if (*num_err != '\0')
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
    fclose(inp_bmp);
    int64_t max = 0;
    for (int32_t i = 0; i < 256; i++)
    {
        if (r_count[i] > max)
        {
            max = r_count[i];
        }
        if (g_count[i] > max)
        {
            max = g_count[i];
        }
        if (b_count[i] > max)
        {
            max = b_count[i];
        }
        if (DEBUG)
        {
            fprintf(stderr, "%d\n", i);
            fprintf(stderr, "  r: %ld\n", r_count[i]);
            fprintf(stderr, "  g: %ld\n", g_count[i]);
            fprintf(stderr, "  b: %ld\n", b_count[i]);
        }
    }
    sBmpPixel24 bg = {0, 0, 0};
    FILE *out_bmp = NULL;
    out_bmp = fopen(out_name, "wb");
    if (out_bmp == NULL)
    {
        fprintf(stdout, "Error: Can't create %s.", out_name);
        goto err_arg;
    }
    if (create_bmp24_bg(out_width, out_height, bg, out_bmp) == -1)
    {
        fprintf(stdout, "Error: Can't create bmp file.");
        goto err_arg;
    }
    fclose(out_bmp);
    if (max == 0)
    {
        return 0;
    }
    for (int32_t i = 0; i < 256; i++)
    {
        r_count[i] = r_count[i] * (abs(out_height) - 1) / max;
        g_count[i] = g_count[i] * (abs(out_height) - 1) / max;
        b_count[i] = b_count[i] * (abs(out_height) - 1) / max;
    }

    out_bmp = fopen(out_name, "rb");
    sBmpHeader output_header = read_header(out_bmp);
    end_byte = count_end_byte(abs(output_header.width), 24);
    fclose(out_bmp);
    out_bmp = fopen(out_name, "wb");
    if (out_bmp == NULL)
    {
        fprintf(stdout, "Error: Can't open %s.", out_name);

        goto err_arg;
    }
    end_byte = count_end_byte(abs(output_header.width), 24);
    fwrite(&output_header, sizeof(sBmpHeader), 1, out_bmp);
    if (max != 0)
    {
        if (DEBUG)
        {
            fprintf(stderr, "t/f: %d\n", ((abs(out_width) / 256) == 0 ? (abs(out_width) / 256) % (abs(out_width) / 256) == 0 : 1));
        }
        if (abs(out_width) > 257)
        {
            if (line_radius == 1)
            {
                for (int32_t i = 0; i < abs(output_header.height); i++)
                {
                    for (int32_t j = 0; j < abs(output_header.width); j++)
                    {

                        sBmpPixel24 out_pixel = {0, 0, 0};
                        int32_t count_into256 = j * 256 / (abs(output_header.width) - 1);

                        int32_t xa = (count_into256 * (abs(output_header.width) - 1) / 256);
                        int32_t xb = ((count_into256 + 1) * (abs(output_header.width) - 1) / 256);
                        if (count_into256 < 255)
                        {
                            if (xa != xb && i == r_count[count_into256] + (r_count[count_into256 + 1] - r_count[count_into256]) * (j - xa) / (xb - xa))
                            {
                                out_pixel.r = 255;
                            }
                            else if (r_count[count_into256 + 1] != r_count[count_into256] && j - 1 == xa + (xb - xa) * (i - r_count[count_into256]) / (r_count[count_into256 + 1] - r_count[count_into256]))
                            {
                                if ((i > r_count[count_into256 + 1] && i < r_count[count_into256]) || (i < r_count[count_into256 + 1] && i > r_count[count_into256]))
                                    out_pixel.r = 255;
                            }
                            else if ((xb - xa) * (i - r_count[count_into256]) == (r_count[count_into256 + 1] - r_count[count_into256]) * (j - xa))
                            {
                                out_pixel.r = 255;
                            }
                            if (xa != xb && i == g_count[count_into256] + (g_count[count_into256 + 1] - g_count[count_into256]) * (j - xa) / (xb - xa))
                            {
                                out_pixel.g = 255;
                            }
                            else if (g_count[count_into256 + 1] != g_count[count_into256] && j - 1 == xa + (xb - xa) * (i - g_count[count_into256]) / (g_count[count_into256 + 1] - g_count[count_into256]))
                            {
                                if ((i > g_count[count_into256 + 1] && i < g_count[count_into256]) || (i < g_count[count_into256 + 1] && i > g_count[count_into256]))
                                    out_pixel.g = 255;
                            }
                            else if ((xb - xa) * (i - g_count[count_into256]) == (g_count[count_into256 + 1] - g_count[count_into256]) * (j - xa))
                            {
                                out_pixel.g = 255;
                            }
                            if (xa != xb && i == b_count[count_into256] + (b_count[count_into256 + 1] - b_count[count_into256]) * (j - xa) / (xb - xa))
                            {
                                out_pixel.b = 255;
                            }
                            else if (b_count[count_into256 + 1] != b_count[count_into256] && j - 1 == xa + (xb - xa) * (i - b_count[count_into256]) / (b_count[count_into256 + 1] - b_count[count_into256]))
                            {
                                if ((i > b_count[count_into256 + 1] && i < b_count[count_into256]) || (i < b_count[count_into256 + 1] && i > b_count[count_into256]))
                                    out_pixel.b = 255;
                            }
                            else if ((xb - xa) * (i - b_count[count_into256]) == (b_count[count_into256 + 1] - b_count[count_into256]) * (j - xa))
                            {
                                out_pixel.b = 255;
                            }
                        }
                        else if (count_into256 == 255)
                        {
                            xa = ((count_into256 - 1) * (abs(output_header.width) - 1) / 256);
                            xb = ((count_into256) * (abs(output_header.width) - 1) / 256);
                            if (xa != xb && i == r_count[count_into256 - 1] + (r_count[count_into256] - r_count[count_into256 - 1]) * (j - xa) / (xb - xa))
                            {
                                out_pixel.r = 255;
                            }
                            else if (r_count[count_into256] != r_count[count_into256 - 1] && j - 1 == xa + (xb - xa) * (i - r_count[count_into256 - 1]) / (r_count[count_into256] - r_count[count_into256 - 1]))
                            {
                                // if ((i > r_count[count_into256] && i < r_count[count_into256 - 1]) || (i < r_count[count_into256] && i > r_count[count_into256 - 1]))
                                out_pixel.r = 255;
                            }
                            else if ((xb - xa) * (i - r_count[count_into256 - 1]) == (r_count[count_into256] - r_count[count_into256 - 1]) * (j - xa))
                            {
                                out_pixel.r = 255;
                            }
                            if (xa != xb && i == g_count[count_into256 - 1] + (g_count[count_into256] - g_count[count_into256 - 1]) * (j - xa) / (xb - xa))
                            {
                                out_pixel.g = 255;
                            }
                            else if (g_count[count_into256] != g_count[count_into256 - 1] && j - 1 == xa + (xb - xa) * (i - g_count[count_into256 - 1]) / (g_count[count_into256] - g_count[count_into256 - 1]))
                            {
                                // if ((i > g_count[count_into256] && i < g_count[count_into256 - 1]) || (i < g_count[count_into256] && i > g_count[count_into256 - 1]))
                                out_pixel.g = 255;
                            }
                            else if ((xb - xa) * (i - g_count[count_into256 - 1]) == (g_count[count_into256] - g_count[count_into256 - 1]) * (j - xa))
                            {
                                out_pixel.g = 255;
                            }
                            if (xa != xb && i == b_count[count_into256 - 1] + (b_count[count_into256] - b_count[count_into256 - 1]) * (j - xa) / (xb - xa))
                            {
                                out_pixel.b = 255;
                            }
                            else if (b_count[count_into256] != b_count[count_into256 - 1] && j - 1 == xa + (xb - xa) * (i - b_count[count_into256 - 1]) / (b_count[count_into256] - b_count[count_into256 - 1]))
                            {
                                // if ((i > b_count[count_into256] && i < b_count[count_into256 - 1]) || (i < b_count[count_into256] && i > b_count[count_into256 - 1]))
                                out_pixel.b = 255;
                            }
                            else if ((xb - xa) * (i - b_count[count_into256 - 1]) == (b_count[count_into256] - b_count[count_into256 - 1]) * (j - xa))
                            {
                                out_pixel.b = 255;
                            }
                        }
                        fwrite(&out_pixel, sizeof(sBmpPixel24), 1, out_bmp);
                    }
                    write_edge_pixel(abs(output_header.width), out_bmp);
                }
            }

            else
            {
                sBmpPixel24 *out_img = calloc(abs(output_header.width) * abs(output_header.height), sizeof(sBmpPixel24));
                if (out_img == NULL)
                {
                    fprintf(stdout, "Error: Can't allocate memory.");
                    goto err_arg;
                }
                for (int32_t i = 0; i < abs(output_header.height); i++)
                {
                    for (int32_t j = 0; j < abs(output_header.width); j++)
                    {
                        int32_t count_into256 = j * 256 / (abs(output_header.width) - 1);
                        int32_t xa = (count_into256 * (abs(output_header.width) - 1) / 256);
                        int32_t xb = ((count_into256 + 1) * (abs(output_header.width) - 1) / 256);
                        if (count_into256 < 255)
                        {
                            if (xa != xb && i == r_count[count_into256] + (r_count[count_into256 + 1] - r_count[count_into256]) * (j - xa) / (xb - xa))
                            {
                                draw_circle(j, i, line_radius, 0, out_img, out_width, out_height);
                            }
                            else if (r_count[count_into256 + 1] != r_count[count_into256] && j - 1 == xa + (xb - xa) * (i - r_count[count_into256]) / (r_count[count_into256 + 1] - r_count[count_into256]))
                            {
                                if ((i > r_count[count_into256 + 1] && i < r_count[count_into256]) || (i < r_count[count_into256 + 1] && i > r_count[count_into256]))
                                    draw_circle(j, i, line_radius, 0, out_img, out_width, out_height);
                            }
                            else if ((xb - xa) * (i - r_count[count_into256]) == (r_count[count_into256 + 1] - r_count[count_into256]) * (j - xa))
                            {
                                draw_circle(j, i, line_radius, 0, out_img, out_width, out_height);
                            }

                            if (xa != xb && i == g_count[count_into256] + (g_count[count_into256 + 1] - g_count[count_into256]) * (j - xa) / (xb - xa))
                            {
                                draw_circle(j, i, line_radius, 1, out_img, out_width, out_height);
                            }
                            else if (g_count[count_into256 + 1] != g_count[count_into256] && j - 1 == xa + (xb - xa) * (i - g_count[count_into256]) / (g_count[count_into256 + 1] - g_count[count_into256]))
                            {
                                if ((i > g_count[count_into256 + 1] && i < g_count[count_into256]) || (i < g_count[count_into256 + 1] && i > g_count[count_into256]))
                                    draw_circle(j, i, line_radius, 1, out_img, out_width, out_height);
                            }
                            else if ((xb - xa) * (i - g_count[count_into256]) == (g_count[count_into256 + 1] - g_count[count_into256]) * (j - xa))
                            {
                                draw_circle(j, i, line_radius, 1, out_img, out_width, out_height);
                            }
                            if (xa != xb && i == b_count[count_into256] + (b_count[count_into256 + 1] - b_count[count_into256]) * (j - xa) / (xb - xa))
                            {
                                draw_circle(j, i, line_radius, 2, out_img, out_width, out_height);
                            }
                            else if (b_count[count_into256 + 1] != b_count[count_into256] && j - 1 == xa + (xb - xa) * (i - b_count[count_into256]) / (b_count[count_into256 + 1] - b_count[count_into256]))
                            {
                                if ((i > b_count[count_into256 + 1] && i < b_count[count_into256]) || (i < b_count[count_into256 + 1] && i > b_count[count_into256]))
                                    draw_circle(j, i, line_radius, 2, out_img, out_width, out_height);
                            }
                            else if ((xb - xa) * (i - b_count[count_into256]) == (b_count[count_into256 + 1] - b_count[count_into256]) * (j - xa))
                            {
                                draw_circle(j, i, line_radius, 2, out_img, out_width, out_height);
                            }
                        }
                        else if (count_into256 == 255)
                        {
                            xa = ((count_into256 - 1) * (abs(output_header.width) - 1) / 256);
                            xb = ((count_into256) * (abs(output_header.width) - 1) / 256);
                            if (xa != xb && i == r_count[count_into256 - 1] + (r_count[count_into256] - r_count[count_into256 - 1]) * (j - xa) / (xb - xa))
                            {
                                draw_circle(j, i, line_radius, 0, out_img, out_width, out_height);
                            }
                            else if (r_count[count_into256] != r_count[count_into256 - 1] && j - 1 == xa + (xb - xa) * (i - r_count[count_into256 - 1]) / (r_count[count_into256] - r_count[count_into256 - 1]))
                            {
                                // if ((i > r_count[count_into256] && i < r_count[count_into256 - 1]) || (i < r_count[count_into256] && i > r_count[count_into256 - 1]))
                                draw_circle(j, i, line_radius, 0, out_img, out_width, out_height);
                            }
                            else if ((xb - xa) * (i - r_count[count_into256 - 1]) == (r_count[count_into256] - r_count[count_into256 - 1]) * (j - xa))
                            {
                                draw_circle(j, i, line_radius, 0, out_img, out_width, out_height);
                            }
                            if (xa != xb && i == g_count[count_into256 - 1] + (g_count[count_into256] - g_count[count_into256 - 1]) * (j - xa) / (xb - xa))
                            {
                                draw_circle(j, i, line_radius, 1, out_img, out_width, out_height);
                            }
                            else if (g_count[count_into256] != g_count[count_into256 - 1] && j - 1 == xa + (xb - xa) * (i - g_count[count_into256 - 1]) / (g_count[count_into256] - g_count[count_into256 - 1]))
                            {
                                // if ((i > g_count[count_into256] && i < g_count[count_into256 - 1]) || (i < g_count[count_into256] && i > g_count[count_into256 - 1]))
                                draw_circle(j, i, line_radius, 1, out_img, out_width, out_height);
                            }
                            else if ((xb - xa) * (i - g_count[count_into256 - 1]) == (g_count[count_into256] - g_count[count_into256 - 1]) * (j - xa))
                            {
                                draw_circle(j, i, line_radius, 1, out_img, out_width, out_height);
                            }
                            if (xa != xb && i == b_count[count_into256 - 1] + (b_count[count_into256] - b_count[count_into256 - 1]) * (j - xa) / (xb - xa))
                            {
                                draw_circle(j, i, line_radius, 2, out_img, out_width, out_height);
                            }
                            else if (b_count[count_into256] != b_count[count_into256 - 1] && j - 1 == xa + (xb - xa) * (i - b_count[count_into256 - 1]) / (b_count[count_into256] - b_count[count_into256 - 1]))
                            {
                                // if ((i > b_count[count_into256] && i < b_count[count_into256 - 1]) || (i < b_count[count_into256] && i > b_count[count_into256 - 1]))
                                draw_circle(j, i, line_radius, 2, out_img, out_width, out_height);
                            }
                            else if ((xb - xa) * (i - b_count[count_into256 - 1]) == (b_count[count_into256] - b_count[count_into256 - 1]) * (j - xa))
                            {
                                draw_circle(j, i, line_radius, 2, out_img, out_width, out_height);
                            }
                        }
                    }
                }

                for (int32_t i = 0; i < abs(output_header.height); i++)
                {
                    for (int32_t j = 0; j < abs(output_header.width); j++)
                    {
                        fwrite(&out_img[i * abs(output_header.width) + j], sizeof(sBmpPixel24), 1, out_bmp);
                    }
                    write_edge_pixel(abs(output_header.width), out_bmp);
                }
                free(out_img);
            }
        }
        else
        {
            int32_t interval = 256 / (abs(out_width) - 1);
            if (DEBUG)
            {
                fprintf(stderr, "interval: %d\n", interval);
            }
            if (line_radius == 1)
            {
                for (int32_t i = 0; i < abs(output_header.height); i++)
                {
                    for (int32_t j = 0; j < abs(output_header.width); j++)
                    {

                        sBmpPixel24 out_pixel = {0, 0, 0};
                        int32_t count_into256 = j * 256 / (abs(output_header.width) - 1);

                        int32_t xa = (count_into256 * (abs(output_header.width) - 1) / 256);
                        int32_t xb = ((count_into256 + interval) * (abs(output_header.width) - 1) / 256);

                        if (count_into256 < 255)
                        {
                            if (xa != xb && i == r_count[count_into256] + (r_count[count_into256 + 1] - r_count[count_into256]) * (j - xa) / (xb - xa))
                            {
                                out_pixel.r = 255;
                            }
                            else if (r_count[count_into256 + interval] != r_count[count_into256] && j - (abs(out_width == 257 ? 0 : 1)) == xa + (xb - xa) * (i - r_count[count_into256]) / (r_count[count_into256 + interval] - r_count[count_into256]))
                            {
                                if ((i > r_count[count_into256 + interval] && i < r_count[count_into256]) || (i < r_count[count_into256 + interval] && i > r_count[count_into256]))
                                    out_pixel.r = 255;
                            }
                            else if ((xb - xa) * (i - r_count[count_into256]) == (r_count[count_into256 + 1] - r_count[count_into256]) * (j - xa))
                            {
                                if ((i >= r_count[count_into256] && i <= r_count[count_into256 + interval]) || (i <= r_count[count_into256] && i >= r_count[count_into256 + interval]))
                                    out_pixel.r = 255;
                            }
                            if (xa != xb && i == g_count[count_into256] + (g_count[count_into256 + interval] - g_count[count_into256]) * (j - xa) / (xb - xa))
                            {
                                out_pixel.g = 255;
                            }
                            else if (g_count[count_into256 + interval] != g_count[count_into256] && j - (abs(out_width == 257 ? 0 : 1)) == xa + (xb - xa) * (i - g_count[count_into256]) / (g_count[count_into256 + interval] - g_count[count_into256]))
                            {
                                if ((i > g_count[count_into256 + interval] && i < g_count[count_into256]) || (i < g_count[count_into256 + interval] && i > g_count[count_into256]))
                                    out_pixel.g = 255;
                            }
                            else if ((xb - xa) * (i - g_count[count_into256]) == (g_count[count_into256 + interval] - g_count[count_into256]) * (j - xa))
                            {
                                if ((i >= g_count[count_into256] && i <= g_count[count_into256 + interval]) || (i <= g_count[count_into256] && i >= g_count[count_into256 + interval]))
                                    out_pixel.g = 255;
                            }
                            if (xa != xb && i == b_count[count_into256] + (b_count[count_into256 + interval] - b_count[count_into256]) * (j - xa) / (xb - xa))
                            {
                                out_pixel.b = 255;
                            }
                            else if (b_count[count_into256 + interval] != b_count[count_into256] && j - (abs(out_width == 257 ? 0 : 1)) == xa + (xb - xa) * (i - b_count[count_into256]) / (b_count[count_into256 + interval] - b_count[count_into256]))
                            {
                                if ((i > b_count[count_into256 + interval] && i < b_count[count_into256]) || (i < b_count[count_into256 + interval] && i > b_count[count_into256]))
                                    out_pixel.b = 255;
                            }
                            else if ((xb - xa) * (i - b_count[count_into256]) == (b_count[count_into256 + interval] - b_count[count_into256]) * (j - xa))
                            {
                                if ((i >= b_count[count_into256] && i <= b_count[count_into256 + interval]) || (i <= b_count[count_into256] && i >= b_count[count_into256 + interval]))
                                    out_pixel.b = 255;
                            }
                        }
                        else if (count_into256 == 255)
                        {
                            xa = ((count_into256 - interval) * (abs(output_header.width) - 1) / 256);
                            xb = ((count_into256) * (abs(output_header.width) - 1) / 256);
                            if (xa != xb && i == r_count[count_into256 - interval] + (r_count[count_into256] - r_count[count_into256 - interval]) * (j - xa) / (xb - xa))
                            {
                                out_pixel.r = 255;
                            }
                            else if (r_count[count_into256] != r_count[count_into256 - interval] && j == xa + (xb - xa) * (i - r_count[count_into256 - interval]) / (r_count[count_into256] - r_count[count_into256 - interval]))
                            {
                                // if ((i > r_count[count_into256] && i < r_count[count_into256 - interval]) || (i < r_count[count_into256] && i > r_count[count_into256 - interval]))
                                out_pixel.r = 255;
                            }
                            else if ((xb - xa) * (i - r_count[count_into256 - interval]) == (r_count[count_into256] - r_count[count_into256 - interval]) * (j - xa))
                            {
                                out_pixel.r = 255;
                            }
                            if (xa != xb && i == g_count[count_into256 - interval] + (g_count[count_into256] - g_count[count_into256 - interval]) * (j - xa) / (xb - xa))
                            {
                                out_pixel.g = 255;
                            }
                            else if (g_count[count_into256] != g_count[count_into256 - interval] && j == xa + (xb - xa) * (i - g_count[count_into256 - interval]) / (g_count[count_into256] - g_count[count_into256 - interval]))
                            {
                                // if ((i > g_count[count_into256] && i < g_count[count_into256 - interval]) || (i < g_count[count_into256] && i > g_count[count_into256 - interval]))
                                out_pixel.g = 255;
                            }
                            else if ((xb - xa) * (i - g_count[count_into256 - interval]) == (g_count[count_into256] - g_count[count_into256 - interval]) * (j - xa))
                            {
                                out_pixel.g = 255;
                            }
                            if (xa != xb && i == b_count[count_into256 - interval] + (b_count[count_into256] - b_count[count_into256 - interval]) * (j - xa) / (xb - xa))
                            {
                                out_pixel.b = 255;
                            }
                            else if (b_count[count_into256] != b_count[count_into256 - interval] && j == xa + (xb - xa) * (i - b_count[count_into256 - interval]) / (b_count[count_into256] - b_count[count_into256 - interval]))
                            {
                                // if ((i > b_count[count_into256] && i < b_count[count_into256 - interval]) || (i < b_count[count_into256] && i > b_count[count_into256 - interval]))
                                out_pixel.b = 255;
                            }
                            else if ((xb - xa) * (i - b_count[count_into256 - interval]) == (b_count[count_into256] - b_count[count_into256 - interval]) * (j - xa))
                            {
                                out_pixel.b = 255;
                            }
                        }
                        fwrite(&out_pixel, sizeof(sBmpPixel24), 1, out_bmp);
                    }
                    write_edge_pixel(abs(output_header.width), out_bmp);
                }
            }

            else
            {
                sBmpPixel24 *out_img = calloc(abs(output_header.width) * abs(output_header.height), sizeof(sBmpPixel24));
                if (out_img == NULL)
                {
                    fprintf(stdout, "Error: Can't allocate memory.");
                    goto err_arg;
                }
                for (int32_t i = 0; i < abs(output_header.height); i++)
                {
                    for (int32_t j = 0; j < abs(output_header.width); j++)
                    {
                        int32_t count_into256 = j * 256 / (abs(output_header.width) - 1);
                        int32_t xa = (count_into256 * (abs(output_header.width) - 1) / 256);
                        int32_t xb = ((count_into256 + interval) * (abs(output_header.width) - 1) / 256);
                        if (count_into256 < 255)
                        {
                            if (xa != xb && i == r_count[count_into256] + (r_count[count_into256 + 1] - r_count[count_into256]) * (j - xa) / (xb - xa))
                            {
                                draw_circle(j, i, line_radius, 0, out_img, out_width, out_height);
                            }
                            else if (r_count[count_into256 + interval] != r_count[count_into256] && j - (abs(out_width == 257 ? 0 : 1)) == xa + (xb - xa) * (i - r_count[count_into256]) / (r_count[count_into256 + interval] - r_count[count_into256]))
                            {
                                if ((i > r_count[count_into256 + interval] && i < r_count[count_into256]) || (i < r_count[count_into256 + interval] && i > r_count[count_into256]))
                                    draw_circle(j, i, line_radius, 0, out_img, out_width, out_height);
                            }
                            else if ((xb - xa) * (i - r_count[count_into256]) == (r_count[count_into256 + 1] - r_count[count_into256]) * (j - xa))
                            {
                                if ((i >= r_count[count_into256 + interval] && i <= r_count[count_into256]) || (i <= r_count[count_into256 + interval] && i >= r_count[count_into256]))
                                    draw_circle(j, i, line_radius, 0, out_img, out_width, out_height);
                            }
                            if (xa != xb && i == g_count[count_into256] + (g_count[count_into256 + interval] - g_count[count_into256]) * (j - xa) / (xb - xa))
                            {
                                draw_circle(j, i, line_radius, 1, out_img, out_width, out_height);
                            }
                            else if (g_count[count_into256 + interval] != g_count[count_into256] && j - (abs(out_width == 257 ? 0 : 1)) == xa + (xb - xa) * (i - g_count[count_into256]) / (g_count[count_into256 + interval] - g_count[count_into256]))
                            {
                                if ((i > g_count[count_into256 + interval] && i < g_count[count_into256]) || (i < g_count[count_into256 + interval] && i > g_count[count_into256]))
                                    draw_circle(j, i, line_radius, 1, out_img, out_width, out_height);
                            }
                            else if ((xb - xa) * (i - g_count[count_into256]) == (g_count[count_into256 + interval] - g_count[count_into256]) * (j - xa))
                            {
                                if ((i >= g_count[count_into256 + interval] && i <= g_count[count_into256]) || (i <= g_count[count_into256 + interval] && i >= g_count[count_into256]))
                                    draw_circle(j, i, line_radius, 1, out_img, out_width, out_height);
                            }
                            if (xa != xb && i == b_count[count_into256] + (b_count[count_into256 + interval] - b_count[count_into256]) * (j - xa) / (xb - xa))
                            {
                                draw_circle(j, i, line_radius, 2, out_img, out_width, out_height);
                            }
                            else if (b_count[count_into256 + interval] != b_count[count_into256] && j - (abs(out_width == 257 ? 0 : 1)) == xa + (xb - xa) * (i - b_count[count_into256]) / (b_count[count_into256 + interval] - b_count[count_into256]))
                            {
                                if ((i > b_count[count_into256 + interval] && i < b_count[count_into256]) || (i < b_count[count_into256 + interval] && i > b_count[count_into256]))
                                    draw_circle(j, i, line_radius, 2, out_img, out_width, out_height);
                            }
                            else if ((xb - xa) * (i - b_count[count_into256]) == (b_count[count_into256 + interval] - b_count[count_into256]) * (j - xa))
                            {
                                if ((i >= b_count[count_into256 + interval] && i <= b_count[count_into256]) || (i <= b_count[count_into256 + interval] && i >= b_count[count_into256]))
                                    draw_circle(j, i, line_radius, 2, out_img, out_width, out_height);
                            }
                        }
                        else if (count_into256 == 255)
                        {
                            xa = ((count_into256 - interval) * (abs(output_header.width) - 1) / 256);
                            xb = ((count_into256) * (abs(output_header.width) - 1) / 256);
                            if (xa != xb && i == r_count[count_into256 - interval] + (r_count[count_into256] - r_count[count_into256 - interval]) * (j - xa) / (xb - xa))
                            {
                                draw_circle(j, i, line_radius, 0, out_img, out_width, out_height);
                            }
                            else if (r_count[count_into256] != r_count[count_into256 - interval] && j == xa + (xb - xa) * (i - r_count[count_into256 - interval]) / (r_count[count_into256] - r_count[count_into256 - interval]))
                            {
                                // if ((i > r_count[count_into256] && i < r_count[count_into256 - interval]) || (i < r_count[count_into256] && i > r_count[count_into256 - interval]))
                                draw_circle(j, i, line_radius, 0, out_img, out_width, out_height);
                            }
                            else if ((xb - xa) * (i - r_count[count_into256 - interval]) == (r_count[count_into256] - r_count[count_into256 - interval]) * (j - xa))
                            {
                                draw_circle(j, i, line_radius, 0, out_img, out_width, out_height);
                            }
                            if (xa != xb && i == g_count[count_into256 - interval] + (g_count[count_into256] - g_count[count_into256 - interval]) * (j - xa) / (xb - xa))
                            {
                                draw_circle(j, i, line_radius, 1, out_img, out_width, out_height);
                            }
                            else if (g_count[count_into256] != g_count[count_into256 - interval] && j == xa + (xb - xa) * (i - g_count[count_into256 - interval]) / (g_count[count_into256] - g_count[count_into256 - interval]))
                            {
                                // if ((i > g_count[count_into256] && i < g_count[count_into256 - interval]) || (i < g_count[count_into256] && i > g_count[count_into256 - interval]))
                                draw_circle(j, i, line_radius, 1, out_img, out_width, out_height);
                            }
                            else if ((xb - xa) * (i - g_count[count_into256 - interval]) == (g_count[count_into256] - g_count[count_into256 - interval]) * (j - xa))
                            {
                                draw_circle(j, i, line_radius, 1, out_img, out_width, out_height);
                            }
                            if (xa != xb && i == b_count[count_into256 - interval] + (b_count[count_into256] - b_count[count_into256 - interval]) * (j - xa) / (xb - xa))
                            {
                                draw_circle(j, i, line_radius, 2, out_img, out_width, out_height);
                            }
                            else if (b_count[count_into256] != b_count[count_into256 - interval] && j == xa + (xb - xa) * (i - b_count[count_into256 - interval]) / (b_count[count_into256] - b_count[count_into256 - interval]))
                            {
                                // if ((i > b_count[count_into256] && i < b_count[count_into256 - interval]) || (i < b_count[count_into256] && i > b_count[count_into256 - interval]))
                                draw_circle(j, i, line_radius, 2, out_img, out_width, out_height);
                            }
                            else if ((xb - xa) * (i - b_count[count_into256 - interval]) == (b_count[count_into256] - b_count[count_into256 - interval]) * (j - xa))
                            {
                                draw_circle(j, i, line_radius, 2, out_img, out_width, out_height);
                            }
                        }
                    }
                }

                for (int32_t i = 0; i < abs(output_header.height); i++)
                {
                    for (int32_t j = 0; j < abs(output_header.width); j++)
                    {
                        fwrite(&out_img[i * abs(output_header.width) + j], sizeof(sBmpPixel24), 1, out_bmp);
                    }
                    write_edge_pixel(abs(output_header.width), out_bmp);
                }
                free(out_img);
            }
        }
    }
    fclose(out_bmp);
    if (out_height < 0)
    {
        out_bmp = fopen("output.bmp", "rb");
        if (out_bmp == NULL)
        {
            fprintf(stdout, "Error: Can't open file.");
            goto err_arg;
        }
        FILE *tmp_out = fopen("tmp_out.bmp", "wb");
        if (tmp_out == NULL)
        {
            fprintf(stdout, "Error: Can't open file.");
            goto err_arg;
        }
        fwrite(&output_header, sizeof(sBmpHeader), 1, tmp_out);
        output_header.height = -output_header.height;
        for (int32_t i = 0; i < abs(output_header.height); i++)
        {
            for (int32_t j = 0; j < abs(output_header.width); j++)
            {
                sBmpPixel24 out_pixel;
                out_pixel = read_pixel(out_bmp, output_header, output_header.height- i-1, j, end_byte);
                fwrite(&out_pixel, sizeof(sBmpPixel24), 1, tmp_out);
            }
            write_edge_pixel(abs(output_header.width), tmp_out);
        }
        fclose(out_bmp);
    }
    return 0;
err_arg:
    return -1;
}

int32_t draw_circle(int32_t x, int32_t y, int32_t r, int8_t rgb, sBmpPixel24 *out_img, int32_t out_width, int32_t out_height)
{
    for (int32_t k = 0; k < r; k++)
    {
        for (int32_t l = y - k; l <= y + k; l++)
        {
            for (int32_t m = x - k; m <= x + k; m++)
            {
                if (l >= 0 && l < abs(out_height) && m >= 0 && m < abs(out_width))
                {
                    if ((l - y) * (l - y) + (m - x) * (m - x) <= k * k)
                    {
                        if (rgb == 0)
                            if (255 - k * 255 / r > out_img[l * abs(out_width) + m].r)
                                out_img[l * abs(out_width) + m].r = 255 - k * 255 / r;
                        if (rgb == 1)
                            if (255 - k * 255 / r > out_img[l * abs(out_width) + m].g)
                                out_img[l * abs(out_width) + m].g = 255 - k * 255 / r;
                        if (rgb == 2)
                            if (255 - k * 255 / r > out_img[l * abs(out_width) + m].b)
                                out_img[l * abs(out_width) + m].b = 255 - k * 255 / r;
                    }
                }
            }
        }
    }

    return 0;
}
