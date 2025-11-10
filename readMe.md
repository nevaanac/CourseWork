# C Coursework - Robot Exploring Arena
Robot exploring an enclosed arena using a Depth-First Search.

## Description
This project simulates a **robot exploring an enclosed circular arena** using a **Depth-First Search (DFS)** algorithm. The robot is in an arena surrounded by black walls, which includes markers (gray circles) and obstacles (dark red squares). The markers and obstacles are placed inside the arena randomly at run-time.

The robot does not know where the markers, obstacles, and walls are; and aims to find them. It explores the grid **cell by cell**, moving with commands: ```moveForward(), turnLeft(), turnRight()```

At each cell, it checks whether: the square is **empty**, contains an **obstacle/wall,** or contains a **marker**. The robot stores this information in its memory. When the robot finds a marker, it **erases it** and continues exploring until the entire arena is mapped. At the end of the exploration, it **returns to its starting position** and **drops the markers** there.
If the robot cannot reach a marker because it is blocked by obstacles, it will recognize the marker as **unreachable** and display a warning such as: ```2 remaining marker(s) unreachable```.

### Example Arena
<img src="example_arena.png" alt="Robot exploring arena" height="300">

### Program Termination
Robot at its start position, dropping all the markers it has collected.

<img src="program_termination.png" alt="Program Termination" height="300">

### Search Algorithm
Depth-First Search (DFS) explores as far as possible along one **branch** (in one direction) before backtracking and exploring other branches. In this project, the robot recursively moves to one of the four neighboring cells following a **fixed priority**: North → South → West → East. The robot continues exploring from there until it encounters a wall, obstacle, or an already visited cell.

DFS is implemented for **systematic exploration** and **complete coverage** of reachable space.

## Files and Descriptions
| File | Description |
|------|--------------|
| main.c  |	Main program — sets up the environment and starts the exploration |
| arena.c / arena.h |	Generates the grid and circular arena |
| obstacle.c / obstacle.h |	Places random obstacles inside the grid |
| marker.c / marker.h |	Creates, erases, and manages markers |
| robot.c / robot.h |	Controls robot movement and exploration logic |
| graphics.c / graphics.h |	Provides visualization functions |

## Execution
**Compile** ```gcc graphics.c coursework.c arena.c obstacle.c marker.c robot.c -o coursework```

**Run** ```./coursework | java -jar drawapp-4.5.jar```
