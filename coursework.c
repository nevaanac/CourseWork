#include "graphics.h"
#include "grid.h"
#include "obstacle.h"
#include "marker.h"
#include "robot.h"
#include <stdlib.h>
#include <time.h>

int main(void)
{
    generateGrid();
    //randObstacle(0, 0);
    generateMarker();
    placeRobot(rand() % num_cols, rand() % num_rows);
    exploreAndFindMarker();
    //pickupMarker();
    return 0;
}

/*commands:
gcc graphics.c coursework.c grid.c obstacle.c marker.c robot.c -o coursework
./coursework | java -jar drawapp-4.5.jar
*/
