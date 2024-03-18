#include "mymaze.h"
int main()
{
    sRoom a[4]={{1,0},{2,0},{1,0},{1,0}};
    printf("--%d",a->doors);
    sPath b ;
    printf("%d\n",find_min_path(a,2,2,&b));
    printf("%d %d",b.cost,b.length);
}