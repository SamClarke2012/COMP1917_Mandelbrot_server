#!/bin/sh
printf "Compiling mandelbrotTest...\n"
gcc -Wall -Werror mandelbrotTest.c mandelbrot.c -o mandelbrotTest -lm
printf "Running regression tests...\n"
./mandelbrotTest
printf "Removing test executable...\n"
rm mandelbrotTest
printf "Done!\n"
