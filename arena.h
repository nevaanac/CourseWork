#ifndef GRID_H
#define GRID_H

extern int cellsize;
extern int **grid;
extern int num_rows;
extern int num_cols;

int generateArena(void);
int generateGrid(void);
int generateWalls(void);

#define CELL_EMPTY 1
#define CELL_OBSTACLE 2
#define CELL_MARKER 3
#define CELL_WALL 4

#endif
