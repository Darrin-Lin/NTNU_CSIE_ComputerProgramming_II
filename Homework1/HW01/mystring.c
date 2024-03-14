#include <stdint.h>
#include <stdio.h>


char *mystrchr(const char *s, int c);
char *mystrrchr(const char *s, int c);
size_t mystrspn(const char *s, const char *accept);
size_t mystrcspn(const char *s, const char *reject);
char *mystrpbrk(const char *s, const char *accept);
char *mystrstr(const char *haystack, const char *needle);
char *mystrtok(char *str, const char *delim);

char *mystrchr(const char *s, int c)
{
    while (*s != '\0')
    {
        if (*s == c)
        {
            return (char *)s;
        }
        s++;
    }
    return NULL;
}
char *mystrrchr(const char *s, int c)
{
    char *last = NULL;
    while (*s != '\0')
    {
        if (*s == c)
        {
            last = (char *)s;
        }
        s++;
    }
    return last;
}
size_t mystrspn(const char *s, const char *accept)
{
    size_t count = 0;
    while (*s != '\0')
    {
        if (mystrchr(accept, *s) == NULL)
        {
            return count;
        }
        count++;
        s++;
    }
    return count;
}
size_t mystrcspn(const char *s, const char *reject)
{
    size_t count = 0;
    while (*s != '\0')
    {
        if (mystrchr(reject, *s) != NULL)
        {
            return count;
        }
        count++;
        s++;
    }
    return count;
}

char *mystrpbrk(const char *s, const char *accept)
{
    while (*s != '\0')
    {
        if (mystrchr(accept, *s) != NULL)
        {
            return (char *)s;
        }
        s++;
    }
    return NULL;
}
char *mystrstr(const char *haystack, const char *needle)
{
    size_t needle_len = strlen(needle);
    while (*haystack != '\0')
    {
        if (strncmp(haystack, needle, needle_len) == 0)
        {
            return (char *)haystack;
        }
        haystack++;
    }
    return NULL;
}
char *mystrtok(char *str, const char *delim)
{
    static char *last = NULL;
    if (str != NULL)
    {
        last = str;
    }
    if (last == NULL)
    {
        return NULL;
    }
    char *start = last;
    while (*last != '\0')
    {
        if (mystrchr(delim, *last) != NULL)
        {
            *last = '\0';
            last++;
            while(*last != '\0' && mystrchr(delim, *last) != NULL)
            {
                last++;
            }
            return start;
        }
        last++;
    }
    last = NULL;
    return start;
}
