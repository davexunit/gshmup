#ifndef GSHMUP_BACKGROUND_H
#define GSHMUP_BACKGROUND_H

#include "common.h"

/* Infinite vertical scrolling background. */
typedef struct {
    ALLEGRO_BITMAP *image;
    float scroll_y;
    float scroll_speed;
} GshmupBackground;

void gshmup_init_background (GshmupBackground *bg, ALLEGRO_BITMAP *image,
                             float scroll_speed);
void gshmup_draw_background (GshmupBackground *bg);
void gshmup_update_background (GshmupBackground *bg);

#endif
