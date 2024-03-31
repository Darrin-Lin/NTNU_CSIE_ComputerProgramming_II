#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// #include "gguf.h"

#define DEBUG 1
#define fptf fprintf

typedef struct _Sgguf_header
{
    uint32_t magic_number;
    uint32_t version;
    uint64_t tenson_count;
    uint64_t metadata_kv_count;

} __attribute__((__packed__)) Sgguf_header;

int main()
{
    FILE *gguf_read = fopen("model.gguf", "rb");
    if (gguf_read == NULL)
    {
        return -1;
    }
    Sgguf_header gguf_header;
    fread(&gguf_header, sizeof(Sgguf_header), 1, gguf_read);
    printf("GGUF: ");
    if (gguf_header.magic_number == 0x46554747)
        printf("true\n");
    else
    {
        printf("false\n");
        fclose(gguf_read);
        return -1;
    }

    printf("Metadata                                Value\n");
    printf("Version:                                %u\n", gguf_header.version);
    printf("tensor_count:                           %lu\n", gguf_header.tenson_count);
    printf("kv_count:                               %lu\n", gguf_header.metadata_kv_count);
    char title[100];
    if (DEBUG)
    {
        
    }
    fclose(gguf_read);

    return 0;
}