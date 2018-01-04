# wild-fire 


compile using: gcc -std=c99 -o wildfire wildfire.c display.h display.c

run using: ./wildfire

Implements the simulation of spreading fire. The program implements

a combination of [Shiflet] Assignents with variations. The state of 

the system is repeatedly computed and displayed to show the progression

of a forest fire. Each state represents the start of a new cycle. The simulation

is represented by a grid of cells. Cursor-control functions are used to show changes

to the grid as the fire spreads. The optional print mode prints another grid for

each simulation cycle.

usage: wildfire [options]
By default, the simulation runs in overlay display mode 
The -pN option makes the simulation run in print mode for up to N cycles

Simulation Configuration Options:   
-H # View simulation options and quit.
-bN # proportion of trees that are already burning. 0 < N < 101.
-cN # probability that a tree will catch fire. 0 < N < 101.
-dN # density/proportion of trees in the grid. 0 < N < 101.
-nN # proportion of neighbors that influence a tree catching fire. -1 < N < 101.
-pN # number of cycles to print before quitting. -1 < N < ...
-sN # simulation grid size. 4 < N < 41.
