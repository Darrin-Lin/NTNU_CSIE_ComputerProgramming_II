#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef DEBUG
#define debug_print(fmt, ...)                       \
    fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
              __LINE__, __func__, ##__VA_ARGS__);
#else
#define debug_print(fmt, ...) \
    {}
#endif

int main()
{

    return 0;
}