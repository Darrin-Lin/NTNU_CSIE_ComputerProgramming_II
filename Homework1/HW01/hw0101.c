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
        char *compS;
        char *compC;
        switch (type)
        {
        case 0:
            return 0;
        case -1:
            printf("mystrchr\n");
            compC = mystrchr("abcdedf", 'e');
            compS = strchr("abcdedf", 'e');
            printf("%s\n", *compC == *compS ? "true" : "false");
            break;
        case 1:
            printf("mystrchr\n");
            scanf("%s", input);
            sscanf(input, "%s", inputcpy);
            char c;
            scanf(" %c", &c);
            compC = mystrchr(input, c);
            compS = strchr(inputcpy, c);
            printf("%s\n", *compC == *compS ? "true" : "false");
            for (size_t i = 0; i < strlen(input); i++)
            {
                printf("%c %c\n", compC[i], compS[i]);
            }
            break;
        case -2:
            printf("mystrrchr\n");
            compC = mystrrchr("abcdedf", 'e');
            compS = strrchr("abcdedf", 'e');
            printf("%s\n", *compC == *compS ? "true" : "false");
            break;
        case 2:
            printf("mystrrchr\n");
            scanf("%s", input);
            sscanf(input, "%s", inputcpy);
            char c2;
            scanf(" %c", &c2);
            compC = mystrrchr(input, c2);
            compS = strrchr(inputcpy, c2);
            printf("%s\n", *compC == *compS ? "true" : "false");
            for (size_t i = 0; i < strlen(compC); i++)
            {
                printf("%c %c\n", compC[i], compS[i]);
            }
            break;
        case -3:
            printf("mystrpbrk\n");
            compC = mystrpbrk("abcdedf", "ed");
            compS = strpbrk("abcdedf", "ed");
            printf("%s\n", *compC == *compS ? "true" : "false");
            break;
        case 3:
            printf("mystrpbrk\n");
            scanf("%s", input);
            sscanf(input, "%s", inputcpy);
            char c3[1000];
            scanf(" %s", c3);
            compC = mystrpbrk(input, c3);
            compS = strpbrk(inputcpy, c3);
            for (size_t i = 0; i < strlen(compC); i++)
            {
                printf("%d\n", compC[i] == compS[i]);
            }
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
            printf("mystrstr\n");
            compC = mystrstr("abcdedf", "ed");
            compS = strstr("abcdedf", "ed");
            printf("%s\n", *compC == *compS ? "true" : "false");
            break;
        case 6:
            printf("mystrstr\n");
            scanf("%s", input);
            sscanf(input, "%s", inputcpy);
            char c6[1000];
            scanf(" %s", c6);
            compC = mystrstr(input, c6);
            compS = strstr(inputcpy, c6);
            for (size_t i = 0; i < strlen(compC); i++)
            {
                printf("%d\n", compC[i] == compS[i]);
            }
            for (size_t i = 0; i < strlen(input); i++)
            {
                printf("%d\n", compC[i] == compS[i]);
            }

            break;

        case -7:
            printf("mystrtok\n");
            printf("%s\n", mystrtok("abcdedf", "ed") == strtok("abcdedf", "ed") ? "true" : "false");
            break;
        case 7:
            printf("mystrtok\n");
            scanf("%s", input);
            sscanf(input, "%s", inputcpy);
            char c7[1000];
            compC = mystrtok(input, c7);
            compS = strtok(inputcpy, c7);
            scanf(" %s", c7);
            for (size_t i = 0; i < strlen(compC); i++)
            {
                printf("%d\n", compC[i] == compS[i]);
            }
            for (size_t i = 0; i < strlen(input); i++)
            {
                printf("%d\n", compC[i] == compS[i]);
            }

            break;

        // case -1:
        //     printf("mystrchr\n");
        //     printf("%s\n", mystrchr("abcdedf", 'e') == strchr("abcdedf", 'e') ? "true" : "false");
        //     break;
        // case 1:
        //     printf("mystrchr\n");
        //     scanf("%s", input);
        //     sscanf(input, "%s", inputcpy);
        //     char c;
        //     scanf(" %c", &c);
        //     printf("%s\n", *mystrchr(input, c) == *strchr(inputcpy, c) ? "true" : "false");
        //     break;
        // case -2:
        //     printf("mystrrchr\n");
        //     printf("%s\n", mystrrchr("abcdedf", 'e') == strrchr("abcdedf", 'e') ? "true" : "false");
        //     break;
        // case 2:
        //     printf("mystrrchr\n");
        //     scanf("%s", input);
        //     sscanf(input, "%s", inputcpy);
        //     char c2;
        //     scanf(" %c", &c2);
        //     printf("%s\n", mystrrchr(input, c2) == strrchr(inputcpy, c2) ? "true" : "false");
        //     break;
        // case -3:
        //     printf("mystrpbrk\n");
        //     printf("%s\n", mystrpbrk("abcdedf", "ed") == strpbrk("abcdedf", "ed") ? "true" : "false");
        //     break;
        // case 3:
        //     printf("mystrpbrk\n");
        //     scanf("%s", input);
        //     sscanf(input, "%s", inputcpy);
        //     char c3[1000];
        //     scanf(" %s", c3);
        //     printf("%s\n", mystrpbrk(input, c3) == strpbrk(inputcpy, c3) ? "true" : "false");
        //     printf("original: %s\n", input == inputcpy ? "true" : "false");
        //     break;
        // case -4:
        //     printf("mystrspn\n");
        //     printf("%s\n", mystrspn("abcdedf", "ed") == strspn("abcdedf", "ed") ? "true" : "false");
        //     break;
        // case 4:
        //     printf("mystrspn\n");
        //     scanf("%s", input);
        //     sscanf(input, "%s", inputcpy);
        //     char c4[1000];
        //     scanf(" %s", c4);
        //     printf("%s\n", mystrspn(input, c4) == strspn(inputcpy, c4) ? "true" : "false");
        //     printf("original: %s\n", input == inputcpy ? "true" : "false");
        //     break;
        // case -5:
        //     printf("mystrcspn\n");
        //     printf("%s\n", mystrcspn("abcdedf", "ed") == strcspn("abcdedf", "ed") ? "true" : "false");
        //     break;
        // case 5:
        //     printf("mystrcspn\n");
        //     scanf("%s", input);
        //     sscanf(input, "%s", inputcpy);
        //     char c5[1000];
        //     scanf(" %s", c5);
        //     printf("%s\n", mystrcspn(input, c5) == strcspn(inputcpy, c5) ? "true" : "false");
        //     printf("original: %s\n", input == inputcpy ? "true" : "false");
        //     break;
        // case -6:
        //     printf("mystrtok\n");
        //     printf("%s\n", mystrtok("abcdedf", "ed") == strtok("abcdedf", "ed") ? "true" : "false");
        //     break;
        // case 6:
        //     printf("mystrtok\n");
        //     scanf("%s", input);
        //     sscanf(input, "%s", inputcpy);
        //     char c6[1000];
        //     scanf(" %s", c6);
        //     printf("%s\n", mystrtok(input, c6) == strtok(inputcpy, c6) ? "true" : "false");
        //     printf("original: %s\n", input == inputcpy ? "true" : "false");
        //     break;
        default:
            break;
        }
    }
}