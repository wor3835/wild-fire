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

#include <unistd.h> // usleep
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // memcpy, strcpy

#include <getopt.h> // processes command line arguments that begin with (-) 

#include "display.h" // display cursor

#include <limits.h> 

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

static int print = DEFAULT_PRINT_COUNT; // print mode; 1 if on, 0 if off

static int cycle = INT_MAX; // cycle of simulation

static int changes = 0; // number of changes in most recent cycle

static int cChanges = 0; // cumulative number of changes of all cycles

static int totalTrees = DEFAULT_TREES; // total number of trees  

static int fireTrees = DEFAULT_FIRE; // total number of trees on fire

static int livingTrees = DEFAULT_LIVING; // total number of living trees

static int spaces = DEFAULT_SPACES; // total number of spaces in grid

// function declarations //
static int applySpread(int row, int col, char copy[size][size]);
static void help();
static void update( char g[size][size] );
int main( int argc, char * argv[] );

/// help function gives instructions. Prints usage information to stderr.

static void help() {
    fprintf( stderr, "usage: wildfire [options]\n" );
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
    exit(0);
}

/// Modifies the grid in place. Applies spread function to 
/// each cell.
/// Changes any burning tree to a 0,1,2 or 3 to represent
/// its current cycle. These numbers are ignored when the grid
/// is printed. The function handles the 2-cycle burn for trees that burn.   
 
static void update( char g[size][size] ) {
   
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
		    //cChanges++;
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
		//cChanges++;
		totalTrees--;
		fireTrees--;
	    }
        }
    }
  //return changes;
}

/// Implements the spread algorithm. Function handles 8-way connectivity of neighbors. The 2-cycle
/// burn for burning trees is handled in update. 

