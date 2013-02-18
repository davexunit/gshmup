#ifndef GSHMUP_SPRITE_H
#define GSHMUP_SPRITE_H

#include "common.h"
#include "color.h"
#include "sprite_sheet.h"

/* Sprites can have a static image or an animation. */
enum {
    GSHMUP_SPRITE_STATIC,
    GSHMUP_SPRITE_ANIMATED,
};

typedef struct {
    int type;
    union {
        ALLEGRO_BITMAP *image;
        GshmupAnimation *anim;
    };
    ALLEGRO_COLOR color;
    GshmupVector2 position;
    GshmupVector2 scale;
    GshmupVector2 anchor;
    float rotation;
} GshmupSprite;

void gshmup_init_sprite (GshmupSprite *sprite, ALLEGRO_BITMAP *image);
void gshmup_init_animated_sprite (GshmupSprite *sprite, GshmupAnimation *anim);
void gshmup_draw_sprite (GshmupSprite *sprite);

#endif
