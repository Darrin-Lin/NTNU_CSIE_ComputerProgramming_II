#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main()
{
    FILE *fp = popen("cat *.c", "r");
    if (fp == NULL)
    {
        fprintf(stderr, "popen() failed\n");
        return 1;
    }
    char tmp[1024];
    while (fgets(tmp, sizeof(tmp), fp) != NULL)
    {
        printf("%s", tmp);
    }
    return 0;
}