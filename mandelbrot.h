// Samuel Clarke - z5045865
// Wed13-Sitar
// 16/04/2016
// Mandelbrot functions header.

#ifndef MANDELBROT_H
#define MANDELBROT_H

#define TARGET_DISTANCE 2
#define MAX_ESCAPE_ITERATIONS 256

typedef struct ComplexNumber {
   double real;
   double imaginary;
} complexNumber;

static complexNumber multiplyComplex( complexNumber a, complexNumber b );
static complexNumber addComplex( complexNumber a, complexNumber b );
static double distanceFromOrigin( complexNumber point );
int escapeSteps (double x, double y);
static void testEscapeSteps(void);

#endif