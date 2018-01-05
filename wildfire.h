/*
 * File:    wildfire.h
 *
 * Author:  William Raffaelle
 *
 * Date: 8 July 2017
 *
 * Description:
 *      Implements the simulation of spreading fire. The program implements
 * a combination of [Shiflet] Assignents with variations. The state of 
 * the system is repeatedly computed and displayed to show the progression
 * of a forest fire. Each state represents the start of a new cycle. The simulation
 * is represented by a grid of cells. Cursor-control functions are used to show changes
 * to the grid as the fire spreads. The optional print mode prints another grid for
 * each simulation cycle.
 *
 */

 #ifndef wildfire
 #define wildfire

/// Help function gives instructions. Prints usage information to stderr.
/// Terminates program when finished. 
/// 
static void help();

/// Modifies the grid in place. Applies spread function to 
/// each cell.
/// Changes any burning tree to a 0,1,2 or 3 to represent
/// its current cycle. These numbers are ignored when the grid
/// is printed. The function handles the 2-cycle burn for trees that burn. 
///
/// @param g: grid to be updated
///
static void update( char g[size][size] );

/// Implements the spread algorithm. Function handles 8-way connectivity of neighbors.
/// The 2-cycle burn for burning trees is handled in update.
///
/// @param row: the destination row
/// @param col: the destination column
/// @param copy: copy of grid that doesn't change
/// 
static int applySpread(int row, int col, char copy[size][size]);

/// Shuffles grid data to initialize cycle 0. Taken from lecture. 
///
/// @param size: size of data
/// @param data: 1d array of empty, tree, and burning characters
/// 
static void shuffle( long size, char data[]);

/// Uses getopt() function to process command line options. Runs the simulation until
/// all fires are out or max number of cycles is reached.
///
/// @param size: size of data
/// @param data: 1d array of empty, tree, and burning characters
/// 
int main( int argc, char * argv[] );

#endif
