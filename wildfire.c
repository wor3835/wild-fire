// file: wildfire.c
// implements the simulation of spreading fire. The program implements
// a combination of [Shiflet] Assignents with variations. The state of 
// the system is repeatedly computed and displayed to show the progression
// of a forest fire. Each state represents the start of a new cycle. The simulation
// is represented by a grid of cells. Cursor-control functions are used to show changes
// to the grid as the fire spreads. The optional print mode prints another grid for
// each simulation cycle.  
// author: wor3835 | wor3835@rit.edu
//

#define _BSD_SOURCE // must be set before headers 

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // memcpy

#include <getopt.h> // processes command line arguments that begin with (-) 

// default values for simulation

#define DEFAULT_BURN 0.10 // default pBurning
#define DEFAULT_PROB_CATCH 0.30 // default pCatch
#define DEFAULT_DENSITY 0.50 // default density
#define DEFAULT_PROP_NEIGHBOR 0.25 // default pNeighbor
#define DEFAULT_PRINT_COUNT 0 // print mode is turned off and overlay display mode is on
#define DEFAULT_SIZE 10 // default size

#define DEFAULT_TREES 0 // default totalTrees
#define DEFAULT_FIRE 0 // default fireTrees
#define DEFAULT_LIVING 0 // default livingTrees
#define DEFAULT_SPACES 0 // default spaces

#define DEFAULT_CYCLE 16 // default cycle 

static size_t size = DEFAULT_SIZE; // size of the grid

static float pCatch = DEFAULT_PROB_CATCH; // probability of a tree catching fire

static float density = DEFAULT_DENSITY; // density

static float pBurning = DEFAULT_BURN; // proportion of the tree population initially on fire

static float pNeighbor = DEFAULT_PROP_NEIGHBOR; // proportion of neighbors that will influence a tree catching fire

static int cycle = DEFAULT_CYCLE; // cycle of simulation

static int changes; // number of changes in most recent cycle

static int cChanges; // cumulative number of changes of all cycles

static int totalTrees = DEFAULT_TREES; // total number of trees  

static int fireTrees = DEFAULT_FIRE; // total number of trees on fire

static int livingTrees = DEFAULT_LIVING; // total number of living trees

static int spaces = DEFAULT_SPACES; // total number of spaces in grid

// function declarations //
static int applySpread(int row, int col, char copy[size][size]);
static void help();
static int update( char g[size][size] );
int main( int argc, char * argv[] );

/// help function gives instructions. Prints usage information to stderr.

static void help() {
    fprintf( stderr, "nusage: wildfire [options]\n" );
    fprintf( stderr, "By default, the simulation runs in overlay display mode.\n" );
    fprintf( stderr, "The -pN option makes the simulation run in print mode for up to N cycles.\n" );
    printf("\n");
    fprintf( stderr, "Simulation Configuration Options:\n" );
    fprintf( stderr, " -H # View simulation options and quit.\n" ); 
    fprintf( stderr, " -bN # proportion of trees that are already burning. 0 < N < 101.\n" );
    fprintf( stderr, " -cN # probability that a tree will catch fire. 0 < N < 101.\n" );
    fprintf( stderr, " -dN # density/proportion of trees in the grid. 0 < N < 101.\n" );
    fprintf( stderr, " -nN # proportion of neighbors that influence a tree catching fire. -1 < N < 101.\n" );
    fprintf( stderr, " -pN # number of cycles to print before quitting. -1 < N < ...\n" );
    fprintf( stderr, " -sN # simulation grid size. 4 < N < 41.\n" );
    printf("\n");
    printf("\n");
}

/// Modifies the grid in place. Applies spread function to 
/// each cell.
/// Changes any burning tree to a 0,1,2 or 3 to represent
/// its current cycle. These numbers are ignored when the grid
/// is printed. The function handles the 2-cycle burn for trees that burn.   
 
