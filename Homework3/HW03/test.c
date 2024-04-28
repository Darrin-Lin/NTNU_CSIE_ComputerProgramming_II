#include "mybmp.h"

int main()
{
    FILE *fp = fopen("b.bmp", "wb");
    if (fp == NULL)
    {
        printf("Error: cannot open file\n");
        return 1;
    }
    sBmpPixel24 bg = {0, 0, 0};
    create_bmp24_bg(8,8,bg,fp);
}