// Samuel Clarke - z5045865
// Wed13-Sitar
// 16/04/2016
// Pixel colour mapper (mandelbrot)

#include <math.h>

static unsigned char stepsToRed (int steps) {
    return (-27*log(steps)+90);
    // 128 - (128 * cos(x/80))
    // unsigned char colour;
    // float func = steps / RED_COS_PERIOD;
    // func = COL_COS_AMPLITUDE * cos(func);
    // colour = (steps == 256) ? 0 : COL_COS_BIAS + func;
    // return steps > 10 ? colour : 0;
}

static unsigned char stepsToBlue (int steps){
    return steps > 30 && steps < 200 ? 0 : (-27*log(steps)+90);
    // float func = steps / BLU_COS_PERIOD;
    // func = COL_COS_AMPLITUDE * cos(func);
    // unsigned char colour = COL_COS_BIAS + func;
    // return steps > 10 ? colour : 0;
}

static unsigned char stepsToGreen (int steps){
    return 0;// -27*log(steps)+100;
    // float func = steps / GRN_COS_PERIOD;
    // func = COL_COS_AMPLITUDE * cos(func);
    // unsigned char colour = COL_COS_BIAS + func;
    // return steps > 10 ? colour : 0;
}