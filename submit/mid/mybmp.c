#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct _sBmpHeader
{
    char bm[2];
    uint32_t size;
    uint32_t reserve;
    uint32_t offset;
    uint32_t header_size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bpp;
    uint32_t compression;
    uint32_t bitmap_size;
    uint32_t hres;
    uint32_t vres;
    uint32_t used;
    uint32_t important;
} __attribute__((__packed__));

typedef struct _sBmpHeader sBmpHeader;

struct _sBmpPixel24
{
    uint8_t b;
    uint8_t g;
    uint8_t r;
} __attribute__((__packed__));
typedef struct _sBmpPixel24 sBmpPixel24;

int32_t count_end_byte(int32_t width, uint16_t bpp);
sBmpHeader read_header(FILE *image_read);
int8_t write_edge_pixel(int32_t width, FILE *image_write);
sBmpPixel24 read_pixel(FILE *image_read, sBmpHeader header, int32_t row, int32_t col, int32_t end_pixel);
int8_t create_bmp24_bg(int32_t width, int32_t height, sBmpPixel24 bg_color, FILE *image_write);
int8_t scale_image(FILE *image_read, FILE *image_write, int32_t scale_width, int32_t scale_height);

int32_t count_end_byte(int32_t width, uint16_t bpp)
{
    return (4 - (width * bpp / 8 % 4)) % 4;
}

sBmpHeader read_header(FILE *image_read)
{
    sBmpHeader header;
    fread(&header, sizeof(sBmpHeader), 1, image_read);
    return header;
}

int8_t write_edge_pixel(int32_t width, FILE *image_write)
{
    char nothing[10] = {0};
    fwrite(nothing, (4 - (width * 3 % 4)) % 4, 1, image_write);
}

sBmpPixel24 read_pixel(FILE *image_read, sBmpHeader header, int32_t row, int32_t col, int32_t end_byte)
{
    sBmpPixel24 pixel;
    if(header.height < 0)
    {
        row = -(header.height + row - 1);
    }
    fseek(image_read, header.offset + row * ((header.width) * 3 + end_byte) + col * 3, SEEK_SET);
    fread(&pixel, sizeof(sBmpPixel24), 1, image_read);
    return pixel;
}

int8_t create_bmp24_bg(int32_t width, int32_t height, sBmpPixel24 bg_color, FILE *image_write)
{
    sBmpHeader header;
    header.bm[0] = 'B';
    header.bm[1] = 'M';
    header.size = 54 + width * height * 3 + count_end_byte(width, 24) * height;
    header.reserve = 0;
    header.offset = 54;
    header.header_size = 40;
    header.width = width;
    header.height = height;
    header.planes = 1;
    header.bpp = 24;
    header.compression = 0;
    header.bitmap_size = width * height * 3 + count_end_byte(width, 24) * height;
    header.hres = 0;
    header.vres = 0;
    header.used = 0;
    header.important = 0;
    fwrite(&header, sizeof(sBmpHeader), 1, image_write);
    for(int32_t i = 0; i < height; i++)
    {
        for(int32_t j = 0; j < width; j++)
        {
            fwrite(&bg_color, sizeof(sBmpPixel24), 1, image_write);
        }
        write_edge_pixel(width, image_write);
    }
}

int8_t scale_image(FILE *image_read, FILE *image_write, int32_t scale_width, int32_t scale_height)
{
    sBmpHeader header = read_header(image_read);
    sBmpHeader header_write = header;
    if(header.height < 0)
    {
        header_write.height = -header.height;
    }
    int32_t end_pixel = count_end_byte(header.width, header.bpp);
   // scale original image to new image
    header_write.width = scale_width;
    header_write.height = scale_height;
    header_write.size = 54 + scale_width * scale_height * 3 + count_end_byte(scale_width, 24) * scale_height;
    header_write.bitmap_size = scale_width * scale_height * 3 + count_end_byte(scale_width, 24) * scale_height;
    fwrite(&header_write, sizeof(sBmpHeader), 1, image_write);
    for(int32_t i = 0; i < scale_height; i++)
    {
        for(int32_t j = 0; j < scale_width; j++)
        {
            sBmpPixel24 pixel = read_pixel(image_read, header, (int32_t)(i * (double)header.height / scale_height), (int32_t)((double)j * header.width / scale_width), end_pixel);
            fwrite(&pixel, sizeof(sBmpPixel24), 1, image_write);
        }
        write_edge_pixel(scale_width, image_write);
    }
    return 0;
}
