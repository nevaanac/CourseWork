#include "graphics.h"
#include "grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int generateObstacle(int x, int y) //coordinates of the left corner (pixels)
{
    fillRect(x, y, cellsize, cellsize); //(x, y, width, height)
    return 0;
}

typedef struct{
        int gridX;
        int gridY;
} Obstacle;

const int num_obstacles = 20;
Obstacle obstacles[num_obstacles];

int randObstacle(){
    // place obstacles randomly inside generated grid
    int i;
    for (i=0; i<num_obstacles; i++){
        int gx = rand() % num_cols;
        int gy = rand() % num_rows;
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
    return grid[gridY][gridX] == CELL_OBSTACLE;
}