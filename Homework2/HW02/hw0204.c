#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define fptf fprintf

struct _sBmpHeader
{
    char bm[2];
    uint32_t size;
    uint32_t reserve;
    uint32_t offset;
    uint32_t header_size;
    uint32_t width;
    uint32_t height;
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
    if (angle < 0 || angle > 90)
    {
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
    sBmpHeader header_write;
    header_write = header;
    size_t shift = header.height * tan(angle);
    header_write.width = shift + header.width;
    header_write.size = header.offset + header.height * header_write.width * 3;
    header_write.bitmap_size = header.height * header_write.width * 3;
    // print header
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
    fptf(stderr, "shift: %ld\n", shift);
    size_t pixel = 0;
    while (!feof(image_read))
    {
        size_t blank = 0;
        blank = (header.height - line) * tan(angle);
        blank *= 3;
        // fptf(stderr, "blank: %ld\n", blank);
        for (size_t i = 0; i < blank; i++)
        {
            fputc(255, image_write);
            pixel++;
        }
        size_t col = 0;
        while (!feof(image_read))
        {

            /* code */

            uint8_t original[3] = {0};
            uint8_t modified[3] = {0};//problem with 3840

            size_t count = fread(original, 1, 3, image_read);
            if (count == 0)
            {
                continue;
            }
            for (size_t i = 0; i < count; i++)
            {
                modified[i] = original[i];
                pixel++;
                col++;
                if (col >= 3 * header.width)
                {
                    // fptf(stderr, "col: %ld\n", col);
                    // fptf(stderr, "i: %ld\n", i);
                    col = 0;
                    fwrite(modified, i + 1, 1, image_write);

                    goto blank_back;
                    // break;
                }
            }
            fwrite(modified, count, 1, image_write);
        }
    blank_back:
        line++;
        for (size_t i = 0; i < (header_write.width * 3) - (blank + header.width * 3); i++)
        {
            fputc(255, image_write);
            pixel++;
        }
        fptf(stderr,"pixel: %ld\n",pixel);
        if (line == header.height)
        {
            break;
        }
        // fptf(stderr, "Line: %ld\n", line);
    }
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