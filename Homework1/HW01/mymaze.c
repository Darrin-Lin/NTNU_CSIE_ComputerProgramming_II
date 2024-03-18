#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdint.h>

#define fptf fprintf

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

static int32_t find_path(int16_t x, int16_t y, int32_t *steps, const sRoom *pMaze, const uint8_t row, const uint8_t col);
static int8_t run_path(int16_t x, int16_t y, int32_t *steps, const sRoom *pMaze, const uint8_t row, const uint8_t col, sPoint **ppPath, uint32_t *size);

static int32_t find_path(int16_t x, int16_t y, int32_t *steps, const sRoom *pMaze, const uint8_t row, const uint8_t col)
{

    if (x < 0 || x >= col || y < 0 || y >= row)
    {
        fptf(stderr, "err0");
        return -1;
    }
    if (*(steps + y * row + x) != 0)
    {
        return *(steps + y * row + x);
    }
    if (x == 0 && y == 0)
    {
        *(steps + y * row + x) = (pMaze + y * row + x)->cost;
        return *(steps + y * row + x);
    }
    int32_t up = 0, down = 0, left = 0, right = 0;
    if (y - 1 >= 0 && (((pMaze + y * row + x)->doors & (uint8_t)0b11000000) >> 6) == (((pMaze + (y - 1) * row + x)->doors & (uint8_t)0b00001100) >> 2))
    {
        fptf(stderr, "up:%d %d\n", ((pMaze + y * row + x)->doors & (uint8_t)0b11000000) >> 6), (((pMaze + (y - 1) * row + x)->doors & (uint8_t)0b00001100) >> 2);
        up = find_path(x, y - 1, steps, pMaze, row, col);
    }
    if (x + 1 < col && (((pMaze + y * row + x)->doors & (uint8_t)0b00110000) >> 4) == (((pMaze + y * row + x + 1)->doors & (uint8_t)0b00000011)))
    {
        fptf(stderr, "right:%u %u\n", type((uint8_t)((pMaze + y * row + x)->doors & (uint8_t)0b00110000) >> 4)), (((pMaze + y * row + x + 1)->doors & (uint8_t)0b00000011));
        right = find_path(x, y + 1, steps, pMaze, row, col);
    }
    if (y + 1 < row && (((pMaze + y * row + x)->doors & (uint8_t)0b00001100) >> 2) == (((pMaze + (y + 1) * row + x)->doors & (uint8_t)0b11000000) >> 6))
    {
        fptf(stderr, "down:%d %d\n", ((pMaze + y * row + x)->doors & (uint8_t)0b00001100) >> 2), (((pMaze + (y + 1) * row + x)->doors & (uint8_t)0b11000000) >> 6);
        down = find_path(x + 1, y, steps, pMaze, row, col);
    }
    if (x - 1 >= 0 && (((pMaze + y * row + x)->doors & (uint8_t)0b00000011)) == (((pMaze + y * row + x - 1)->doors & (uint8_t)0b00110000) >> 4))
    {
        fptf(stderr, "left:%d %d\n", ((pMaze + y * row + x)->doors & (uint8_t)0b00000011)), (((pMaze + y * row + x - 1)->doors & (uint8_t)0b00110000) >> 4);
        left = find_path(x - 1, y, steps, pMaze, row, col);
    }
    if (up == 0 && down == 0 && left == 0 && right == 0)
    {
        *(steps + y * row + x) = -1;
        fptf(stderr, "err2");
        return -1;
    }
    int32_t min = up;
    if (down != 0 && (min == 0 || down < min) && down != -1)
    {
        min = down;
    }
    if (left != 0 && (min == 0 || left < min) && left != -1)
    {
        min = left;
    }
    if (right != 0 && (min == 0 || right < min) && right != -1)
    {
        min = right;
    }
    if (min == -1)
    {
        *(steps + y * row + x) = -1;
        fptf(stderr, "err2");
        return -1;
    }
    if (down != min && y - 1 > 0)
    {
        *(steps + (y - 1) * row + x) = -1;
    }
    if (right != min && x + 1 < col)
    {
        *(steps + y * row + x + 1) = -1;
    }
    if (down != min && y + 1 < row)
    {
        *(steps + (y + 1) * row + x) = -1;
    }
    if (left != min && x - 1 > 0)
    {
        *(steps + y * row + x - 1) = -1;
    }
    *(steps + y * row + x) = min + (pMaze + y * row + x)->cost;
    return min + (pMaze + y * row + x)->cost;
}

