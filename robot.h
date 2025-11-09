#ifndef ROBOT_H
#define ROBOT_H

typedef struct {
    int centerX;
    int centerY;
    int x_triangle[3];
    int y_triangle[3];
    int direction; // 0 = up, 1 = right, 2 = down, 3 = left  
} Robot;

int size;
int drawRobot(int centerX, int centerY);
int placeRobot(int cellX, int cellY);
int turnRight();
int turnLeft();
int moveForward();
int atMarker();
//int pickupMarker();
int goToCorner();
int dropMarker();
void exploreAndFindMarker();

#endif
