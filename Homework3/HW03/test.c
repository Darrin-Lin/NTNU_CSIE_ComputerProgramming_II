#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <getopt.h>

int main()
{
    int32_t fd = open("a.in", O_RDWR);
    printf("%ld\n",_SC_PAGESIZE);
    for( int i =1;i<100;i++)
    {
    int8_t *map = mmap(NULL, 2, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)i);
    if (map == MAP_FAILED)
    {
        perror("");
        printf("err\n");
        // return 0;
    }
    else{
        printf("i:%d\n",i);
        return 0;
    }
    }
    // map[0]++;
    close(fd);
}