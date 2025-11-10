#ifndef MARKER_H
#define MARKER_H

typedef struct Marker
{
    int x, y;         // pixel position
    int gridX, gridY; // grid position
    int visible;      // 1 if marker is visible, 0 if collected
} Marker;

extern int num_markers;
extern Marker *markers;
extern int marker_radius;

int generateMarker(void);
int eraseMarker(int y, int x);

#endif
