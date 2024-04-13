#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#pragma once

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