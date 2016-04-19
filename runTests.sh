#!/bin/sh

# mandelbrot escapeSteps tests
printf "Compiling mandelbrotTest...\n"
gcc -Wall -Werror mandelbrotTest.c mandelbrot.c -o mandelbrotTest -lm
printf "Running regression tests...\n"
./mandelbrotTest
printf "Removing mandelbrot test executable...\n"
rm mandelbrotTest
printf "Done!\n"
# pixelColor tests
printf "Compiling colourTest...\n"
gcc -Wall -Werror pixelColorTest.c pixelColor.c -o colourTest -lm
printf "Running regression tests...\n"
./colourTest
printf "Removing colour test executable...\n"
rm colourTest
printf "Done!\n"

