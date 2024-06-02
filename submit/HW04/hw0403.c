#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef DEBUG
#define debug_print(fmt, ...)                     \
    fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
            __LINE__, __func__, ##__VA_ARGS__);
#else
#define debug_print(fmt, ...) \
    {                         \
    }
#endif

int main()
{
    // char *arr[] = {"a", "b", "c", "d", "e"};
    // char **x = arr;
    // printf("%c\n", (*x)[2]) ;
    // printf("%ld\n", sizeof(arr));
    // for (char **x = arr; x < arr+(sizeof(arr)/sizeof(*arr)); x++)
    // {
    //     printf("%s", *x);
    // }
    // int i[] = {1, 2, 3, 4, 5};
    // for(char *i : arr)
    // {
    //     printf("%s", i);        
    // }   
    return 0;
}