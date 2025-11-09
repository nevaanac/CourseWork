# C Coursework - Robot Exploring Arena
Robot exploring an enclosed arena using a Depth-First Search.

## Description
This project simulates a **robot exploring an enclosed circular arena** using a **Depth-First Search (DFS)** algorithm. The robot is in an arena surrounded by black walls, which includes markers (gray circles) and obstacles (dark red squares). The markers and obstacles are placed inside the arena randomly at run-time.

The robot does not know where the markers, obstacles, and walls are; and aims to find them. It explores the grid **cell by cell**, moving with commands: ```moveForward(), turnLeft(), turnRight()```

At each cell, it checks whether: the square is **empty**, contains an **obstacle/wall,** or contains a **marker**. The robot stores this information in its memory. When the robot finds a marker, it **erases it** and continues exploring until the entire arena is mapped. At the end of the exploration, it **returns to its starting position** and **drops the markers** there.
If the robot cannot reach a marker because it is blocked by obstacles, it will recognize the marker as **unreachable** and display a warning such as: ```2 remaining marker(s) unreachable```.

### Depth-First Search (DFS)
DFS explores as far as possible along one **branch** (in one direction) before backtracking and exploring other branches. In this project, the robot recursively moves to one of the four neighboring cells following a fixed priority: **North → South → West → East**. The robot continues exploring from there until it encounters a wall, obstacle, or an already visited cell.

1. Starts at a random empty cell.
2. Checks if the cell contains a marker (and erases it).
3. Marks it as visited in ```robotMemory```.
4. Moves along the current branch following the priority order (North, South, West, East).
5. Returns to the previous cell after fully exploring a **branch**.

DFS is implemented to idealize **systematic exploration** and **complete coverage** of reachable space.

## Files and Descriptions
| File | Description |
|------|--------------|
| main.c  |	Main program — sets up the environment and starts the exploration |
| grid.c / grid.h |	Generates the circular grid and walls |
| obstacle.c / obstacle.h |	Places random obstacles inside the grid |
| marker.c / marker.h |	Creates, erases, and manages markers |
| robot.c / robot.h |	Controls robot movement and exploration logic |
| graphics.c / graphics.h |	Provides visualization functions |

## Execution
**Compile** ```gcc graphics.c coursework.c grid.c obstacle.c marker.c robot.c -o coursework```

**Run** ```./coursework | java -jar drawapp-4.5.jar```
