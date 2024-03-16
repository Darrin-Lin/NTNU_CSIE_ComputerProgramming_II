#include "mycal.h"
int main()
{
    char *pResult = NULL;
    printf("%d\n",calculate("1_10 + 2_11 * 1AA_11", 10, &pResult));
    printf("%s",pResult);
    free(pResult);
    return 0;

}