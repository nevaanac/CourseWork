#include "graphics.h"
#include "arena.h"
#include "marker.h"
#include <stdio.h>
#include <stdlib.h>

int num_markers = 0;
Marker *markers = NULL;
int marker_radius = 10;

int generateMarker(void)
{
    num_markers = (num_rows * num_cols) / 10;
    if (num_markers < 1)
        num_markers = 1;

    markers = malloc(num_markers * sizeof(Marker));
    if (!markers)
    {
        fprintf(stderr, "Failed to allocate markers array (%d)\n", num_markers);
        return -1;
    }

    const int centerOffset = cellsize / 2;
    setColour(gray);

    for (int i = 0; i < num_markers; i++)
    {
        Marker *currentMarker = &markers[i];
        do
        {
            currentMarker->gridX = rand() % num_cols;
            currentMarker->gridY = rand() % num_rows;
        } while (grid[currentMarker->gridY][currentMarker->gridX] != CELL_EMPTY);

        currentMarker->x = currentMarker->gridX * cellsize + centerOffset;
        currentMarker->y = currentMarker->gridY * cellsize + centerOffset;
        currentMarker->visible = 1;

        fillOval(currentMarker->x - marker_radius,
                 currentMarker->y - marker_radius,
                 marker_radius * 2, marker_radius * 2);
        grid[currentMarker->gridY][currentMarker->gridX] = CELL_MARKER;
    }
    return 0;
}

int eraseMarker(int x, int y)
{
    foreground();
    setColour(white);
    fillRect(x * cellsize, y * cellsize, cellsize, cellsize);
    setColour(black);
    drawRect(x * cellsize, y * cellsize, cellsize, cellsize);

    grid[y][x] = CELL_EMPTY;

    for (int i = 0; i < num_markers; i++)
    {
        if (markers[i].visible && markers[i].gridX == x && markers[i].gridY == y)
        {
            markers[i].visible = 0;
            return 0;
        }
    }
    return 0;
}