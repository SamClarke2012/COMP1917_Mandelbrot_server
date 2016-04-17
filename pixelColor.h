// Samuel Clarke - z5045865
// Wed13-Sitar
// 16/04/2016
// Pixel colour mapper (mandelbrot)

#ifndef PIXELCOLOR_H
#define PIXELCOLOR_H

#define RED_COS_PERIOD 81
#define BLU_COS_PERIOD 60
#define GRN_COS_PERIOD 40
#define COL_COS_BIAS   128
#define COL_COS_AMPLITUDE (-128)

static unsigned char stepsToRed (int steps);
static unsigned char stepsToBlue (int steps);
static unsigned char stepsToGreen (int steps);

#endif