static int update( char g[size][size] ) {
   
    int s = size;

    char cpy[s][s];
    memcpy(cpy, g, sizeof (char) * s * s); // makes copy of 2D array 

    int r;
    int c;

    int ret; // return value of spread (1 if tree starts burning)

    for (r = 0; r < s; r++) {
        for (c = 0; c < s; c++) {    
	    if ( cpy[r][c] == 'Y' ) {
		ret = applySpread(r, c, cpy);
		if ( ret == 1 ) {
		    g[r][c] = '0'; // becomes burning in actual grid
		    changes++;
		    fireTrees++;
		    livingTrees--;
		}
	    }
	    else if ( cpy[r][c] == '*' ) {
		g[r][c] = '0';
	    }
	    else if ( cpy[r][c] == '0' ) {
		g[r][c] = '1'; // first cycle
		//changes++;
	    }
	    else if ( cpy[r][c] == '1' ) {
		g[r][c] = '2'; // second cycle 
		//changes++;
	    }
	    else if ( cpy[r][c] == '2' ) {
		g[r][c] = '.'; // third cycle
		changes++;
		totalTrees--;
		fireTrees--;
	    }
        }
    }
  return changes;
}

/// Implements the spread algorithm. Function handles 8-way connectivity of neighbors. The 2-cycle
/// burn for burning trees is handled in update. 

static int applySpread(int row, int col, char copy[size][size]) {
	
    int totalNeighbors = 0; // total neighbors of tree 
    int burningNeighbors =  0; // total tree neighbors of tree

    if ( ( row - 1 >= 0 ) ) { // north neighbor
	if ( ( copy[row - 1][col] == 'Y' || copy[row - 1][col] == '*' 
		|| copy[row - 1][col] == '0' || copy[row - 1][col] == '1' || copy[row - 1][col] == '2') ) {
	    totalNeighbors++;
	}
	if ( ( copy[row - 1][col] == '*' || copy[row - 1][col] == '0' || copy[row - 1][col] == '1'
		|| copy[row - 1][col] == '2') ) {
	    burningNeighbors++;
	}
    }

    if ( ( col + 1 < size ) ) { // east neighbor
        if ( ( copy[row][col + 1] == 'Y' || copy[row][col + 1] == '*'
                || copy[row][col + 1] == '0' || copy[row][col + 1] == '1' || copy[row][col + 1] == '2') ) {
            totalNeighbors++;
        }
        if ( ( copy[row][col + 1] == '*' || copy[row][col + 1] == '0' || copy[row][col + 1] == '1'
                || copy[row][col + 1] == '2') ) {
            burningNeighbors++;
        }
    }


    if ( ( row + 1 < size) ) { // south neighbor
        if ( ( copy[row + 1][col] == 'Y' || copy[row + 1][col] == '*'
                || copy[row + 1][col] == '0' || copy[row + 1][col] == '1' || copy[row + 1][col] == '2') ) {
            totalNeighbors++;
        }
        if ( ( copy[row + 1][col] == '*' || copy[row + 1][col] == '0' || copy[row + 1][col] == '1'
                || copy[row + 1][col] == '2') ) {
            burningNeighbors++;
        }
    }


    if ( ( col - 1 >= 0 ) ) { // west neighbor 
        if ( ( copy[row][col - 1] == 'Y' || copy[row][col - 1] == '*'
                || copy[row][col - 1] == '0' || copy[row][col - 1] == '1' || copy[row][col - 1] == '2') ) {
            totalNeighbors++;
        }
        if ( ( copy[row][col - 1] == '*' || copy[row][col - 1] == '0' || copy[row][col - 1] == '1'
                || copy[row][col - 1] == '2') ) {
            burningNeighbors++;
        }
    }


    if ( ( row - 1 >= 0 && col + 1 < size ) ) { // northeast neighbor 
	if ( ( copy[row - 1][col + 1] == 'Y' || copy[row - 1][col + 1] == '*'
                || copy[row - 1][col + 1] == '0' || copy[row - 1][col + 1] == '1' || copy[row - 1][col + 1] == '2') ) {
            totalNeighbors++;
        }
        if ( ( copy[row - 1][col + 1] == '*' || copy[row - 1][col + 1] == '0' || copy[row - 1][col + 1] == '1'
                || copy[row - 1][col + 1] == '2') ) {
            burningNeighbors++;
        }
    }


    if ( ( row + 1 < size && col + 1 < size ) ) { // southeast neighbor
	if ( ( copy[row + 1][col + 1] == 'Y' || copy[row + 1][col + 1] == '*'
                || copy[row + 1][col + 1] == '0' || copy[row + 1][col + 1] == '1' || copy[row + 1][col + 1] == '2') ) {
            totalNeighbors++;
        }
        if ( ( copy[row + 1][col + 1] == '*' || copy[row + 1][col + 1] == '0' || copy[row + 1][col + 1] == '1'
                || copy[row + 1][col + 1] == '2') ) {
            burningNeighbors++;
        }
    }


    if ( ( row + 1 < size && col - 1 >= 0 ) ) { // southwest neighbor
	if ( ( copy[row + 1][col - 1] == 'Y' || copy[row + 1][col - 1] == '*'
                || copy[row + 1][col - 1] == '0' || copy[row + 1][col - 1] == '1' || copy[row + 1][col - 1] == '2') ) {
            totalNeighbors++;
        }
        if ( ( copy[row + 1][col - 1] == '*' || copy[row + 1][col + 1] == '0' || copy[row + 1][col - 1] == '1'
                || copy[row + 1][col - 1] == '2') ) {
            burningNeighbors++;
        }
    }


    if ( ( row - 1 >= 0 && col - 1 >= 0 ) ) { // northwest neighbor 
	if ( ( copy[row - 1][col - 1] == 'Y' || copy[row - 1][col - 1] == '*'
                || copy[row - 1][col - 1] == '0' || copy[row - 1][col - 1] == '1' || copy[row - 1][col - 1] == '2') ) {
            totalNeighbors++;
        }
        if ( ( copy[row - 1][col - 1] == '*' || copy[row - 1][col - 1] == '0' || copy[row - 1][col - 1] == '1'
                || copy[row - 1][col - 1] == '2') ) {
            burningNeighbors++;
        }
    }
 
    if ( ( (float) burningNeighbors/totalNeighbors ) > pNeighbor ) { // proportion of neighbors is higher
	float rand = random() / (float) RAND_MAX;
	if ( rand < pCatch) {
	    return 1;
	}
    }

  return 0;   

}

