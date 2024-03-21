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
    if(s==NULL)
        return NULL;
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
    if (s == NULL)
        return NULL;
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
    if (s == NULL)
        return -1;
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
    if (s == NULL)
        return -1;
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
    if (s == NULL)
        return NULL;
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
    if (haystack == NULL || needle == NULL)
        return NULL;
    size_t needle_len = 0;
    const char *pStr_cp = needle;
    while (*pStr_cp != '\0')
    {
        pStr_cp++;
        needle_len++;
    }
    while (*haystack != '\0')
    {
        // pStr_cp=haystack;
        size_t count = 0;
        for (size_t i = 0; i <= needle_len && *(haystack + i) != '\0'; i++)
        {
            if (*(haystack + i) == *(needle + i))
                count++;
            else
                break;
        }
        if (count == needle_len)
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
    while (*start != '\0')
    {
        if (mystrchr(delim, *start) == NULL)
        {

            while (*(start) != '\0' && mystrchr(delim, *(start)) != NULL)
                start++;
            
            last = start;
            while (*(last) != '\0'  && mystrchr(delim, *(last)) == NULL)
                last++;
            if (*last != '\0')
            {
                *(last) = '\0';
                last++;
            }
            else
                last = NULL;
                
            return start;
        }
        start++;
    }
    last = start;
    return NULL;
    
}
