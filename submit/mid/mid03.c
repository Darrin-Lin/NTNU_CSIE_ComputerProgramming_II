#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char file_name[100];
    printf("cover: ");
    fgets(file_name, 100, stdin);
    file_name[strlen(file_name) - 1] = '\0';
    FILE *cover = fopen(file_name, "rb");
    if (cover == NULL)
    {
        printf("No file.\n");
        return -1;
    }
    printf("x (in pixel): ");
    return 0;
}