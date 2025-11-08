#include "graphics.h"
#include "grid.h"

int generateGrid(int cellsize)
{
    //int cellwidth=30; int cellheight=30;
    //int cellsize = 30;
    background();
    int cellnums=10;
    int gridsize = cellsize * cellnums; // 300

    setColour(red);
    drawRect(0, 0, gridsize, gridsize); // border around the arena --> adjust thickness!!
    setColour(black);

    // Vertical lines
    for (int i = cellsize; i < gridsize; i += cellsize) {
        drawLine(i, 0, i, gridsize);
    }

    // Horizontal lines
    for (int j = cellsize; j < gridsize; j += cellsize) {
        drawLine(0, j, gridsize, j);
    }
    return 0;
}