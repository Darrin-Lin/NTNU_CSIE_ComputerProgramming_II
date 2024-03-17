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

static int32_t find_path(uint8_t x, uint8_t y, int64_t *steps, const sRoom *pMaze, const uint8_t row, const uint8_t col);


int32_t find_min_path(const sRoom *pMaze, const uint8_t row, const uint8_t col, sPath *pMinPath)
{
    if (pMaze == NULL || pMinPath == NULL || row == 0 || col == 0)
    {
        return -1;
    }
    int64_t steps[row][col];
    for(uint8_t i = 0; i < row; i++)
    {
        for(uint8_t j = 0; j < col; j++)
        {
            steps[i][j] = (pMaze + i * col + j)->cost;
        }
    }

}