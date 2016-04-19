// Samuel Clarke - z5045865
// Wed13-Sitar
// 16/04/2016
// Mandelbrot functions.


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "pixelColor.h"
#include "mandelbrot.h"

/* ONLY HERE BECAUSE I'M NOT ALOWED TO CHANGE THE .H FILES */
#define TARGET_DISTANCE 2
#define MAX_ESCAPE_ITERATIONS 256

typedef struct ComplexNumber {
   double real;
   double imaginary;
} complexNumber;

static complexNumber multiplyComplex( complexNumber a, complexNumber b );
static complexNumber addComplex( complexNumber a, complexNumber b );
static double distanceFromOrigin( complexNumber point );
/* ONLY HERE BECAUSE I'M NOT ALOWED TO CHANGE THE .H FILES */




int escapeSteps( double x, double y ){
   // Complex numbers
   complexNumber constant;
   constant.real      = x;
   constant.imaginary = y;
   complexNumber result; // this is escape steps
   result.real        = 0;
   result.imaginary   = 0;
   // Iteration counter
   unsigned int iterations = 0;
   // Until we get far enough away
   while( (distanceFromOrigin(result) < TARGET_DISTANCE) && 
                    (iterations < MAX_ESCAPE_ITERATIONS) ) {
      // Square resulting complex number
      result = multiplyComplex( result, result );
      // Add complex constant
      result = addComplex( result, constant );
      iterations++;
   }
   // Return iteration counter
   return iterations;
}

static complexNumber multiplyComplex( complexNumber a, complexNumber b ) {
   complexNumber result;
   // ab+ab(i^2) - Simplified expansion on real part of (a+ai)(b+bi)
   result.real = (a.real * b.real) + (a.imaginary * b.imaginary * -1); 
   // abi+bai - Simplified expansion on imaginary part of (a+ai)(b+bi)
   result.imaginary = a.real*b.imaginary + b.real*a.imaginary;
   // Return new complex number
   return result;
}

static complexNumber addComplex( complexNumber a, complexNumber b ) {
   complexNumber result;
   // Add real components
   result.real = a.real + b.real;
   // Add imaginary component
   result.imaginary = a.imaginary + b.imaginary;
   // Return new complex number
   return result;
}

static double distanceFromOrigin( complexNumber point ) {
   // Length of position vector OP.
   double rSquared = point.real * point.real;
   double iSquared = point.imaginary * point.imaginary;
   // return |OP|
   return sqrt(rSquared + iSquared);
}