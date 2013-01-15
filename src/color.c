#include "color.h"

ALLEGRO_COLOR
gshmup_add_color (ALLEGRO_COLOR a, ALLEGRO_COLOR b)
{
    ALLEGRO_COLOR color;

    color.r = a.r + b.r;
    color.g = a.g + b.g;
    color.b = a.b + b.b;
    color.a = a.a + b.a;

    return color;
}

ALLEGRO_COLOR
gshmup_color_mult_alpha (ALLEGRO_COLOR color)
{
    ALLEGRO_COLOR new_color;

    new_color.r = color.r * color.a;
    new_color.g = color.g * color.a;
    new_color.b = color.b * color.a;
    new_color.a = color.a;

    return new_color;
}
