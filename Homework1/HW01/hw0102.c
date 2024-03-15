#include "mycal.h"
int main()
{
    uint64_t pass=0;
    char * pExpr="ABC_10+-*";//error_case

    sscanf(pExpr,"%*[0-9A-F_+-*]%ln",&pass);
    if(pass!=strlen(pExpr))printf("%ld",pass);
    return 0;
}