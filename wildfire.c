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

#include <stdio.h>
#include <stdlib.h>

#include <getopt.h> // processes command line arguments that begin with (-)

//#include <math.h> 

//float roundf(float x); 

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

static size_t size = DEFAULT_SIZE; // size of the grid

static float pCatch = DEFAULT_PROB_CATCH; // probability of a tree catching fire

static float density = DEFAULT_DENSITY; // density

static float pBurning = DEFAULT_BURN; // proportion of the tree population initially on fire

static float pNeighbor = DEFAULT_PROP_NEIGHBOR; // proportion of neighbors that will influence a tree catching fire

static int cycle; // cycle of simulation

//static int changes; // number of changes in most recent cycle

//static int cChanges; // cumulative number of changes of all cycles

static int totalTrees = DEFAULT_TREES; // total number of trees  

static int fireTrees = DEFAULT_FIRE; // total number of trees initially on fire

static int livingTrees = DEFAULT_LIVING; // total number of living trees

static int spaces = DEFAULT_SPACES; // total number of spaces in grid

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

/// Uses getopt() function to process command line options. 
/// @param argc length of argv
/// @param argv array of command strings
///

int main( int argc, char * argv[] ) {

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

    default: // default simulation
	fprintf( stderr, "Bad option causes failure. \n");
	break;
    }


  }

	// gets cells

	float x = (size * size) * density;
	totalTrees = (int)(x + 0.5); // rounds float to integer
	//totalTrees = roundf(totalTrees); 
	float y = totalTrees * pBurning; // represented by (*) 
	fireTrees = (int)(y + 0.5);
	//fireTrees = roundf(fireTrees);
	float z = (totalTrees - fireTrees); // represented by (Y)
	livingTrees = (int)(z + 0.5);
	//livingTrees = roundf(fireTrees);
	float s = totalTrees - (fireTrees + livingTrees); // represented by space character
	spaces = (int)(s + 0.5); 
	//spaces = roundf(spaces);

}

