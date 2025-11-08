#ifndef MARKER_H
#define MARKER_H

typedef struct {
    int x;
    int y;
    int visible; // 1 if marker is drawn, 0 if collected
} Marker;

extern const int num_markers;
extern Marker markers[];
extern int marker_radius;

int generateMarker(void);
int pickupMarker(void);

#endif
