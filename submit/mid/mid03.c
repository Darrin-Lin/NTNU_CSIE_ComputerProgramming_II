#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "mybmp.h"

int main()
{
    char file_name[5000];
    printf("cover: ");
    fgets(file_name, 5000, stdin);
    file_name[strlen(file_name) - 1] = '\0';
    FILE *cover = fopen(file_name, "rb");
    if (cover == NULL)
    {
        printf("No file.\n");
        return -1;
    }
    printf("x (in pixel): ");
    int32_t x;
    scanf("%d", &x);
    printf("y (in pixel): ");
    int32_t y;
    scanf("%d", &y);
    printf("w (in pixel): ");
    int32_t w;
    scanf("%d", &w);
    printf("h (in pixel): ");
    int32_t h;
    scanf("%d", &h);
    printf("new: ");
    fgetc(stdin);
    fgets(file_name, 5000, stdin);
    file_name[strlen(file_name) - 1] = '\0';
    FILE *new = fopen(file_name, "rb");
    if (new == NULL)
    {
        printf("No file.\n");
        return -1;
    }
    printf("output: ");
    fgets(file_name, 5000, stdin);
    file_name[strlen(file_name) - 1] = '\0';
    FILE *output = fopen(file_name, "wb");
    if (output == NULL)
    {
        printf("No file.\n");
        return -1;
    }
    sBmpHeader cover_header = read_header(cover);
    FILE *image_write = fopen("tmp.bmp", "wb");
    scale_image(new, image_write, w, h);
    fclose(image_write);
    fclose(new);
    new = fopen("tmp.bmp", "rb");
    sBmpHeader new_header = read_header(new);
    fwrite(&cover_header, sizeof(sBmpHeader), 1, output);
    // y = cover_header.width - y;
    for (int32_t i = 0; i < abs(cover_header.height); i++)
    {
        for (int32_t j = 0; j < cover_header.width; j++)
        {
            sBmpPixel24 pixel;
            if (i >= cover_header.height - y && i < cover_header.height - y + h && j >= x && j < x + w)
            {
                pixel = read_pixel(new, new_header, i - (cover_header.height - y), j - x, 0);
            }
            else
            {
                pixel = read_pixel(cover, cover_header, i, j, 0);
            }
            fwrite(&pixel, sizeof(sBmpPixel24), 1, output);
        }
        write_edge_pixel(cover_header.width, output);
    }
    fclose(cover);
    fclose(new);
    fclose(output);
    remove("tmp.bmp");

    return 0;
}