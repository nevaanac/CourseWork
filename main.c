#include "graphics.h"
#include "arena.h"
#include "obstacle.h"
#include "marker.h"
#include "robot.h"
#include <time.h>
#include <stdlib.h>

int main(void)
{
    srand(time(NULL));
    background();
    generateArena();
    randObstacle();
    generateMarker();

    foreground();
    placeRobot();
    exploreAndFindMarker();
    return 0;
}