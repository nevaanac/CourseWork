#include "graphics.h"
#include "grid.h"
#include "marker.h"
#include <stdio.h>
#include <stdlib.h>

int num_markers = 0; // will be computed after grid is generated
Marker *markers = NULL;
int marker_radius = 10;

int generateMarker() {
    // num of markers: 10% of grid cells, at least 1
    num_markers = (num_rows * num_cols) / 10;
    if (num_markers < 1) num_markers = 1;

    markers = malloc(num_markers * sizeof(Marker));
    if (!markers) {
        fprintf(stderr, "Failed to allocate markers array (%d)\n", num_markers);
        return -1;
    }

    int i;
    for (i = 0; i < num_markers; i++) {
        markers[i].gridX = rand() % num_cols;
        markers[i].gridY = rand() % num_rows;

        //find empty cell for marker
        while(grid[markers[i].gridY][markers[i].gridX] != CELL_EMPTY){
            markers[i].gridX = rand() % num_cols;
            markers[i].gridY = rand() % num_rows;
        }
        // convert grid to pixel coordinates
        markers[i].x = markers[i].gridX * cellsize + cellsize / 2;
        markers[i].y = markers[i].gridY * cellsize + cellsize / 2;
        markers[i].visible = 1;

    // draw marker on background layer
    setColour(gray);
    fillOval(markers[i].x - marker_radius, markers[i].y - marker_radius,
         marker_radius * 2, marker_radius * 2); //topleftcorner = (x-radius, y-radius)

        grid[markers[i].gridY][markers[i].gridX] = CELL_MARKER;
        fprintf(stderr, "Marker placed at grid (%d, %d) → pixels (%d, %d)\n",
                markers[i].gridY, markers[i].gridX,
                markers[i].y, markers[i].x);
        } 
    return 0;
}

int eraseMarker(int x, int y) {
    foreground();
    setColour(white); // same as grid background
    fillRect(x * cellsize, y * cellsize, cellsize, cellsize);

    // redraw grid lines for that cell on background
    setColour(black);
    drawRect(x * cellsize, y * cellsize, cellsize, cellsize);

    grid[y][x] = CELL_EMPTY; // update grid

    // mark the corresponding marker struct as invisible
    for (int i = 0; i < num_markers; i++) {
        if (markers[i].visible && markers[i].gridX == x && markers[i].gridY == y) {
            markers[i].visible = 0;
            break;
        }
    }

    foreground(); // restore foreground for robot drawing

    fprintf(stderr, "Marker erased from grid (%d, %d)\n", y, x);
    return 0;
}