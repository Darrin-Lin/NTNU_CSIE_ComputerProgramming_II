#include <errno.h>
#include <stdio.h>
#include <string.h>
// define a new error number and use perror

#define MY_ERRNO 200

void perror(const char *s)
{
    if (s != NULL)
    {
        fprintf(stderr, "%s: ", s);
    }
    switch (errno)
    {
    case MY_ERRNO:
        fprintf(stderr, "MY_ERRNO\n");
        break;
    default:
        fprintf(stderr, "%s\n", strerror(errno));
        break;
    }
}

int main()
{
    for (int i = 0; i <200; i++)
    {
        errno = i;
        int errno_save = errno;
        fprintf(stderr, "errno: %d: ", errno_save);
        errno = errno_save;
        perror("");
    }
    errno = MY_ERRNO;
    perror("MY_ERRNO");
    return 0;
}