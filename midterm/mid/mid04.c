#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "mybmp.h"

int main()
{
    char file_name[100];
    printf("Please enter the file name: ");
    fgets(file_name, 100, stdin);
    file_name[strlen(file_name) - 1] = '\0';
    FILE *file = fopen(file_name, "rb");
    if (file == NULL)
    {
        printf("No file.\n");
        return -1;
    }
    return 0;
}