#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define fptf fprintf
#define DEBUG 0

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

int main()
{
    char file_name[100];
    printf("Please input a BMP file: ");
    fgets(file_name, 100, stdin);
    file_name[strlen(file_name) - 1] = '\0';
    char output_name[100];
    printf("Please input the output BMP file name: ");
    fgets(output_name, 100, stdin);
    output_name[strlen(output_name) - 1] = '\0';
    double angle;
    printf("Angle (0-90): ");
    scanf("%lf", &angle);
    if (angle < 0 || angle >= 90)
    {
        printf("Invalid angle\n");
        return -1;
    }
    if (strcmp(file_name, output_name) == 0)
    {
        printf("Input and output file must be different\n");
        return -1;
    }
    angle = angle * M_PI / 180;
    FILE *image_read = fopen(file_name, "rb");
    FILE *image_write = fopen(output_name, "wb");
    if (image_read == NULL)
    {
        return -1;
    }
    if (image_write == NULL)
    {
        return -1;
    }
    sBmpHeader header;
    fread(&header, sizeof(header), 1, image_read);

    if (header.bm[0] != 'B' || header.bm[1] != 'M')
    {
        printf("Not a BMP file\n");
        return -1;
    }
    sBmpHeader header_write;
    header_write = header;
    if (header.width < 0)
    {
        header.width = -header.width;
    }
    if (header.height < 0)
    {
        header.height = -header.height;
    }
    size_t shift = header.height * tan(angle);
    header_write.width = shift + header.width;
    if (header_write.bpp == 24)
    {
        header_write.size = header.offset + header.height * (header_write.width * 3 + (4 - ((header_write.width * 3) % 4)) % 4);
    }
    else if (header_write.bpp >= 8)
    {
        header_write.size = header.offset + header.height * (header_write.width * header_write.bpp / 8);
    }
    header_write.bitmap_size = header_write.size - header.offset;
    // print header
    if (DEBUG)
    {
        printf("ID: %c%c\n", header.bm[0], header.bm[1]);
        printf("Size: %u\n", header.size);
        printf("Reserve: %u\n", header.reserve);
        printf("Offset: %u\n", header.offset);
        printf("Header Size: %u\n", header.header_size);
        printf("Width: %u\n", header.width);
        printf("Height: %u\n", header.height);
        printf("Planes: %u\n", header.planes);
        printf("Bits Per Pixel: %u\n", header.bpp);
        printf("Compression: %u\n", header.compression);
        printf("Bitmap Data Size: %u\n", header.bitmap_size);
        printf("H-Resolution: %u\n", header.hres);
        printf("V-Resolution: %u\n", header.vres);
        printf("Used Colors: %u\n", header.used);
        printf("Important Colors: %u\n", header.important);
    }
    // print header_write
    if (DEBUG)
    {
        printf("ID: %c%c\n", header_write.bm[0], header_write.bm[1]);
        printf("Size: %u\n", header_write.size);
        printf("Reserve: %u\n", header_write.reserve);
        printf("Offset: %u\n", header_write.offset);
        printf("Header Size: %u\n", header_write.header_size);
        printf("Width: %u\n", header_write.width);
        printf("Height: %u\n", header_write.height);
        printf("Planes: %u\n", header_write.planes);
        printf("Bits Per Pixel: %u\n", header_write.bpp);
        printf("Compression: %u\n", header_write.compression);
        printf("Bitmap Data Size: %u\n", header_write.bitmap_size);
        printf("H-Resolution: %u\n", header_write.hres);
        printf("V-Resolution: %u\n", header_write.vres);
        printf("Used Colors: %u\n", header_write.used);
        printf("Important Colors: %u\n", header_write.important);
    }
    fwrite(&header_write, sizeof(header_write), 1, image_write);
    size_t line = 0;
    if (DEBUG)
        fptf(stderr, "shift: %ld\n", shift);
    size_t pixel = 0;
    while (!feof(image_read))
    {
        size_t blank = 0;
        blank = (header.height - line) * tan(angle);
        // fptf(stderr, "blank: %ld\n", blank);
        // fptf(stderr,"%d\n",(header_write.width * 3) - (blank + header.width * 3));
        if (header_write.height > 0)
        {
            for (size_t i = 0; i < (header_write.width) - (blank + header.width); i++)
            {
                if (header_write.bpp == 24)
                {
                    uint8_t white[3] = {0};
                    white[0] = 255;
                    white[1] = 255;
                    white[2] = 255;
                    if (DEBUG)
                    {
                        if (i == 0)
                        {
                            white[0] = 0;
                            white[1] = 0;
                            white[2] = 0;
                        }
                    }
                    fwrite(white, 3, 1, image_write);
                    pixel += 3;
                }
                else if (header_write.bpp >= 8)
                {
                    uint8_t white[10] = {0};
                    for (int32_t j = 0; j < header_write.bpp / 8; j++)
                    {

                        white[j] = 255;
                        if (DEBUG)
                        {
                            if (i == 0)
                            {
                                white[j] = 0;
                            }
                        }
                    }
                    fwrite(white, header_write.bpp / 8, 1, image_write);
                    pixel += header_write.bpp / 8;
                }
            }
        }
        else
        {
            if (header_write.bpp == 24)
            {
                for (size_t i = 0; i < blank; i++)
                {
                    uint8_t white[3] = {0};
                    white[0] = 255;
                    white[1] = 255;
                    white[2] = 255;
                    if (DEBUG)
                    {
                        if (i == blank - 1)
                        {
                            white[0] = 0;
                            white[1] = 0;
                            white[2] = 0;
                        }
                    }
                    fwrite(white, 3, 1, image_write);
                    pixel += 3;
                }
            }
            else if (header_write.bpp >= 8)
            {
                for (size_t i = 0; i < blank; i++)
                {
                    uint8_t white[10] = {0};
                    for (int32_t i = 0; i < header_write.bpp / 8; i++)
                    {
                        white[i] = 255;
                    }
                    if (DEBUG)
                    {
                        if (i == blank - 1)
                        {
                            for (int32_t i = 0; i < header_write.bpp / 8; i++)
                            {
                                white[i] = 0;
                            }
                        }
                    }
                    fwrite(white, header_write.bpp / 8, 1, image_write);
                    pixel += header_write.bpp / 8;
                }
            }
        }

        // fptf(stderr, "blank_pixel: %ld\n", pixel);
        size_t col = 0;
        while (!feof(image_read))
        {
            uint8_t original[10] = {0};
            uint8_t modified[10] = {0}; // problem with 3840
            size_t count = 0;
            if (header_write.bpp == 24)
            {

                count = fread(original, 1, 3, image_read);
                if (count == 0)
                {
                    continue;
                }
                for (size_t i = 0; i < count; i++)
                {
                    modified[i] = original[i];
                    pixel++;
                }
                if (DEBUG)
                {
                    if (col == 0)
                    {
                        modified[0] = 0;
                        modified[1] = 0;
                        modified[2] = 0;
                    }
                }
            }
            else if (header_write.bpp >= 8)
            {
                count = fread(original, 1, header_write.bpp / 8, image_read);
                if (count == 0)
                {
                    continue;
                }
                for (size_t i = 0; i < count; i++)
                {
                    modified[i] = original[i];
                    pixel++;
                }
                if (DEBUG)
                {
                    if (col == 0)
                    {
                        for (int32_t i = 0; i < header_write.bpp / 8; i++)
                        {
                            modified[i] = 0;
                        }
                    }
                }
            }
            col++;
            if (col >= header.width)
            {
                // fptf(stderr, "col: %ld\n", col);
                // fptf(stderr, "i: %ld\n", i);
                col = 0;
                if (header_write.bpp == 24)
                {
                    if (DEBUG)
                    {
                        modified[0] = 0;
                        modified[1] = 0;
                        modified[2] = 0;
                    }
                    fwrite(modified, 3, 1, image_write);
                }
                else if (header_write.bpp >= 8)
                {
                    if (DEBUG)
                    {
                        for (int32_t i = 0; i < header_write.bpp / 8; i++)
                        {
                            modified[i] = 0;
                        }
                    }
                    fwrite(modified, header_write.bpp / 8, 1, image_write);
                }
                // fptf(stderr, "pixel: %ld\n", pixel);
                goto blank_back;
                // break;
            }
            fwrite(modified, count, 1, image_write);
        }

    blank_back:
        line++;
        char not_use[10];
        if (header_write.bpp == 24)
        {
            fread(not_use, (4 - (header.width * 3 % 4)) % 4, 1, image_read);
            if (header_write.height > 0)
            {
                for (size_t i = 0; i < blank; i++)
                {
                    uint8_t white[3] = {0};
                    white[0] = 255;
                    white[1] = 255;
                    white[2] = 255;
                    if (DEBUG)
                    {
                        if (i == blank - 1)
                        {
                            white[0] = 0;
                            white[1] = 0;
                            white[2] = 0;
                        }
                    }
                    fwrite(white, 3, 1, image_write);
                    pixel += 3;
                }
            }
            else
            {
                for (size_t i = 0; i < (header_write.width) - (blank + header.width); i++)
                {

                    uint8_t white[3] = {0};
                    white[0] = 255;
                    white[1] = 255;
                    white[2] = 255;
                    if (DEBUG)
                    {
                        if (i == 0)
                        {
                            white[0] = 0;
                            white[1] = 0;
                            white[2] = 0;
                        }
                    }
                    fwrite(white, 3, 1, image_write);
                    pixel += 3;
                }
            }
            for (uint32_t i = 0; i < (4 - (header_write.width * 3 % 4)) % 4; i++)
            {
                fputc(0, image_write);
            }
        }
        else if (header_write.bpp >= 8)
        {
            // fptf(stderr, "blank: %ld\n", (4 - (header_write.width * header_write.bpp / 8) % 4) % 4);
            fread(not_use, (4 - (header.width * (header_write.bpp / 8) % 4)) % 4, 1, image_read);
            if (header_write.height > 0)
            {
                for (size_t i = 0; i < blank; i++)
                {
                    uint8_t white[10] = {0};
                    for (int32_t i = 0; i < header_write.bpp / 8; i++)
                    {
                        white[i] = 255;
                    }
                    if (DEBUG)
                    {
                        if (i == blank - 1)
                        {
                            for (int32_t i = 0; i < header_write.bpp / 8; i++)
                            {
                                white[i] = 0;
                            }
                        }
                    }
                    fwrite(white, header_write.bpp / 8, 1, image_write);
                    pixel += header_write.bpp / 8;
                }
            }
            else
            {
                for (size_t i = 0; i < (header_write.width) - (blank + header.width); i++)
                {

                    uint8_t white[10] = {0};
                    for (int32_t j = 0; j < header_write.bpp / 8; j++)
                    {

                        white[j] = 255;
                        if (DEBUG)
                        {
                            if (i == 0)
                            {
                                white[j] = 0;
                            }
                        }
                    }
                    fwrite(white, header_write.bpp / 8, 1, image_write);
                    pixel += header_write.bpp / 8;
                }
            }

            for (uint32_t i = 0; i < (4 - (header_write.width * (header_write.bpp / 8) % 4)) % 4; i++)
            {
                fputc(0, image_write);
            }
        }
        // if (shift != 0)
        //     fputc(255, image_write); // dont know why but it works
        // fptf(stderr, "blank_back_pixel: %ld\n", pixel);
        if (line == header.height)
        {
            break;
        }
        // fptf(stderr, "Line: %ld\n", line);
    }
    if (DEBUG)
        fptf(stderr, "pixel: %ld\n", pixel);
    fclose(image_read);
    fclose(image_write);
    return 0;
}
/*
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

struct _sBmpHeader
{
    char		bm[2];
    uint32_t	size;
    uint32_t	reserve;
    uint32_t	offset;
    uint32_t	header_size;
    uint32_t	width;
    uint32_t	height;
    uint16_t	planes;
    uint16_t	bpp;
    uint32_t	compression;
    uint32_t	bitmap_size;
    uint32_t	hres;
    uint32_t	vres;
    uint32_t	used;
    uint32_t	important;
}__attribute__ ((__packed__));

typedef struct _sBmpHeader sBmpHeader;

void print_bmp_header( sBmpHeader *pHeader )
{
    printf( "ID: %c%c\n", pHeader -> bm[0], pHeader -> bm[1] );
    printf( "Size: %u\n", pHeader -> size );
    printf( "Reserve: %u\n", pHeader -> reserve );
    printf( "Offset: %u\n", pHeader -> offset );
    printf( "Header Size: %u\n", pHeader -> header_size );
    printf( "Width: %u\n", pHeader -> width );
    printf( "Height: %u\n", pHeader -> height );
    printf( "Planes: %u\n", pHeader -> planes );
    printf( "Bits Per Pixel: %u\n", pHeader -> bpp );
    printf( "Compression: %u\n", pHeader -> compression );
    printf( "Bitmap Data Size: %u\n", pHeader -> bitmap_size );
    printf( "H-Resolution: %u\n", pHeader -> hres );
    printf( "V-Resolution: %u\n", pHeader -> vres );
    printf( "Used Colors: %u\n", pHeader -> used );
    printf( "Important Colors: %u\n", pHeader -> important );

    return;
}


int main()
{
    FILE	*pFile = NULL;
    FILE	*pFile2 = NULL;

    if( ( pFile = fopen( "maldives.bmp", "rb" ) ) == NULL )
    {
        printf( "File could not be opened!\n" );
        return 0;
    }

    if( ( pFile2 = fopen( "maldives_out.bmp", "wb" ) ) == NULL )
    {
        printf( "File could not be opened!\n" );
        return 0;
    }

    sBmpHeader	header;
    fread( &header, sizeof( header ), 1, pFile );
    fwrite( &header, sizeof( header ), 1, pFile2 );

    while( !feof( pFile ) )
    {
        uint8_t	original[1024] = {0};
        uint8_t modified[1024] = {0};

        size_t count = fread( original, 1, 1024, pFile );

        for( size_t i = 0 ; i < count ; i++ )
        {
            modified[i] = 255 - original[i];
        }

        fwrite( modified, count, 1, pFile2 );

    }


    fclose( pFile );
    fclose( pFile2 );

    return 0;
}
*/