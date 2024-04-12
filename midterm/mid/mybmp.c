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

int8_t read_edge_pixel(int32_t width, FILE *image_read);
int8_t write_edge_pixel(int32_t width);

int8_t read_edge_pixel(int32_t width, FILE *image_read)
{
    char not_use[10];
    fread(not_use, (4 - (width * 3 % 4)) % 4, 1, image_read);
}