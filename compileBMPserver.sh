#!/bin/sh
printf "Compiling BMPserver...\n\n"
gcc -Wall -Werror bmpServer.c mandelbrot.c pixelColor.c -o BMPserver -lm 
