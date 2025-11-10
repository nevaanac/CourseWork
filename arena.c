#include "graphics.h"
#include "arena.h"
#include <stdlib.h>

int **grid;
int num_rows;
int num_cols;
int cellsize = 30;

int generateArena(void)
{
    generateGrid();
    generateWalls();
    return 0;
}

int generateGrid(void)
{
    num_rows = 21;
    num_cols = 21;

    grid = malloc(num_rows * sizeof(int *));
    for (int i = 0; i < num_rows; i++)
    {
        grid[i] = malloc(num_cols * sizeof(int));
        for (int j = 0; j < num_cols; j++)
        {
            grid[i][j] = CELL_EMPTY;
        }
    }
    int gridsizeX = cellsize * num_cols;
    int gridsizeY = cellsize * num_rows;
    setWindowSize(gridsizeX + 1, gridsizeY + 1);

    clear();
    setColour(black);
    for (int i = cellsize; i < gridsizeX; i += cellsize)
    {
        drawLine(i, 0, i, gridsizeY);
    }
    for (int j = cellsize; j < gridsizeY; j += cellsize)
    {
        drawLine(0, j, gridsizeX, j);
    }

    return 0;
}

int generateWalls(void)
{
    setColour(black);
    double centerX = num_cols / 2.0;
    double centerY = num_rows / 2.0;
    double radius = (num_rows - 3) / 2.0; // radius in grid cells

    double innerRadius = radius - 1.0;
    double innerSq = innerRadius * innerRadius;

    for (int row = 0; row < num_rows; row++)
    {
        for (int col = 0; col < num_cols; col++)
        {
            double dx = col - centerX;
            double dy = row - centerY;
            double distSq = dx * dx + dy * dy;

            if (distSq <= innerSq)
            {
                continue;
            }
            else
            {
                int px = col * cellsize;
                int py = row * cellsize;
                fillRect(px + 1, py + 1, cellsize - 2, cellsize - 2);
                grid[row][col] = CELL_WALL;
            }
        }
    }
    return 0;
}