#include "mybmp.h"

int main()
{
    FILE *image_read = fopen("inv.bmp", "rb");
    FILE *image_write = fopen("b.bmp", "wb");
    sBmpHeader header = read_header(image_read);
    sBmpHeader header_write = header;
    if(header.height < 0)
    {
        header_write.height = -header.height;
    }
    int32_t end_pixel = count_end_byte(header.width, header.bpp);
    fwrite(&header_write, sizeof(sBmpHeader), 1, image_write);
    for (int32_t i = 0; i < abs(header.height); i++)
    {
        for (int32_t j = 0; j < header.width; j++)
        {
            sBmpPixel24 pixel = read_pixel(image_read, header, i, j, end_pixel);
            fwrite(&pixel, sizeof(sBmpPixel24), 1, image_write);
        }
        write_edge_pixel(header.width, image_write);
    }
    fclose(image_read);
    fclose(image_write);
    return 0;
}