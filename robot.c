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

//0=up, 1=right, 2=down, 3=left
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

int placeRobot()
{ //places robot to the given CELLS (x, y)
    int x=rand() % num_cols;
    int y=rand() % num_rows;
    while(grid[y][x] != CELL_EMPTY){
        x=rand() % num_cols;
        y=rand() % num_rows;
    }
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
    sleep(100);
    return 0;
}

int turnLeft()
{ // (0→3→2→1→0)
    //erase old robot
    setColour(white);
    fillPolygon(3, robot.x_triangle, robot.y_triangle);

    robot.direction = (robot.direction + 3) % 4;
    drawRobot(robot.centerX, robot.centerY);
    sleep(100);
    return 0;
}

int moveForward()
{ // erase old robot
    setColour(white);
    fillPolygon(3, robot.x_triangle, robot.y_triangle);
    /*
    setColour(lightgray); //see visited cells --> not a must but i found it quite useful
    fillRect(
        robot.centerX - size,
        robot.centerY - size,
        size * 2,
        size * 2
    );
    */

    if (robot.direction == 0)
        robot.centerY -= cellsize; // up
    else if (robot.direction == 1)
        robot.centerX += cellsize; // right
    else if (robot.direction == 2)
        robot.centerY += cellsize; // down
    else if (robot.direction == 3)
        robot.centerX -= cellsize; // left

    drawRobot(robot.centerX, robot.centerY);
    sleep(100);
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

int findPathToCorner(int targetX, int targetY, Point path[], int* pathLen)
{
    int startX = robot.centerX / cellsize;
    int startY = robot.centerY / cellsize;

    // Queue for BFS
    Point queue[MAX_ROWS * MAX_COLS];
    int front = 0, rear = 0;

    // For reconstructing path
    Point parent[MAX_ROWS][MAX_COLS];
    int visited[MAX_ROWS][MAX_COLS] = {0};

    // Start BFS from current position
    queue[rear++] = (Point){startX, startY};
    visited[startY][startX] = 1;

    int foundPath = 0;
    while (front < rear && !foundPath) {
        Point cur = queue[front++];

        // Try all 4 directions
        for (int dir = 0; dir < 4; dir++) {
            int nx = cur.x + dx[dir];
            int ny = cur.y + dy[dir];

            if (!isInside(nx, ny) || visited[ny][nx])
                continue;

            // Skip obstacles or walls
            if (grid[ny][nx] == CELL_OBSTACLE || grid[ny][nx] == CELL_WALL)
                continue;

            // Record how we got to this cell
            parent[ny][nx] = cur;
            visited[ny][nx] = 1;

            // Add to queue
            queue[rear++] = (Point){nx, ny};

            // If we reached target
            if (nx == targetX && ny == targetY) {
                foundPath = 1;
                break;
            }
        }
    }

    if (!foundPath) {
        return -1;
    }

    // Reconstruct path from target back to start
    *pathLen = 0;

    // Work backwards from target
    Point cur = {targetX, targetY};
    while (cur.x != startX || cur.y != startY) {
        path[*pathLen] = cur;
        (*pathLen)++;
        cur = parent[cur.y][cur.x];
    }

    return 0;
}

/* THIS WAS IMPLEMENTED FOR STAGE 4 OF THE PROJECT
int goToCorner()
{
    // Try each corner in turn
    Point corners[4] = {
        {0, 0},             // Top-left
        {num_cols - 1, 0},  // Top-right
        {0, num_rows - 1},  // Bottom-left
        {num_cols - 1, num_rows - 1}  // Bottom-right
    };
    char* cornerNames[4] = {
        "top-left",
        "top-right", 
        "bottom-left",
        "bottom-right"
    };
    
    Point path[MAX_ROWS * MAX_COLS];
    int pathLen;

    //try each corner until a reachable one is found
    for (int i = 0; i < 4; i++) {
        if (findPathToCorner(corners[i].x, corners[i].y, path, &pathLen) == 0) {
            fprintf(stderr, "Found path to %s corner\n", cornerNames[i]);
            
            // follow path (in reverse bc we built it backwards)
            for (int j = pathLen - 1; j >= 0; j--) {
                moveToCell(path[j].x, path[j].y);
            }
            dropMarker();
            return 0;
        }
        fprintf(stderr, "No path to %s corner\n", cornerNames[i]);
    }
    fprintf(stderr, "No reachable corners found!\n");
    return -1;
} */

int dropMarker()
{
    int gridX = robot.centerX / cellsize;  //current robot grid position
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
    //foreground();

    grid[gridY][gridX] = CELL_MARKER;
    return 0;
}

//Check if cell is inside the grid
int isInside(int x, int y) {
    return x >= 0 && x < num_cols && y >= 0 && y < num_rows;
}

//Move robot to (x, y)
void moveToCell(int targetX, int targetY)
{
    int curX = robot.centerX / cellsize; //current grid cell
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
        moveForward();
        // update current cell coordinates
        curX = robot.centerX / cellsize;
        curY = robot.centerY / cellsize;
    }
}

/* REFERENCE: The DFS functions below are ** using CoPilot
based on previous code structure and user instructions. */

// DFS traversal: returns number of markers found (and erased) in this subtree
static int dfsVisit(int x, int y)
{
    moveToCell(x, y);
    // already visited?
    if (robotMemory[y][x] != UNKNOWN)
        return 0;
    robotMemory[y][x] = EMPTY;

    int found = 0;
    // check for marker
    if (grid[y][x] == CELL_MARKER) {
        robotMemory[y][x] = MARKER;
        printf("Marker found at (%d, %d)!\n", x, y);
        eraseMarker(x, y);
        sleep(50);
        found = 1;
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
        if (cellContent == CELL_OBSTACLE || cellContent == CELL_WALL) {
            robotMemory[ny][nx] = OBSTACLE;
            continue;
        }

        // recursively traverse neighbors
        found += dfsVisit(nx, ny);

        // after exploring child, move back to this cell to continue
        moveToCell(x, y);
    }
    return found;
}

// DFS Exploration: visit all reachable cells, erase markers, return to corner
void exploreAndFindMarker()
{ //set robot memory to unknown
    for (int r = 0; r < num_rows && r < MAX_ROWS; r++) {
        for (int c = 0; c < num_cols && c < MAX_COLS; c++) {
            robotMemory[r][c] = UNKNOWN;
        }
    }

    int startX = robot.centerX / cellsize;
    int startY = robot.centerY / cellsize;

    int totalFound = dfsVisit(startX, startY);
    if (totalFound == 0) {
        printf("No markers found.\n");
    } else {
        printf("Erased %d marker(s).\n", totalFound);
    }

    // check for any unreachable, remaining markers
    int remaining = 0;
    for (int r = 0; r < num_rows; r++) {
        for (int c = 0; c < num_cols; c++) {
            if (grid[r][c] == CELL_MARKER)
                remaining++;
        }
    }
    if (remaining > 0) {
        fprintf(stderr, "%d remaining marker(s) unreachable.\n", remaining);
    }

    // return to start position and drop a marker there
    moveToCell(startX, startY);
    dropMarker();
}