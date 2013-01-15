#ifndef GSHMUP_SPRITE_H
#define GSHMUP_SPRITE_H

#include "common.h"
#include "color.h"

typedef struct {
    ALLEGRO_BITMAP *image;
    ALLEGRO_COLOR color;
    GshmupVector2 position;
    GshmupVector2 scale;
    GshmupVector2 anchor;
    float rotation;
} GshmupSprite;

void gshmup_init_sprite (GshmupSprite *sprite, ALLEGRO_BITMAP *image);
void gshmup_draw_sprite (GshmupSprite *sprite);

#endif
