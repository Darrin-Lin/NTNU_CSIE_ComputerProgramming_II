#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdint.h>

typedef struct _sRoom
{
    uint32_t cost;
    uint8_t doors; // 8 bits: aabbccdd
    // aa: the up door number 0-3
    // bb: the right door number 0-3
    // cc: the down number 0-3
    // dd: the left door number 0-3
} sRoom;

typedef struct _sPoint
{
    uint32_t row;
    uint32_t col;
} sPoint;

typedef struct _sPath
{
    uint32_t length; // Path length.
    uint32_t cost;   // Cost
    sPoint *pPath;   // An array of all points in order.
} sPath;

// The start point is pMaze[0][0] and the exit point is pMaze[row -1][col -1]
// If there is no path , return 0; If there is any errors in inputs , return -1; otherwise, return 1;
int32_t find_min_path(const sRoom *pMaze, const uint8_t row, const uint8_t col, sPath *pMinPath);

static int32_t find_path(uint8_t x, uint8_t y, int32_t *steps, const sRoom *pMaze, const uint8_t row, const uint8_t col);

static int32_t find_path(uint8_t x, uint8_t y, int32_t *steps, const sRoom *pMaze, const uint8_t row, const uint8_t col)
{
    if (x < 0 || x >= row || y < 0 || y >= col)
    {
        return -1;
    }
    if (*(steps + x * col + y) != 0)
    {
        return *(steps + x * col + y);
    }
    if (x == 0 && y == 0)
    {
        *(steps + x * col + y) = (pMaze + x * col + y)->cost;
        return *(steps + x * col + y);
    }
    int32_t up = 0, down = 0, left = 0, right = 0;
    if (y - 1 > 0 && (((pMaze + x * col + y)->doors & 0b11000000) >> 6) == (((pMaze + x * col + y - 1)->doors & 0b00001100) >> 2))
    {
        up = find_path(x, y - 1, steps, pMaze, row, col);
    }
    if (x + 1 < col && (((pMaze + x * col + y)->doors & 0b00110000) >> 4) == (((pMaze + (x + 1) * col + y)->doors & 0b00000011)))
    {
        right = find_path(x, y + 1, steps, pMaze, row, col);
    }
    if (y + 1 < row && (((pMaze + x * col + y)->doors & 0b00001100) >> 2) == (((pMaze + x * col + y + 1)->doors & 0b11000000) >> 6))
    {
        down = find_path(x + 1, y, steps, pMaze, row, col);
    }
    if (x - 1 > 0 && (((pMaze + x * col + y)->doors & 0b00000011)) == (((pMaze + (x - 1) * col + y)->doors & 0b00110000) >> 4))
    {
        left = find_path(x - 1, y, steps, pMaze, row, col);
    }
    if (up == 0 && down == 0 && left == 0 && right == 0)
    {
        *(steps + x * col + y) = -1;
        return -1;
    }
    int32_t min = up;
    if (down != 0 && (min == 0 || down < min)&&down!=-1)
    {
        min = down;
    }
    if (left != 0 && (min == 0 || left < min)&&left!=-1)
    {
        min = left;
    }
    if (right != 0 && (min == 0 || right < min)&&right!=-1)
    {
        min = right;
    }
    if(min==-1){
        *(steps + x * col + y) = -1;
        return -1;
    }
    *(steps + x * col + y) = min + (pMaze + x * col + y)->cost;
    return min + (pMaze + x * col + y)->cost;
}
int32_t find_min_path(const sRoom *pMaze, const uint8_t row, const uint8_t col, sPath *pMinPath)
{
    if (pMaze == NULL || pMinPath == NULL || row == 0 || col == 0)
    {
        return -1;
    }
    int32_t steps[row][col];
    for (uint8_t i = 0; i < row; i++)
    {
        for (uint8_t j = 0; j < col; j++)
        {
            steps[i][j] = 0;
        }
    }
    int64_t cost = find_path(row - 1, col - 1, steps, pMaze, row, col);
    if (cost == -1)
    {
        return 0;
    }
    
}