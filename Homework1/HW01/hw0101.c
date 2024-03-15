#include "mystring.h"
#include <string.h>
int main()
{
    
    while (1)
    {
        int type = 0;
        char input[1000];
        char inputcpy[1000];
        scanf("%d", &type);
        switch (type)
        {
        case 0:
            return 0;
        case -1:
            printf("mystrchr\n");
            printf("%s\n", mystrchr("abcdedf", 'e') == strchr("abcdedf", 'e') ? "true" : "false");
            break;
        case 1:
            printf("mystrchr\n");
            scanf("%s", input);
            sscanf(input, "%s", inputcpy);
            char c;
            scanf(" %c", &c);
            printf("%s\n", mystrchr(input, c) == strchr(inputcpy, c) ? "true" : "false");
            break;
        case -2:
            printf("mystrrchr\n");
            printf("%s\n", mystrrchr("abcdedf", 'e') == strrchr("abcdedf", 'e') ? "true" : "false");
            break;
        case 2:
            printf("mystrrchr\n");
            scanf("%s", input);
            sscanf(input, "%s", inputcpy);
            char c2;
            scanf(" %c", &c2);
            printf("%s\n", mystrrchr(input, c2) == strrchr(inputcpy, c2) ? "true" : "false");
            break;
        case -3:
            printf("mystrpbrk\n");
            printf("%s\n", mystrpbrk("abcdedf", "ed") == strpbrk("abcdedf", "ed") ? "true" : "false");
            break;
        case 3:
            printf("mystrpbrk\n");
            scanf("%s", input);
            sscanf(input, "%s", inputcpy);
            char c3[1000];
            scanf(" %s", c3);
            printf("%s\n", mystrpbrk(input, c3) == strpbrk(inputcpy, c3) ? "true" : "false");
            printf("original: %s\n", input == inputcpy ? "true" : "false");
            break;
        case -4:
            printf("mystrspn\n");
            printf("%s\n", mystrspn("abcdedf", "ed") == strspn("abcdedf", "ed") ? "true" : "false");
            break;
        case 4:
            printf("mystrspn\n");
            scanf("%s", input);
            sscanf(input, "%s", inputcpy);
            char c4[1000];
            scanf(" %s", c4);
            printf("%s\n", mystrspn(input, c4) == strspn(inputcpy, c4) ? "true" : "false");
            printf("original: %s\n", input == inputcpy ? "true" : "false");
            break;
        case -5:
            printf("mystrcspn\n");
            printf("%s\n", mystrcspn("abcdedf", "ed") == strcspn("abcdedf", "ed") ? "true" : "false");
            break;
        case 5:
            printf("mystrcspn\n");
            scanf("%s", input);
            sscanf(input, "%s", inputcpy);
            char c5[1000];
            scanf(" %s", c5);
            printf("%s\n", mystrcspn(input, c5) == strcspn(inputcpy, c5) ? "true" : "false");
            printf("original: %s\n", input == inputcpy ? "true" : "false");
            break;
        case -6:
            printf("mystrtok\n");
            printf("%s\n", mystrtok("abcdedf", "ed") == strtok("abcdedf", "ed") ? "true" : "false");
            break;
        case 6:
            printf("mystrtok\n");
            scanf("%s", input);
            sscanf(input, "%s", inputcpy);
            char c6[1000];
            scanf(" %s", c6);
            printf("%s\n", mystrtok(input, c6) == strtok(inputcpy, c6) ? "true" : "false");
            printf("original: %s\n", input == inputcpy ? "true" : "false");
            break;
        default:
            break;
        }
    }
}