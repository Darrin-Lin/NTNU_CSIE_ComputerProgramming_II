#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "mybmp.h"

#define WIDTH 256
#define HEIGHT 256
int32_t di[8] = {1, 1, 0, -1, -1, -1, 0, 1};
int32_t dj[8] = {0, 1, 1, 1, 0, -1, -1, -1};
int32_t ri = 0, rj = 0;
int32_t now_x = 127, now_y = 127;
int32_t color_now = 0;
sBmpPixel24 color_draw[8];
sBmpPixel24 picture[256][256];

int8_t F_command()
{
    now_x += di[ri];
    now_y += dj[rj];
    if (now_x < 0 || now_x >= WIDTH || now_y < 0 || now_y >= HEIGHT)
    {
        return -1;
    }
    picture[now_x][now_y] = color_draw[color_now];
    return 0;
}
int8_t R_command()
{
    ri = (ri + 1) % 8;
    rj = (rj + 1) % 8;
    return 0;
}

int8_t C_command()
{
    color_now = (color_now + 1) % 8;
    return 0;
}

int8_t loop_command(int32_t *code_n, char *code)
{
    (*code_n)++;
    int32_t temp = *code_n;
    while (code[*code_n] != ']')
    {
        if (code[*code_n] == '[')
        {
            loop_command(code_n, code);
        }
        else if (code[*code_n] == 'F')
        {
            if (F_command() == -1)
            {
                break;
            }
        }
        else if (code[*code_n] == 'R')
        {
            R_command();
        }
        else if (code[*code_n] == 'C')
        {
            C_command();
        }
        (*code_n)++;
        
    }
    
    *code_n = temp;

    while (code[*code_n] != ']')
    {
        if (code[*code_n] == '[')
        {
            loop_command(code_n, code);
        }
        else if (code[*code_n] == 'F')
        {
            if (F_command() == -1)
            {
                break;
            }
        }
        else if (code[*code_n] == 'R')
        {
            R_command();
        }
        else if (code[*code_n] == 'C')
        {
            C_command();
        }
        (*code_n)++;
    }
    return 0;
}

int main()
{
    char file_name[5000];
    printf("Enter the output filename: ");
    fgets(file_name, 5000, stdin);
    file_name[strlen(file_name) - 1] = '\0';
    FILE *file = fopen(file_name, "wb");
    if (file == NULL)
    {
        printf("No file.\n");
        return -1;
    }
    printf("Enter the background color (R,G,B): ");
    sBmpPixel24 bg_color;
    scanf("(%hhd,%hhd,%hhd)", &bg_color.r, &bg_color.g, &bg_color.b);
    printf("Type code here: ");
    fgetc(stdin);
    char code[1000];
    fgets(code, 1000, stdin);
    code[strlen(code) - 1] = '\0';

    create_bmp24_bg(WIDTH, HEIGHT, bg_color, file);
    fclose(file);
    file = fopen(file_name, "rb");
    sBmpHeader header = read_header(file);
    // forget in midterm.
    for(int32_t i =0;i<256;i++)
    {
        for(int32_t j = 0;j<256;j++)
        {
            picture[i][j]=read_pixel(file,header,i,j,0);
        }
    }
    // forget in midterm
    file = fopen(file_name, "wb");

    {
        color_draw[0].r = 0xff;
        color_draw[0].g = 0xff;
        color_draw[0].b = 0xff;

        color_draw[1].r = 0;
        color_draw[1].g = 0;
        color_draw[1].b = 0;

        color_draw[2].r = 0x33;
        color_draw[2].g = 0x66;
        color_draw[2].b = 0xff;

        color_draw[3].r = 0x00;
        color_draw[3].g = 0xcc;
        color_draw[3].b = 0x00;

        color_draw[4].r = 0x00;
        color_draw[4].g = 0xcc;
        color_draw[4].b = 0xcc;

        color_draw[5].r = 0xcc;
        color_draw[5].g = 0x00;
        color_draw[5].b = 0x00;

        color_draw[6].r = 0xcc;
        color_draw[6].g = 0x00;
        color_draw[6].b = 0xcc;

        color_draw[7].r = 0xcc;
        color_draw[7].g = 0xcc;
        color_draw[7].b = 0x00;
    }
    int32_t code_n = 0;
    while (code[code_n] != '\0')
    {
        if (code[code_n] == '[')
        {
            loop_command(&code_n, code);
        }
        else if (code[code_n] == 'F')
        {
            if (F_command() == -1)
            {
                break;
            }
        }
        else if (code[code_n] == 'R')
        {
            R_command();
        }
        else if (code[code_n] == 'C')
        {
            C_command();
        }
        code_n++;
    }

    fwrite(&header, sizeof(sBmpHeader), 1, file);
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            fwrite(&picture[i][j], sizeof(sBmpPixel24), 1, file);
        }
        write_edge_pixel(WIDTH, file);
    }
    fclose(file);
    return 0;
}