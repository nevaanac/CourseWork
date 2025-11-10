#include "graphics.h"
#include "robot.h"
#include "marker.h"
#include "arena.h"
#include <stdio.h>
#include <stdlib.h>

#define UNKNOWN 0
#define EMPTY 1
#define OBSTACLE 2
#define MARKER 3

// 0=up, 1=right, 2=down, 3=left
int dx[4] = {0, 1, 0, -1};
int dy[4] = {-1, 0, 1, 0};

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

    switch (robot.direction)
    {
    case 0: // up
        robot.x_triangle[0] = x;
        robot.y_triangle[0] = y - size;

        robot.x_triangle[1] = x - size;
        robot.y_triangle[1] = y + size;

        robot.x_triangle[2] = x + size;
        robot.y_triangle[2] = y + size;
        break;
    case 1: // right
        robot.x_triangle[0] = x + size;
        robot.y_triangle[0] = y;

        robot.x_triangle[1] = x - size;
        robot.y_triangle[1] = y - size;

        robot.x_triangle[2] = x - size;
        robot.y_triangle[2] = y + size;
        break;
    case 2: // down
        robot.x_triangle[0] = x;
        robot.y_triangle[0] = y + size;

        robot.x_triangle[1] = x - size;
        robot.y_triangle[1] = y - size;

        robot.x_triangle[2] = x + size;
        robot.y_triangle[2] = y - size;
        break;
    default: // left (3)
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
int placeRobot(void)
{
    int x = rand() % num_cols;
    int y = rand() % num_rows;
    while (grid[y][x] != CELL_EMPTY)
    {
        x = rand() % num_cols;
        y = rand() % num_rows;
    }
    int centerX = x * cellsize + cellsize / 2;
    int centerY = y * cellsize + cellsize / 2;
    robot.direction = 0;
    drawRobot(centerX, centerY);
    return 0;
}
int eraseRobot(void)
{
    setColour(white);
    fillPolygon(3, robot.x_triangle, robot.y_triangle);
    return 0;
}

int turnRight(void)
{
    eraseRobot();
    robot.direction = (robot.direction + 1) % 4; //(0→1→2→3→0)
    drawRobot(robot.centerX, robot.centerY);
    sleep(100);
    return 0;
}

int turnLeft(void)
{
    eraseRobot();
    robot.direction = (robot.direction + 3) % 4; // (0→3→2→1→0)
    drawRobot(robot.centerX, robot.centerY);
    sleep(100);
    return 0;
}

int moveForward(void)
{
    eraseRobot();
    robot.centerX += dx[robot.direction] * cellsize;
    robot.centerY += dy[robot.direction] * cellsize;

    drawRobot(robot.centerX, robot.centerY);
    sleep(100);
    return 0;
}

int atMarker(void)
{
    int robotCellX = robot.centerX / cellsize;
    int robotCellY = robot.centerY / cellsize;

    for (int i = 0; i < num_markers; i++)
    {
        if (markers[i].visible == 1 &&
            robotCellX == markers[i].gridX &&
            robotCellY == markers[i].gridY)
        {
            return 1;
        }
    }
    return 0;
}

int dropMarker(void)
{
    int gridX = robot.centerX / cellsize;
    int gridY = robot.centerY / cellsize;

    markers[0].gridX = gridX;
    markers[0].gridY = gridY;
    markers[0].x = robot.centerX;
    markers[0].y = robot.centerY;
    markers[0].visible = 1;

    foreground();
    setColour(gray);
    fillOval(markers[0].x - marker_radius, markers[0].y - marker_radius,
             marker_radius * 2, marker_radius * 2);

    grid[gridY][gridX] = CELL_MARKER;
    return 0;
}

int isInside(int x, int y)
{
    return x >= 0 && x < num_cols && y >= 0 && y < num_rows;
}

void moveToCell(int targetX, int targetY)
{
    int curX = robot.centerX / cellsize;
    int curY = robot.centerY / cellsize;
    int steps = 0;
    const int maxSteps = (num_rows * num_cols) * 4;

    while (curX != targetX || curY != targetY)
    {
        if (steps++ > maxSteps)
        {
            fprintf(stderr, "moveToCell stuck: from (%d,%d) to (%d,%d) after %d steps\n", curX, curY, targetX, targetY, steps);
            fflush(stderr);
            return; // give up to avoid crash!
        }
        int targetDir;
        if (curX != targetX)
        {
            targetDir = (curX < targetX) ? 1 : 3; // right : left
        }
        else
        {
            targetDir = (curY < targetY) ? 2 : 0; // down : up
        }

        while (robot.direction != targetDir)
        {
            int diff = (targetDir - robot.direction + 4) % 4;
            if (diff == 1 || diff == 2)
                turnRight();
            else
                turnLeft();
        }
        moveForward();
        curX = robot.centerX / cellsize;
        curY = robot.centerY / cellsize;
    }
}

/* REFERENCE: The DFS functions below are modified using CoPilot
based on previous code structure and my instructions. */

// returns number of markers found (and erased) in this subtree
static int dfsVisit(int x, int y)
{
    fflush(stderr);
    moveToCell(x, y);
    if (robotMemory[y][x] != UNKNOWN)
        return 0;
    robotMemory[y][x] = EMPTY;

    int found = 0;
    if (grid[y][x] == CELL_MARKER)
    {
        robotMemory[y][x] = MARKER;
        eraseMarker(x, y);
        sleep(50);
        found = 1;
    }

    // explore neighbors
    for (int dir = 0; dir < 4; dir++)
    {
        int nx = x + dx[dir];
        int ny = y + dy[dir];
        if (!isInside(nx, ny))
            continue;
        if (robotMemory[ny][nx] != UNKNOWN)
            continue;

        int cellContent = grid[ny][nx];
        if (cellContent == CELL_OBSTACLE || cellContent == CELL_WALL)
        {
            robotMemory[ny][nx] = OBSTACLE;
            continue;
        }

        found += dfsVisit(nx, ny);
        moveToCell(x, y);
    }
    fflush(stderr);
    return found;
}

// DFS Exploration: visit all reachable cells, erase markers, return to corner
void exploreAndFindMarker(void)
{
    for (int r = 0; r < num_rows && r < MAX_ROWS; r++)
    {
        for (int c = 0; c < num_cols && c < MAX_COLS; c++)
        {
            robotMemory[r][c] = UNKNOWN;
        }
    }
    int startX = robot.centerX / cellsize;
    int startY = robot.centerY / cellsize;

    dfsVisit(startX, startY);

    int remaining = 0;
    for (int i = 0; i < num_markers; i++)
    {
        if (markers[i].visible)
        {
            remaining++;
        }
    }
    if (remaining > 0)
    {
        fprintf(stderr, "%d remaining marker(s) unreachable.\n", remaining);
    }
    moveToCell(startX, startY);
    dropMarker();
}