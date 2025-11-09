#include "graphics.h"
#include "robot.h"
#include "marker.h"
#include "grid.h"
#include <stdio.h>
#include <stdlib.h>

#define UNKNOWN 0
#define EMPTY 1
#define OBSTACLE 2
#define MARKER 3

#define MAX_ROWS 20
#define MAX_COLS 20

int robotMemory[MAX_ROWS][MAX_COLS] = {0};

int size = 10;
Robot robot;

int drawRobot(int centerX, int centerY)
{
    foreground();
    robot.centerX = centerX;
    robot.centerY = centerY;

    int x = centerX;
    int y = centerY;

    if (robot.direction == 0) { // up
        robot.x_triangle[0] = x;
        robot.y_triangle[0] = y - size;
        robot.x_triangle[1] = x - size;
        robot.y_triangle[1] = y + size;
        robot.x_triangle[2] = x + size;
        robot.y_triangle[2] = y + size;
    } else if (robot.direction == 1) { // right
        robot.x_triangle[0] = x + size;
        robot.y_triangle[0] = y;
        robot.x_triangle[1] = x - size;
        robot.y_triangle[1] = y - size;
        robot.x_triangle[2] = x - size;
        robot.y_triangle[2] = y + size;
    } else if (robot.direction == 2) { // down
        robot.x_triangle[0] = x;
        robot.y_triangle[0] = y + size;
        robot.x_triangle[1] = x - size;
        robot.y_triangle[1] = y - size;
        robot.x_triangle[2] = x + size;
        robot.y_triangle[2] = y - size;
    } else { // left
        robot.x_triangle[0] = x - size;
        robot.y_triangle[0] = y;
        robot.x_triangle[1] = x + size;
        robot.y_triangle[1] = y - size;
        robot.x_triangle[2] = x + size;
        robot.y_triangle[2] = y + size;
    }

    setColour(blue);
    fillPolygon(3, robot.x_triangle, robot.y_triangle);
    return 0;
}

int placeRobot(int x, int y)
{ //places robot to the given CELLS (x, y)
    int centerX = x * cellsize + cellsize / 2; //centers the robot
    int centerY = y * cellsize + cellsize / 2;
    robot.direction = 0; // start facing UP
    drawRobot(centerX, centerY);
    return 0;
}

int turnRight()
{ // (0→1→2→3→0)
    //erase old robot
    setColour(white);
    fillPolygon(3, robot.x_triangle, robot.y_triangle);

    robot.direction = (robot.direction + 1) % 4;
    drawRobot(robot.centerX, robot.centerY);
    return 0;
}

int turnLeft()
{ // (0→3→2→1→0)
    //erase old robot
    setColour(white);
    fillPolygon(3, robot.x_triangle, robot.y_triangle);

    // ensure positive modulo for left rotation
    robot.direction = (robot.direction + 3) % 4;
    drawRobot(robot.centerX, robot.centerY);
    return 0;
}

int moveForward()
{
    // erase old robot
    setColour(white);
    fillPolygon(3, robot.x_triangle, robot.y_triangle);

    if (robot.direction == 0)
        robot.centerY -= cellsize; // up
    else if (robot.direction == 1)
        robot.centerX += cellsize; // right
    else if (robot.direction == 2)
        robot.centerY += cellsize; // down
    else if (robot.direction == 3)
        robot.centerX -= cellsize; // left

    drawRobot(robot.centerX, robot.centerY);
    return 0;
}

int atMarker()
{
    for (int i = 0; i < num_markers; i++) {
        if (markers[i].visible == 1) {
            int robotCellX = robot.centerX / cellsize;
            int robotCellY = robot.centerY / cellsize;
            if (robotCellX == markers[i].gridX && robotCellY == markers[i].gridY) {
                return 1;
            }
        }
    }
    return 0;
}

int goToCorner()
{ //goes to bottom right corner
    int targetX = (num_cols - 1);
    int targetY = (num_rows - 1);

    while ((robot.centerX / cellsize) != targetX || (robot.centerY / cellsize) != targetY) {
        int curX = robot.centerX / cellsize;
        int curY = robot.centerY / cellsize;

        if (curX < targetX)
            robot.direction = 1; // right
        else if (curX > targetX)
            robot.direction = 3; // left
        else if (curY < targetY)
            robot.direction = 2; // down
        else if (curY > targetY)
            robot.direction = 0; // up

        moveForward();
        sleep(200);
    }
    dropMarker();
    return 0;
}

