// Samuel Clarke - z5045865
// Wed13-Sitar
// 16/04/2016
// Pixel colour mapper (mandelbrot)

#include <math.h>
#include "pixelColor.h"

unsigned char stepsToRed (int steps) {
    // Falling natural log curve 
    unsigned char red;
    if( steps < 256 ){
        red = -27*log(steps)+95;
    } else {
        red = 255;
    }
    return red;
    //return steps < 256 ? (-27*log(steps)+95) : 255;
}

unsigned char stepsToBlue (int steps){
    unsigned char blue;
    if( steps > 30 && steps < 200 ){
        blue = 0;
    } else {
        blue = -27*log(steps)+95;
    }
    return blue;
    //return steps > 30 && steps < 200 ? 0 : (-27*log(steps)+95);
}

unsigned char stepsToGreen (int steps){
    unsigned char green;
    if( steps < 256 ){
        green = steps;
    } else {
        green = 255;
    }
    return green;
    //return steps < 256 ? steps : 255;
}