#include "mystring.h"
#include <string.h>
int main()
{

    char c[] = "123--23-52-5-35-25-45-45-23-5-235-24-5-5--436-3--------346-3-46-4-66-7-574-----6-2-652-5-25--24-6-34-6-3-63-6-2-64-246-52-2-624-6-26-2-6-26-24---2-6-4-62-6-24-634-64-36-45-64-64-56-4-64-64-6-46-46-4-64-64-64-64-";
    char *a = "1-";
    char C[] = "123--23-52-5-35-25-45-45-23-5-235-24-5-5--436-3--------346-3-46-4-66-7-574-----6-2-652-5-25--24-6-34-6-3-63-6-2-64-246-52-2-624-6-26-2-6-26-24---2-6-4-62-6-24-634-64-36-45-64-64-56-4-64-64-6-46-46-4-64-64-64-64-";

    char *token = mystrtok(c, a);
    char *Token = strtok(C,a);
    printf("token %s",token);
    // for (int i = 0; i < 212; i++)
    //     printf("%c", *(c + i));
    printf("\n");
    while (token != NULL)
    {
        // printf("%s\n", token);
        for (int i = 0; i < 212; i++)
            printf("%d", *(c + i)==*(C + i)?1:*(c+i));
            printf("\n");   
            for (int i = 0; i < 212; i++)
            printf("%c", *(C + i));
            printf("\n");
            for (int i = 0; i < 212; i++)
            printf("%c", *(c + i));
            printf("\n");
            Token=strtok(NULL,a);
        token = mystrtok(NULL, a);
        // printf("\n");
    }
    printf("\n");
}