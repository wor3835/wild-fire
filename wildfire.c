// file: wildfire.c
// implements the simulation of spreading fire.
// author: wor3835
//

#include <stdio.h>
#include <stdlib.h>

#include <getopt.h> // processes command line arguments that begin with (-)

// default values for simulation

#define DEF_S 10 // default size
#define DEF_CATCH 0.30 // default pCatch
#define DEF_D 0.50 // default density
#define DEF_BURN 0.10 // default pBurning
#define DEF_N 0.25 // default pNeighbor
#define DEF_CYCLE 16 // default cycle

static size_t size = DEF_S; // size of the grid

static float pCatch; // probability of a tree catching fire

static float density; // density

static float pBurning; // proportion of the tree population initially on fire

static float pNeighbor; // proportion of neighbors that will influence a tree catching fire

static int cycle; // cycle of simulation

//static int changes; // number of changes in most recent cycle

//static int cChanges; // cumulative number of changes of all cycles 

/// help function gives instructions. Prints usage information to stderr.

static void help() {
    fprintf( stderr, "\nusage: wildfire [options]\n" );
    fprintf( stderr, "\tBy default, the simulation runs in overlay display mode.\n" );
    fprintf( stderr, "\tThe -pN option makes the simulation run in print mode for up to N cycles.\n" );
    printf("\n");
    fprintf( stderr, "\tSimulation Configuration Options:\n" );
    fprintf( stderr, "\t-H # View simulation options and quit.\n" ); 
    fprintf( stderr, "\t-bN # proportion of trees that are already burning. 0 < N < 101.\n" );
    fprintf( stderr, "\t-cN # probability that a tree will catch fire. 0 < N < 101.\n" );
    fprintf( stderr, "\t-dN # density/proportion of trees in the grid. 0 < N < 101.\n" );
    fprintf( stderr, "\t-nN # proportion of neighbors that influence a tree catching fire. -1 < N < 101.\n" );
    fprintf( stderr, "\t-pN # number of cycles to print before quitting. -1 < N < ...\n" );
    fprintf( stderr, "\n-sN # simulation grid size. 4 < N < 41.\n" );
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
// Need to finish
// 
// 
// // // // // // // // // // // // // // // // // // // // // // // // 

  while ( (c = getopt( argc, argv, "Hbcdnps:") ) != -1 ) { // not sure if string is right

    switch ( c ) {
    case 'H':
        help();
	break;

    case 'b':
	opterr = (int)strtol( optarg, NULL, 10);
	if ( 0 < opterr && opterr < 101) {
	    pBurning = (float)opterr;
	} else {
	    fprintf( stderr, "proportion already burning. must be an integer in [1...100].\n");
	    help();
	}
	break;

    case 'c':
	opterr = (int)strtol( optarg, NULL, 10);
	if (0 < opterr && opterr < 101) {
	    pCatch = (float)opterr;
	} else {
	    fprintf( stderr, "probability a tree will catch fire. must be an integer in [1...100].\n");
	    help();
	}
	break;

    case 'd':
	opterr = (int)strtol( optarg, NULL, 10);
	if (0 < opterr && opterr < 101) {
	    density = (float)opterr;
	} else {
	    fprintf( stderr, "density of trees in the grid must be an integer in [1...100].\n");
	    help();
	}
	break;

    case 'n':
	opterr = (int)strtol( optarg, NULL, 10);
	if (-1 < opterr && opterr < 101) {
	    pNeighbor = (float)opterr;
	} else {
	    fprintf( stderr, "neighbors influence catching fire must be an integer in [0...100].\n");
	    help();
	}
	break;

    case 'p':
	opterr = (int)strtol( optarg, NULL, 10);
	if (-1 < opterr) {
	    cycle = (int)opterr;
	} else {
	    fprintf( stderr, "number of cycles to print. must be an integer in [0...10000].\n");
	    help();
	}
	break;

    case 's':
	opterr = (int)strtol( optarg, NULL, 10);
	if (4 < opterr && opterr < 41) {
	    size = (size_t)opterr;
	    printf("%zu\n", size);
	} else {
	    fprintf( stderr, "simulation grid size must be an integer in [5...40].\n");
	    help();
	}
	break;

    default: // default simulation
	size = DEF_S; 
	pCatch = DEF_CATCH;
	density = DEF_D;
	pBurning = DEF_BURN;
	pNeighbor = DEF_N;
	cycle = DEF_CYCLE;
	break;
    }

  }

}

