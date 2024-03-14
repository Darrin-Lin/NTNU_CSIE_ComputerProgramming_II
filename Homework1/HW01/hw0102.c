#include "mycal.h"
int main()
{
    uint64_t pass=0;
    char * pExpr="ABC_10+-*";//error_case

    sscanf(pExpr,"%*[0-9A-F_+-*]%n",&pass);
    if(pass!=strlen(pExpr))printf("%d",pass);
    return 0;
}