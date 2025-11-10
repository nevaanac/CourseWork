#include "graphics.h"
#include "arena.h"
#include <stdlib.h>

int num_obstacles = 0;

int randObstacle(void)
{
    num_obstacles = (num_rows * num_cols) / 10;
    if (num_obstacles <= 0)
        return 0;

    setRGBColour(100, 0, 0);
    
    for (int i = 0; i < num_obstacles; i++)
    {
        int gx, gy;
        do
        {
            gx = rand() % num_cols;
            gy = rand() % num_rows;
        } while (grid[gy][gx] != CELL_EMPTY);

        int px = gx * cellsize; //pixel coordinates
        int py = gy * cellsize;
        fillRect(px + 1, py + 1, cellsize - 2, cellsize - 2);

        grid[gy][gx] = CELL_OBSTACLE;
    }
    return 0;
}

int isObstacle(int gridX, int gridY)
{
    if (gridY < 0 || gridY >= num_rows || gridX < 0 || gridX >= num_cols)
        return 0;
    return grid[gridY][gridX] == CELL_OBSTACLE || grid[gridY][gridX] == CELL_WALL;
}