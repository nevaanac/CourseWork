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
            grid[i][j] = CELL_EMPTY;  // mark empty cells consistently
        }
    }
    background();
    int gridsizeX = cellsize * num_cols;
    int gridsizeY = cellsize * num_rows;
    setWindowSize(gridsizeX+1, gridsizeY+1); //+10 for border
    
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
    fprintf(stderr, "Grid generated: %d rows × %d cols\n", num_rows, num_cols);
    return 0;
}