static int8_t run_path(int16_t x, int16_t y, int32_t *steps, const sRoom *pMaze, const uint8_t row, const uint8_t col, sPoint **ppPath, uint32_t *size)
{
    if (x < 0 || x >= col || y < 0 || y >= row)
    {
        return -1;
    }
    if (x == col - 1 && y == row - 1)
    {
        (*size)++;
        sPoint *temp = NULL;
        temp = realloc(*ppPath, sizeof(sPoint) * (*size));
        if (temp == NULL)
            return -1;
        *ppPath = temp;
        (*ppPath + (*size) - 1)->col = x;
        (*ppPath + (*size) - 1)->row = y;

        return 0;
    }
    if (*(steps + y * row + x + 1) != -1)
    {
        (*size)++;
        sPoint *temp = NULL;
        temp = realloc(*ppPath, sizeof(sPoint) * (*size));
        if (temp == NULL)
            return -1;
        *ppPath = temp;
        (*ppPath + (*size) - 1)->col = x;
        (*ppPath + (*size) - 1)->row = y;
        (*ppPath + (*size) - 1)->col = x;
        (*ppPath + (*size) - 1)->row = y;

        return run_path(x + 1, y, steps, pMaze, row, col, ppPath, size);
    }
    else if (*(steps + (y + 1) * row + x) != -1)
    {
        (*size)++;
        sPoint *temp = NULL;
        temp = realloc(*ppPath, sizeof(sPoint) * (*size));
        if (temp == NULL)
            return -1;
        *ppPath = temp;
        (*ppPath + (*size) - 1)->col = x;
        (*ppPath + (*size) - 1)->row = y;
        (*ppPath + (*size) - 1)->col = x;
        (*ppPath + (*size) - 1)->row = y;

        return run_path(x, y + 1, steps, pMaze, row, col, ppPath, size);
    }
    else if (*(steps + (y - 1) * row + x) != -1)
    {
        (*size)++;
        sPoint *temp = NULL;
        temp = realloc(*ppPath, sizeof(sPoint) * (*size));
        if (temp == NULL)
            return -1;
        *ppPath = temp;
        (*ppPath + (*size) - 1)->col = x;
        (*ppPath + (*size) - 1)->row = y;
        (*ppPath + (*size) - 1)->col = x;
        (*ppPath + (*size) - 1)->row = y;

        return run_path(x, y - 1, steps, pMaze, row, col, ppPath, size);
    }
    else if (*(steps + y * row + x - 1) != -1)
    {
        (*size)++;
        sPoint *temp = NULL;
        temp = realloc(*ppPath, sizeof(sPoint) * (*size));
        if (temp == NULL)
            return -1;
        *ppPath = temp;
        (*ppPath + (*size) - 1)->col = x;
        (*ppPath + (*size) - 1)->row = y;
        (*ppPath + (*size) - 1)->col = x;
        (*ppPath + (*size) - 1)->row = y;

        return run_path(x - 1, y, steps, pMaze, row, col, ppPath, size);
    }
    return 0;
}

int32_t find_min_path(const sRoom *pMaze, const uint8_t row, const uint8_t col, sPath *pMinPath)
{
    if (pMaze == NULL || row == 0 || col == 0)
    {
        return -1;
    }
    int32_t steps[row * col];
    for (uint8_t i = 0; i < row; i++)
    {
        for (uint8_t j = 0; j < col; j++)
        {
            steps[i * row + j] = 0;
        }
    }
    int64_t cost = find_path(row - 1, col - 1, steps, pMaze, row, col);
    if (cost == -1)
    {
        fptf(stderr, "aaa");
        return 0;
    }
    sPoint *pPath = NULL;
    pPath = calloc(0, sizeof(sPoint));
    uint32_t size = 0;
    run_path(0, 0, steps, pMaze, row, col, &pPath, &size);
    sPoint path[size];
    for (uint32_t i = 0; i < size; i++)
    {
        path[i].col = (pPath + i)->col;
        path[i].row = (pPath + i)->row;
    }
    free(pPath);
    pMinPath->cost = cost;
    pMinPath->length = size;
    pMinPath->pPath = path;

    return 1;
}