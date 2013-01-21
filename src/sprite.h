#ifndef GSHMUP_SPRITE_H
#define GSHMUP_SPRITE_H

#include "common.h"
#include "color.h"

/* Sprites can have a static image or an animation. */
enum {
    GSHMUP_SPRITE_STATIC,
    GSHMUP_SPRITE_ANIMATED,
};

/* Animation modes */
enum {
    GSHMUP_ANIM_ONCE,
    GSHMUP_ANIM_LOOP,
};

/* Sprite animation */
typedef struct {
    GshmupSpriteSheet *sprite_sheet;
    int duration;
    int timer;
    int num_frames;
    int current_frame;
    int *frames;
    int anim_mode;
    bool playing;
} GshmupAnimation;

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

GshmupAnimation *gshmup_create_animation (GshmupSpriteSheet *sprite_sheet, int duration,
                                          int num_frames, int *frames, int anim_mode);
void gshmup_destroy_animation (GshmupAnimation *anim);
ALLEGRO_BITMAP *gshmup_get_animation_image (GshmupAnimation *anim);
void gshmup_update_animation (GshmupAnimation *anim);
void gshmup_play_animation (GshmupAnimation *anim);
void gshmup_stop_animation (GshmupAnimation *anim);

void gshmup_init_sprite (GshmupSprite *sprite, ALLEGRO_BITMAP *image);
void gshmup_init_animated_sprite (GshmupSprite *sprite, GshmupAnimation *anim);
void gshmup_draw_sprite (GshmupSprite *sprite);

#endif
