#include "graphics.h"
#include "grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int **grid; // pointer to 2D array
int num_rows;
int num_cols;
int cellsize = 30;

int generateGrid()
{
    srand(time(NULL));
    num_rows = 21; // Fixed size for stage 5, odd number to have a center
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

    clear(); // Clear the background
    setColour(black);
    // Vertical lines
    for (int i = cellsize; i < gridsizeX; i += cellsize)
    {
        drawLine(i, 0, i, gridsizeY);
    }

    // Horizontal lines
    for (int j = cellsize; j < gridsizeY; j += cellsize)
    {
        drawLine(0, j, gridsizeX, j);
    }

    // circular walls
    setColour(black);
    double centerX = num_cols / 2.0;
    double centerY = num_rows / 2.0;
    double radius = (num_rows - 3) / 2.0; // radius in grid cells

    double innerRadius = radius - 1.0; // interior radius (cells inside this are empty)
    double innerSq = innerRadius * innerRadius;
    double outerSq = (radius + 0.8) * (radius + 0.8); // anything beyond this is also wall

    for (int row = 0; row < num_rows; row++)
    {
        for (int col = 0; col < num_cols; col++)
        {
            double dx = col - centerX;
            double dy = row - centerY;
            double distSq = dx * dx + dy * dy;

            if (distSq <= innerSq)
            {
                // inside the playable circle - already default CELL_EMPTY
                continue;
            }
            else
            {
                // anything outside the inner circle should be a wall
                int px = col * cellsize;
                int py = row * cellsize;
                fillRect(px + 1, py + 1, cellsize - 2, cellsize - 2);
                grid[row][col] = CELL_WALL;
            }
        }
    }
    fprintf(stderr, "Grid generated: %d rows × %d cols\n", num_rows, num_cols);
    return 0;
}