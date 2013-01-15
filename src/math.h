#ifndef GSHMUP_MATH_H
#define GSHMUP_MATH_H

#include <math.h>
#include <stdlib.h>

#include "common.h"

float gshmup_deg_to_rad (float degrees);
float gshmup_rad_to_deg (float radians);
float gshmup_rand1 (void);
float gshmup_randf (float max);
float gshmup_clamp (float n, float min, float max);

#endif
