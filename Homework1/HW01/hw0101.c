#include "mystring.h"
#include <string.h>
int main()
{
    char s[] = "Hello, world!";
    char sub[] = "wor";
    printf("strchr: %s\n", strchr(s, 'w')); // "world!"
    printf("mystrchr: %s\n", mystrchr(s, 'w')); // "world!"
    printf("strrchr: %s\n", strrchr(s, 'o')); // "orld!"
    printf("mystrrchr: %s\n", mystrrchr(s, 'o')); // "orld!"
    printf("strspn: %zu\n", strspn(s, "Helo, wrd")); // 7
    printf("mystrspn: %zu\n", mystrspn(s, "Helo, wrd")); // 7
    printf("strcspn: %zu\n", strcspn(s, "Helo, wrd")); // 0
    printf("mystrcspn: %zu\n", mystrcspn(s, "Helo, wrd")); // 0
    printf("strpbrk: %s\n", strpbrk(s, "Helo, wrd")); // "ello, world!"
    printf("mystrpbrk: %s\n", mystrpbrk(s, "Helo, wrd")); // "ello, world!"
    printf("strstr: %s\n", strstr(s, sub)); // "world!"
    printf("mystrstr: %s\n", mystrstr(s, sub)); // "world!"
    char st[] = "Hello, world!";
    
    printf("strtok: %s, %s\n", strtok(st, ", "),st); // "Hello"
    printf("mystrtok: %s, %s\n", mystrtok(s, ", "),s); // "Hello"
    printf("strtok: %s\n", strtok("-----6", "1-")); // "world!"
    printf("mystrtok: %s\n", mystrtok("-----6", "1-")); // "world!" 
    return 0;
}