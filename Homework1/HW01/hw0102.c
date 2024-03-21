#include "mycal.h"
int main()
{
    char *pResult = NULL;
    char input[1000];
    fgets(input, 1000, stdin);
    if(input[strlen(input)-1] == '\n')
    {
        input[strlen(input)-1] = '\0';
    }
    int32_t base;
    scanf("%d", &base);
    printf("return value: %d\n", calculate(input,base, &pResult));
    printf("%s",pResult);
    free(pResult);
    return 0;

}