#include "graphics.h"
#include "grid.h"
#include "obstacle.h"
#include "marker.h"
#include "robot.h"
#include <stdlib.h>
#include <time.h>

int main(void)
{
    setWindowSize(300, 300);
    int cellsize=30;
    generateGrid(cellsize);
    //randObstacle(0, 0);
    generateMarker();
    //delay(2000); // wait 2 seconds
    //eraseMarker();
    placeRobot(0, 0);
    findMarker();
    return 0;
}

/*commands:
gcc graphics.c coursework.c grid.c obstacle.c marker.c robot.c -o coursework
./coursework | java -jar drawapp-4.5.jar




./a.out | java -jar drawapp-4.5.jar

gcc graphics.c coursework.c
./a.out | java -jar drawapp-4.5.jar
*/
