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