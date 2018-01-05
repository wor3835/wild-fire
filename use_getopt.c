// file: use_getopt.c
// use_getopt.c demonstrates use of the getopt command line processing tool.
// author: bksteele
//

#include <stdio.h>
#include <stdlib.h>

#include <getopt.h>  // processing for "-fN" command line args


#define DEFAULT_SIZE   10            ///< used if no override on command line


static size_t the_size = DEFAULT_SIZE; ///< a program setting.

static size_t the_count = 0;         ///< another program setting. default is 0.

/// usage function gives instructions.

static void usage() {
    fprintf( stderr, "\nusage: use_getopt [-H -pNUM -s pos_num]\n" );
    fprintf( stderr, "\t-H is for getting help\n" ); 
    fprintf( stderr, "\t-pN overrides the default count\n" ); 
    fprintf( stderr, "\t-sN overrides the default size\n" ); 
    fprintf( stderr, "\nAn argument pair of \"-p 5\" is same as \"-p5\"\n" ); 
}

/// This demonstrates the use of the getopt() function.
/// main : [-H] [-p num] [-s pos_num] [-- ... ] -> int
/// To terminate option flag processing enter the "--" as an argument.
/// @param argc the length of the command line arguments array
/// @param argv the array of command line argument strings
///
/// @see man -s 3 getopt
/// @see man getopt
///

int main( int argc, char * argv[] ) {

    int opt;
    int tmpsize = 0;

    fprintf( stderr, "command line:\t >>> " );
    for ( int j = 0; j < argc; ++j ) {
        fprintf( stderr, "%s ", argv[j] );
    }
    fprintf( stderr, "\n" );

    usage();  // print usage to guide the demonstration

    // // // // // // // // // // // // // // // // // // // // // // // // 
    // string "Hs:p:" means that, if -H -s or -p are on the command line,
    // then getopt code will process those arguments, ignoring others.
    // The -H option (case sensitive) has no argument.
    // The -s and -p options each expect an argument.
    // // // // // // // // // // // // // // // // // // // // // // // // 

    while ( (opt = getopt( argc, argv, "Hs:p:") ) != -1 ) {

        switch ( opt ) {
        case 'H':
            fprintf( stderr, "\nPlease read the usage message above.\n" );
            break;

        case 'p':
            tmpsize = (int)strtol( optarg, NULL, 10 );
            if ( tmpsize > 0 ) {
                the_count = (size_t)tmpsize;
                printf( "Option %c got the value %d\n", opt, tmpsize );
            } else {
                fprintf( stderr, "The -p got no value; should be an error.\n" );
            }
            break;

        case 's':
            tmpsize = (int)strtol( optarg, NULL, 10 );
            if ( tmpsize > 0 ) {
                the_size = (size_t)tmpsize;
                printf( "Option %c got the value %d\n", opt, tmpsize );
                break;
            } else {
                fprintf( stderr, "The -s option requires positive value\n" );
                return EXIT_FAILURE;
            }

        default:
            // some unknown, possibly unacceptable option flag
            fprintf( stderr, "Bad option causes failure; ignored.\n" );
            break;
        }
    }

    printf( "\nsettings: " );
    printf( "the_size == %zu;\n", the_size );
    printf( "the_count == %zu;\n", the_count );
    printf( "\n" );

    // // // // // // // // // // // // // // // // // // // // // // // // 
    // this example shows the remaining command line arguments.
    // At this point, optind should equal index of next argument.
    // if there are additional arguments, optind is index of the first one.
    // // // // // // // // // // // // // // // // // // // // // // // // 

    printf( "\n\tREMAINING COMMAND LINE ARGUMENTS: (index: argv[index])\n" );
    for ( int j = optind; j < argc; ++j ) {
        printf( "\t(%d: %s)\n", j, argv[j] );
    }
    printf( "\n" );

    return EXIT_SUCCESS;
}

