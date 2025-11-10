#include "graphics.h"
#include "grid.h"
#include "obstacle.h"
#include "marker.h"
#include "robot.h"
#include <stdlib.h>
#include <time.h>

int main(void)
{
    background();
    generateGrid();
    randObstacle();
    generateMarker();

    foreground();
    placeRobot();
    exploreAndFindMarker();
    return 0;
}

/*commands:
gcc graphics.c main.c grid.c obstacle.c marker.c robot.c -o coursework
./coursework | java -jar drawapp-4.5.jar
*/