static int applySpread(int row, int col, char copy[size][size]) {
	
    int totalNeighbors = 0; // total neighbors of tree 
    int burningNeighbors =  0; // total tree neighbors of tree

    int tmpsize = size;

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

    if ( ( col + 1 < tmpsize ) ) { // east neighbor
        if ( ( copy[row][col + 1] == 'Y' || copy[row][col + 1] == '*'
                || copy[row][col + 1] == '0' || copy[row][col + 1] == '1' || copy[row][col + 1] == '2') ) {
            totalNeighbors++;
        }
        if ( ( copy[row][col + 1] == '*' || copy[row][col + 1] == '0' || copy[row][col + 1] == '1'
                || copy[row][col + 1] == '2') ) {
            burningNeighbors++;
        }
    }


    if ( ( row + 1 < tmpsize) ) { // south neighbor
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


    if ( ( row - 1 >= 0 && col + 1 < tmpsize ) ) { // northeast neighbor 
	if ( ( copy[row - 1][col + 1] == 'Y' || copy[row - 1][col + 1] == '*'
                || copy[row - 1][col + 1] == '0' || copy[row - 1][col + 1] == '1' || copy[row - 1][col + 1] == '2') ) {
            totalNeighbors++;
        }
       if ( ( copy[row - 1][col + 1] == '*' || copy[row - 1][col + 1] == '0' || copy[row - 1][col + 1] == '1'
                || copy[row - 1][col + 1] == '2') ) {
            burningNeighbors++;
        }
    }


    if ( ( row + 1 < tmpsize && col + 1 < tmpsize ) ) { // southeast neighbor
	if ( ( copy[row + 1][col + 1] == 'Y' || copy[row + 1][col + 1] == '*'
                || copy[row + 1][col + 1] == '0' || copy[row + 1][col + 1] == '1' || copy[row + 1][col + 1] == '2') ) {
            totalNeighbors++;
        }
        if ( ( copy[row + 1][col + 1] == '*' || copy[row + 1][col + 1] == '0' || copy[row + 1][col + 1] == '1'
                || copy[row + 1][col + 1] == '2') ) {
            burningNeighbors++;
        }
    }


    if ( ( row + 1 < tmpsize && col - 1 >= 0 ) ) { // southwest neighbor
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

/// Shuffles grid data to initialize cycle 0
/// @param size size of grid
/// @param data array of cells in grid
/// 
static void shuffle( long size, char data[]) {
    for( int i = 0; i < size - 1; ++i) {
	unsigned long j = (i + random()) % size;
	char tmp = data[i];
	data[i] = data[j];
	data[j] = tmp;
    }
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
	    pBurning = (float)opterr/100;
	    //printf("%f\n", pBurning);
	} else {
	    fprintf( stderr, "(-bN) proportion already burning. must be an integer in [1...100].\n");
	    help();
	}
	break;

    case 'c':
	opterr = (int)strtol( optarg, NULL, 10);
	if (0 <  opterr && opterr < 101) {
	    pCatch = (float)opterr/100;
	    //printf("%f\n", pCatch);
	} else {
	    fprintf( stderr, "(-cN) probability a tree will catch fire. must be an integer in [1...100].\n");
	    help();
	}
	break;

    case 'd':
	opterr = (int)strtol( optarg, NULL, 10);
	if (0 < opterr && opterr < 101) {
	    density = (float)opterr/100;
	    //printf("%f\n", density);
	} else {
	    fprintf( stderr, "(-dN) density of trees in the grid must be an integer in [1...100].\n");
	    help();
	}
	break;

    case 'n':
	opterr = (int)strtol( optarg, NULL, 10);
	if (-1 < opterr && opterr < 101) {
	    pNeighbor = (float)opterr/100;
	    //printf("%f\n", pNeighbor);
	} else {
	    fprintf( stderr, "(-nN) neighbors influence catching fire must be an integer in [0...100].\n");
	    help();
	}
	break;

    case 'p':
	opterr = (int)strtol( optarg, NULL, 10);
	if (-1 < opterr) {
	    print = 1;
	    cycle = (int)opterr;
	    //printf("%d\n", cycle);
	} else {
	    fprintf( stderr, "(-pN) number of cycles to print. must be an integer in [0...10000].\n");
	    help();
	}
	break;

    case 's':
	opterr = (int)strtol( optarg, NULL, 10);
	if (4 < opterr && opterr < 41) {
	    size = (size_t)opterr;
	    //printf("%zu\n", size);
	} else {
	    fprintf( stderr, "(-sN) simulation grid size must be an integer in [5...40].\n");
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

	//int tempfT = fireTrees;
	//int templT = livingTrees;
	//int temps = spaces;

	size_t spots = spaces + livingTrees + fireTrees;
	char start[spots];

	memset(start, 0, spots);

	int d;
	int e;
	int f;

	size_t len;

	for (d = 0; d < spaces; d++) {
	    len = strlen(start);
	    start[len] = ' ';
	    start[len + 1] = '\0';
	    /*
	    len = strlen(start);
	    snprintf(start + len, sizeof start - len, "%c", ' ');  
	    */
	}

	for (e = 0; e < livingTrees; e++) {
	    len = strlen(start);
            start[len] = 'Y';
            start[len + 1] = '\0';
	    /*
	    len = strlen(start);
            snprintf(start + len, sizeof start - len, "%c", ' ');
	    */
	}

	for (f = 0; f < fireTrees; f++) {
	    len = strlen(start);
            start[len] = '*';
            start[len + 1] = '\0';
	    /*
	    len = strlen(start);
            snprintf(start + len, sizeof start - len, "%c", ' ');
	    */
	}

	/*
	int new;
        for (new = 0; new < spots; new++) {
            printf("%c", start[new]);
        }
	*/


	shuffle(spots, start);

	//  

	char grid [size][size]; // grid is represented as a 2D array

	int i;
	int j;
	int k = 0;
	//int b = 1; // boolean
	
	if (print == 0) {
	clear(); 
	}

	if (print == 1) {
	    printf("%s\n", "============================");
  	    printf("%s\n", "======== Wildfire ==========");
  	    printf("%s\n", "============================");
            printf("==== Print <=  %d Cycles ====\n", cycle);
            printf("%s\n", "============================");
	}

	for (i = 0; i < size; i++) {
	    for (j = 0; j < size; j++) {
	        grid[i][j] = start[k];
		if (print == 0) {
		    set_cur_pos(i, j);
		    put(grid[i][j]);
		} else {
		printf("%c", grid[i][j]);
		}
		k++;
	    }
	  //if (print == 1) {
	  printf("\n");
	  //}
	}
		
	
	/*
	for (i = 0; i < size; i++) { // builds cycle0 randomly
	    for (j = 0; j < size; j++) {
	      while(b) {
		int r = random() % 3;
		if (r == 0 && fireTrees > 0) {
		    fireTrees--;
		    grid[i][j] = '*';
		    if (print == 0) {
			set_cur_pos(i, j);
			put(grid[i][j]);
		    } else {
		    printf("%c", grid[i][j]);
		    }
		    break;
		} else if (r == 1 && livingTrees > 0) {
		    livingTrees--;
		    grid[i][j] = 'Y';
		    if (print == 0) {
                        set_cur_pos(i, j);
                        put(grid[i][j]);
                    } else {
                    printf("%c", grid[i][j]);
                    }
		    break;
		} else if (r == 2 && spaces > 0) {
		    spaces--;
		    grid[i][j] = ' ';
		    if (print == 0) {
                        set_cur_pos(i, j);
                        put(grid[i][j]);
                    } else {
                    printf("%c", grid[i][j]);
                    }
		    break; 
		}

	     }

	  }

	if( print == 1 && i != (size - 1)) {
                printf("\n");
	}

	}
	*/

	printf("\rsize %zu, pCatch %.2f, density %.2f, pBurning %.2f, pNeighbor %.2f", size, pCatch, density, pBurning, pNeighbor);
        printf("\ncycle %d, changes %d, cumulative changes %d\n ", 0, 0, 0);
	usleep(750000);

	// reassignment 

	//fireTrees = tempfT;
	//livingTrees = templT;
	//spaces = temps;

	//

// // // // // // // // // // // // // // // // // // // // // // // // 
// 
// The Simulation Loop:
// The loop continually applies the update algorithm and checks if
// all fires are out or the number of cycles has been reached. 
//
// // // // // // // // // // // // // // // // // // // // // // // // 

  //int newC; // changes per cycle
  int currCycle = 1; // current cycle of simulation 	

  while(fireTrees > 0 && cycle > 0) {
	//printf("%d", fireTrees);
	update(grid);
	cChanges += changes;
	//changes = 0;

	for (i = 0; i < size; i++) { 
            for (j = 0; j < size; j++) {
		if ( grid[i][j] == '0' || grid[i][j] == '1' || grid[i][j] == '2' ) {
		    if ( print == 0) {
		        set_cur_pos(i, j);
			put('*');
		    } else {
		    printf("%c", '*');
		    }
		} else {
		    if ( print == 0) {
		        set_cur_pos(i, j);
			put(grid[i][j]);
		    } else {
		printf("%c", grid[i][j]);
		}
	      }
	    }
	    if( print == 1 && i != (size - 1)) {
	        printf("\n");
	    }
	}
	puts(" ");
	//printf("\rsize %zu, pCatch %.2f, density %.2f, pBurning %.2f, pNeighbor %.2f", size, pCatch, density, pBurning, pNeighbor);
	//cChanges = changes;
	//printf("\ncycle %d, changes %d, cumulative changes %d", currCycle, changes, changes);
	//printf("cumulative changes %d", changes);
	//newC = 0;
	printf("\rsize %zu, pCatch %.2f, density %.2f, pBurning %.2f, pNeighbor %.2f", size, pCatch, density, pBurning, pNeighbor);
        printf("\ncycle %d, changes %d, cumulative changes %d \n", currCycle, changes, cChanges);	
	changes = 0;
	currCycle++;
	cycle--;
	usleep(750000);
  }
  if ( fireTrees == 0) {
      printf("%s\n", "\nFires are out.");
  }

return(EXIT_SUCCESS);

}
