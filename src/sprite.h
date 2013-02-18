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
        struct {
            GshmupAnimation *anim;
            gint current_frame;
            gint timer;
            gboolean playing;
        } anim_state;
    };
    ALLEGRO_COLOR color;
    GshmupVector2 position;
    GshmupVector2 scale;
    GshmupVector2 anchor;
    float rotation;
} GshmupSprite;

GshmupSprite gshmup_create_sprite (ALLEGRO_BITMAP *image);
GshmupSprite gshmup_create_sprite_animated (GshmupAnimation *anim);
void gshmup_draw_sprite (GshmupSprite *sprite);
void gshmup_sprite_play_animation (GshmupSprite *sprite);
void gshmup_sprite_stop_animation (GshmupSprite *sprite);

#endif