int dropMarker()
{
    markers[0].gridX = num_cols - 1;
    markers[0].gridY = num_rows - 1;
    markers[0].x = markers[0].gridX * cellsize + cellsize / 2;
    markers[0].y = markers[0].gridY * cellsize + cellsize / 2;
    markers[0].visible = 1;

    background();
    setColour(gray);
    fillOval(markers[0].x - marker_radius, markers[0].y - marker_radius,
             marker_radius * 2, marker_radius * 2);

    grid[markers[0].gridY][markers[0].gridX] = CELL_MARKER;

    fprintf(stderr, "Marker dropped at bottom-right corner!\n");
    return 0;
}

typedef struct {
    int x, y;
} Point;

//0=up, 1=right, 2=down, 3=left
int dx[4] = {0, 1, 0, -1};
int dy[4] = {-1, 0, 1, 0};

//Check if cell is inside the grid
int isInside(int x, int y)
{
    return x >= 0 && x < num_cols && y >= 0 && y < num_rows;
}

//Move robot to (x, y) physically using turning + forward
void moveToCell(int targetX, int targetY)
{
    // Move step-by-step from current grid cell to target grid cell
    int curX = robot.centerX / cellsize;
    int curY = robot.centerY / cellsize;

    while (curX != targetX || curY != targetY) {
        int stepX = 0, stepY = 0;
        if (curX < targetX)
            stepX = 1;
        else if (curX > targetX)
            stepX = -1;
        else if (curY < targetY)
            stepY = 1;
        else if (curY > targetY)
            stepY = -1;

        int targetDir;
        if (stepX == 1)
            targetDir = 1; // right
        else if (stepX == -1)
            targetDir = 3; // left
        else if (stepY == 1)
            targetDir = 2; // down
        else
            targetDir = 0; // up (stepY == -1)

        // rotate towards targetDir
        while (robot.direction != targetDir) {
            int diff = (targetDir - robot.direction + 4) % 4;
            if (diff == 1)
                turnRight();
            else if (diff == 3)
                turnLeft();
            else {
                turnRight();
                turnRight();
            }
        }

        // move one cell in that direction
        moveForward();
        sleep(100);

        // update current cell coordinates
        curX = robot.centerX / cellsize;
        curY = robot.centerY / cellsize;
    }
}

// DFS helper: returns 1 if marker found in this subtree, 0 otherwise
static int dfsVisit(int x, int y)
{
    // move robot to this cell
    moveToCell(x, y);

    // already visited?
    if (robotMemory[y][x] != UNKNOWN)
        return 0;
    robotMemory[y][x] = EMPTY;

    // check for marker
    if (grid[y][x] == CELL_MARKER) {
        robotMemory[y][x] = MARKER;
        printf("Marker found at (%d, %d)!\n", x, y);
        eraseMarker(0);
        sleep(100);
        goToCorner();
        return 1;
    }

    // explore neighbors
    for (int dir = 0; dir < 4; dir++) {
        int nx = x + dx[dir];
        int ny = y + dy[dir];
        if (!isInside(nx, ny))
            continue;
        if (robotMemory[ny][nx] != UNKNOWN)
            continue;

        int cellContent = grid[ny][nx];
        if (cellContent == CELL_OBSTACLE) {
            robotMemory[ny][nx] = OBSTACLE;
            continue;
        }

        // recurse into neighbor
        if (dfsVisit(nx, ny))
            return 1;

        // if child didn't find marker, move back to this cell to continue
        moveToCell(x, y);
        sleep(50);
    }

    return 0;
}

// DFS Exploration (entry point)
void exploreAndFindMarker()
{
    int startX = robot.centerX / cellsize;
    int startY = robot.centerY / cellsize;

    // start DFS
    if (!dfsVisit(startX, startY)) {
        printf("No marker found.\n");
    }
}