/// Uses getopt() function to process command line options. 
/// @param argc length of argv
/// @param argv array of command strings
///

int main( int argc, char * argv[] ) {

    int seed = 41;
    srandom(seed); // seeds random number generator 

    int c;
    int opterr = 0; 

// // // // // // // // // // // // // // // // // // // // // // // // 
// 
// If -H, -b, -c, -d, -n, -p or -s are on the command line, getopt will
// process those arguments. All options except the -H option expect an 
// argument. 
//
// // // // // // // // // // // // // // // // // // // // // // // // 

  while ( (c = getopt( argc, argv, "Hb:c:d:n:p:s:") ) != -1 ) { 

    switch ( c ) {
    case 'H':
        help();
	break;

    case 'b':
	opterr = (int)strtol( optarg, NULL, 10);
	if ( 0 < opterr && opterr < 101) {
	    pBurning = (float)opterr;
	    //printf("%f\n", pBurning);
	} else {
	    fprintf( stderr, "proportion already burning. must be an integer in [1...100].\n");
	    help();
	}
	break;

    case 'c':
	opterr = (int)strtol( optarg, NULL, 10);
	if (0 <  opterr && opterr < 101) {
	    pCatch = (float)opterr;
	    //printf("%f\n", pCatch);
	} else {
	    fprintf( stderr, "probability a tree will catch fire. must be an integer in [1...100].\n");
	    help();
	}
	break;

    case 'd':
	opterr = (int)strtol( optarg, NULL, 10);
	if (0 < opterr && opterr < 101) {
	    density = (float)opterr;
	    //printf("%f\n", density);
	} else {
	    fprintf( stderr, "density of trees in the grid must be an integer in [1...100].\n");
	    help();
	}
	break;

    case 'n':
	opterr = (int)strtol( optarg, NULL, 10);
	if (-1 < opterr && opterr < 101) {
	    pNeighbor = (float)opterr;
	    //printf("%f\n", pNeighbor);
	} else {
	    fprintf( stderr, "neighbors influence catching fire must be an integer in [0...100].\n");
	    help();
	}
	break;

    case 'p':
	opterr = (int)strtol( optarg, NULL, 10);
	if (-1 < opterr) {
	    cycle = (int)opterr;
	    //printf("%d\n", cycle);
	} else {
	    fprintf( stderr, "number of cycles to print. must be an integer in [0...10000].\n");
	    help();
	}
	break;

    case 's':
	opterr = (int)strtol( optarg, NULL, 10);
	if (4 < opterr && opterr < 41) {
	    size = (size_t)opterr;
	    //printf("%zu\n", size);
	} else {
	    fprintf( stderr, "simulation grid size must be an integer in [5...40].\n");
	    help();
	}
	break;

    default: 
	fprintf( stderr, "Bad option causes failure. \n");
	break;
    }


  }

	// gets cells

	float x = (size * size) * density;
	totalTrees = (int)(x + 0.5); // rounds float to integer
	//printf("%d\n", totalTrees);  
	float y = totalTrees * pBurning; // represented by (*) 
	fireTrees = (int)(y + 0.5);
	//printf("%d\n", fireTrees);
	float z = (totalTrees - fireTrees); // represented by (Y)
	livingTrees = (int)(z + 0.5);
	//printf("%d\n", livingTrees);
	float s = (size * size) - totalTrees; // represented by space character
	spaces = (int)(s + 0.5);
	//printf("%d\n", spaces);

	// temp variables 

	int tempfT = fireTrees;
	int templT = livingTrees;
	int temps = spaces;

	//  

	char grid [size][size]; // grid is represented as a 2D array

	int i;
	int j;
	int b = 1; // boolean 
	
	for (i = 0; i < size; i++) { // builds cycle0 randomly
	    for (j = 0; j < size; j++) {
	      while(b) {
		int r = random() % 3;
		if (r == 0 && fireTrees > 0) {
		    fireTrees--;
		    grid[i][j] = '*';
		    printf("%c", grid[i][j]);
		    break;
		} else if (r == 1 && livingTrees > 0) {
		    livingTrees--;
		    grid[i][j] = 'Y';
		    printf("%c", grid[i][j]);
		    break;
		} else if (r == 2 && spaces > 0) {
		    spaces--;
		    grid[i][j] = ' ';
		    printf("%c", grid[i][j]);
		    break; 
		}

	     }

	    }

	printf("\n");

	}
	
	// reassignment 

	fireTrees = tempfT;
	livingTrees = templT;
	spaces = temps;

	//

// // // // // // // // // // // // // // // // // // // // // // // // 
// 
// The Simulation Loop:
// The loop continually applies the update algorithm and checks if
// all fires are out or the number of cycles has been reached. 
//
// // // // // // // // // // // // // // // // // // // // // // // // 

  int newC; // changes per cycle
  int currCycle = 0; // current cycle of simulation

  while(fireTrees >= 0 && cycle >= 0) {
	newC = update(grid);
	cChanges += newC;
	changes = 0;

	for (i = 0; i < size; i++) { 
            for (j = 0; j < size; j++) {
		printf("%c", grid[i][j]);
	    }
	  printf("\n");
	}
	//printf("%d\n", newC);
	//printf("%d\n", currCycle);
	newC = 0;
	currCycle++;
	cycle--;
  }

return(EXIT_SUCCESS);

}
