#include "mymaze.h"
int main()
{
    int32_t row = 0, col = 0;
    scanf("%d", &row);
    scanf("%d", &col);
    sRoom inp[row][col];
    for (int32_t i = 0; i < row; i++)
    {
        for (int32_t j = 0; j < col; j++)
        {
            scanf("%u", &inp[i][j].cost);
            // input doors in four sides
            int32_t up, right, down, left;
            scanf("%d %d %d %d", &up, &right, &down, &left);
            inp[i][j].doors = (up << 6) | (right << 4) | (down << 2) | left;
        }
    }
    sPath result;
    result.pPath = NULL;
    result.length = 0;
    result.cost = 0;
    printf("return: %d\n",find_min_path(&(inp[0][0]),row,col,&result));
    printf("cost:%d length:%d\n\n",result.cost,result.length);
    for(uint32_t i=0;i<result.length;i++)
    {
        printf("%d %d\n",result.pPath[i].row,result.pPath[i].col);
    }
    free(result.pPath);
}