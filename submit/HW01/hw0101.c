#include "mystring.h"
#include <string.h>
int main()
{
    // 1: mystrchr 2: mystrrchr 3: mystrpbrk 4: mystrspn 5: mystrcspn 6: mystrstr 7: mystrtok
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
            printf("%s\n", strcmp(compC,compS)==0 ? "true" : "false");
            break;
        case 1:
            printf("mystrchr\n");
            scanf("%s", input);
            sscanf(input, "%s", inputcpy);
            char c;
            scanf("%c", &c);
            compC = mystrchr(input, c);
            compS = strchr(inputcpy, c);
            if (compC == NULL && compS == NULL)
            {
                printf("true\n");
                break;
            }
            printf("%s\n", *compC == *compS ? "true" : "false");
            for (size_t i = 0; i < strlen(input); i++)
            {
                printf("%d", compC[i] == compS[i]);
            }
            printf("\n");
            break;
        case -2:
            printf("mystrrchr\n");
            compC = mystrrchr("abcdedf", 'e');
            compS = strrchr("abcdedf", 'e');
            printf("%s\n", strcmp(compC,compS) ==0? "true" : "false");
            break;
        case 2:
            printf("mystrrchr\n");
            scanf("%s", input);
            sscanf(input, "%s", inputcpy);
            char c2;
            scanf("%c", &c2);
            compC = mystrrchr(input, c2);
            compS = strrchr(inputcpy, c2);
            if (compC == NULL && compS == NULL)
            {
                printf("true\n");
                break;
            }
            printf("%s\n", *compC == *compS ? "true" : "false");
            for (size_t i = 0; i < strlen(compC); i++)
            {
                printf("%d", compC[i] == compS[i]);
            }
            printf("\n");
            break;
        case -3:
            printf("mystrpbrk\n");
            compC = mystrpbrk("abcdedf", "ed");
            compS = strpbrk("abcdedf", "ed");
            printf("%s\n", strcmp(compC,compS)==0 ? "true" : "false");
            break;
        case 3:
            printf("mystrpbrk\n");
            scanf("%s", input);
            sscanf(input, "%s", inputcpy);
            char c3[1000];
            scanf("%s", c3);
            compC = mystrpbrk(input, c3);
            compS = strpbrk(inputcpy, c3);
            for (size_t i = 0; i < strlen(compC); i++)
            {
                printf("%d", compC[i] == compS[i]);
            }
            printf("\n");
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
            scanf("%s", c4);
            printf("%s\n", mystrspn(input, c4) == strspn(inputcpy, c4) ? "true" : "false");
            printf("original: %s\n", strcmp(input, inputcpy) == 0 ? "true" : "false");
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
            printf("original: %s\n", strcmp(input, inputcpy) == 0 ? "true" : "false");
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
                printf("%d", compC[i] == compS[i]);
                printf("\n");
            }
            for (size_t i = 0; i < strlen(input); i++)
            {
                printf("%d", compC[i] == compS[i]);
            }
            printf("\n");

            break;

        case -7:
            printf("mystrtok\n");
            printf("%s\n", strcmp(mystrtok("abcdedf", "ed"), strtok("abcdedf", "ed")) == 0 ? "true" : "false");
            break;
        case 7:
            printf("mystrtok\n");
            scanf("%s", input);
            sscanf(input, "%s", inputcpy);
            char c7[1000];
            scanf("%s", c7);
            compC = mystrtok(input, c7);
            compS = strtok(inputcpy, c7);

            for (size_t i = 0; i < strlen(compC); i++)
            {
                printf("%d", compC[i] == compS[i]);
            }
            printf("\n");
            for (size_t i = 0; i < strlen(input); i++)
            {
                printf("%d", compC[i] == compS[i]);
            }
            printf("\n");

            break;
        default:
            break;
        }
    }
}