#include "termpet.h"

#include <stdlib.h>

float random_float(float min, float max) {
    float x;

    x = (float)rand() / (float)RAND_MAX;    // between 0 and 1
    x *= max - min;                         // between 0 and (max-min)
    x += min;                               // between min and max

    return x;
}
