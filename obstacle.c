#include "graphics.h"
#include "grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int generateObstacle(int x, int y) //coordinates of the left corner (pixels)
{
    setColour(black);
    fillRect(x + 1, y + 1, cellsize - 2, cellsize - 2); //(x, y, width, height)
    return 0;
}

typedef struct{
    int gridX;
    int gridY;
} Obstacle;

int num_obstacles = 0;
Obstacle *obstacles = NULL;

int randObstacle(){
    // place obstacles randomly inside generated grid
    num_obstacles = (num_rows * num_cols) / 10; //10% of grid cells
    if (num_obstacles <= 0) return 0;

    obstacles = malloc(num_obstacles * sizeof(Obstacle));
    if (!obstacles) {
        fprintf(stderr, "Failed to allocate obstacles array (%d)\n", num_obstacles);
        return -1;
    }

    int i;
    for (i = 0; i < num_obstacles; i++) {
        int gx, gy;
        // pick an empty cell (don't place obstacles on walls or existing obstacles)
        do {
            gx = rand() % num_cols;
            gy = rand() % num_rows;
        } while (grid[gy][gx] != CELL_EMPTY);

        obstacles[i].gridX = gx;
        obstacles[i].gridY = gy;

        int px = gx * cellsize;
        int py = gy * cellsize;

        generateObstacle(px, py);
        // mark grid cell as obstacle
        grid[gy][gx] = CELL_OBSTACLE;
        fprintf(stderr, "Obstacle %d → grid (%d, %d)\n", i, gx, gy);
    }
    return 0;
}

int isObstacle(int gridX, int gridY){
    if (gridY < 0 || gridY >= num_rows || gridX < 0 || gridX >= num_cols) return 0;
    return grid[gridY][gridX] == CELL_OBSTACLE || grid[gridY][gridX] == CELL_WALL;
}