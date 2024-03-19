#include "mymaze.h"
int main()
{
    sRoom a[4]={{1,0},{2,0},{1,0},{1,0}};
    sPath b ;
    printf("return: %d\n",find_min_path(a,2,2,&b));
    printf("cost:%d length:%d\n\n",b.cost,b.length);
    for(uint32_t i=0;i<b.length;i++)
    {
        printf("%d %d\n",b.pPath[i].row,b.pPath[i].col);
    }
    free(b.pPath);
}   