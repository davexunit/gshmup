#include "math.h"

float gshmup_deg_to_rad (float degrees)
{
    return degrees * ALLEGRO_PI / 180;
}

float gshmup_rad_to_deg (float radians)
{
    return radians * 180 / ALLEGRO_PI;
}

float
gshmup_rand1 (void)
{
    return (float) rand () / (float) RAND_MAX;
}

float gshmup_randf (float max)
{
    return gshmup_rand1 () * max;
}

float
gshmup_clamp (float n, float min, float max)
{
    return fmax (min, fmin (max, n));
}
