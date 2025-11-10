#ifndef ROBOT_H
#define ROBOT_H

typedef struct
{
    int centerX;
    int centerY;
    int x_triangle[3];
    int y_triangle[3];
    int direction; // 0 = up, 1 = right, 2 = down, 3 = left
} Robot;

typedef struct
{
    int x, y;
} Point;

// 0=up, 1=right, 2=down, 3=left
extern int dx[4];
extern int dy[4];

int size;
int drawRobot(int centerX, int centerY);
int placeRobot();
int turnRight();
int turnLeft();
int moveForward();
int atMarker();
int goToCorner();
int dropMarker();
int isInside(int x, int y);
void moveToCell(int targetX, int targetY);
void exploreAndFindMarker();

#endif
