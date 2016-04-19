#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "pixelColor.h"

int main( int argc, char **argv ){
    int i;
    printf("Testing pixel colour functions...\n");
    // Test range of codomain
    for( i=0; i<=256; i++ ) {
        // Is [0,255]
        assert( (stepsToRed(i) >= 0 && stepsToRed(i) <= 255) );
        assert( (stepsToGreen(i) >= 0 && stepsToGreen(i) <= 255) );
        assert( (stepsToBlue(i) >= 0 && stepsToBlue(i) <= 255) );
    }
    printf("All tests passed!\n");
    return EXIT_SUCCESS;
}