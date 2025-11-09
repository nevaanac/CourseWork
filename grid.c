#include "graphics.h"
#include "grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int **grid; //pointer to 2D array
int num_rows;
int num_cols;
int cellsize=30;

int generateGrid()
{
    srand(time(NULL));  // seed random once 
    num_rows=(rand() % 11)+5;  // btw. 5-15
    num_cols=(rand() % 11)+5;
   
    // Dynamically allocate the grid
    grid = malloc(num_rows * sizeof(int *));
    for (int i = 0; i < num_rows; i++) {
        grid[i] = malloc(num_cols * sizeof(int));
        for (int j = 0; j < num_cols; j++) {
            grid[i][j] = CELL_EMPTY;  // mark cells as empty
        }
    }
    int gridsizeX = cellsize * num_cols;
    int gridsizeY = cellsize * num_rows;
    setWindowSize(gridsizeX+1, gridsizeY+1);
    
    clear(); // Clear the background
    setColour(red);
    drawRect(0, 0, gridsizeX, gridsizeY); // border around the arena --> adjust thickness!!
    
    setColour(black);
    // Vertical lines
    for (int i = cellsize; i < gridsizeX; i += cellsize) {
        drawLine(i, 0, i, gridsizeY);
    }

    // Horizontal lines
    for (int j = cellsize; j < gridsizeY; j += cellsize) {
        drawLine(0, j, gridsizeX, j);
    }

    // draw surrounding walls
    setRGBColour(100, 0, 0); 
    int wall_thickness = 2;

    // top wall rows
    for (int r = 0; r < wall_thickness && r < num_rows; r++) {
        for (int col = 0; col < num_cols; col++) {
            int px = col * cellsize;
            int py = r * cellsize;
            fillRect(px + 1, py + 1, cellsize - 2, cellsize - 2);
            grid[r][col] = CELL_WALL;
        }
    }

    // bottom wall rows
    for (int r = num_rows - wall_thickness; r < num_rows; r++) {
        if (r < 0) continue;
        for (int col = 0; col < num_cols; col++) {
            int px = col * cellsize;
            int py = r * cellsize;
            fillRect(px + 1, py + 1, cellsize - 2, cellsize - 2);
            grid[r][col] = CELL_WALL;
        }
    }

    // Left and right wall columns (skip those already set by top/bottom)
    for (int row = wall_thickness; row < num_rows - wall_thickness; row++) {
        for (int t = 0; t < wall_thickness; t++) {
            int colL = t;
            int colR = num_cols - 1 - t;
            int pxL = colL * cellsize;
            int pxR = colR * cellsize;
            int py = row * cellsize;
            fillRect(pxL + 1, py + 1, cellsize - 2, cellsize - 2);
            fillRect(pxR + 1, py + 1, cellsize - 2, cellsize - 2);
            grid[row][colL] = CELL_WALL;
            grid[row][colR] = CELL_WALL;
        }
    }
    fprintf(stderr, "Grid generated: %d rows × %d cols\n", num_rows, num_cols);
    return 0;
}