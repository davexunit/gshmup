#ifndef GSHMUP_SPRITE_SHEET_H
#define GSHMUP_SPRITE_SHEET_H

#include "common.h"

/* Animation modes */
enum {
    GSHMUP_ANIM_ONCE,
    GSHMUP_ANIM_LOOP,
};

typedef struct GshmupSpriteSheet GshmupSpriteSheet;

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

/*
 * GshmupSpriteSheet
 *
 * Splits an image into a grid of many small tiles.
 */
struct GshmupSpriteSheet {
    ALLEGRO_BITMAP *image;
    gint tile_width;
    gint tile_height;
    gint spacing;
    gint margin;
    gint num_tiles;
    ALLEGRO_BITMAP **tiles;
    GHashTable *animations;
};

GshmupAnimation *gshmup_create_animation (GshmupSpriteSheet *sprite_sheet, int duration,
                                          int num_frames, int *frames, int anim_mode);
void gshmup_destroy_animation (GshmupAnimation *anim);
ALLEGRO_BITMAP *gshmup_get_animation_image (GshmupAnimation *anim);
void gshmup_update_animation (GshmupAnimation *anim);
void gshmup_play_animation (GshmupAnimation *anim);
void gshmup_stop_animation (GshmupAnimation *anim);

GshmupSpriteSheet *gshmup_create_sprite_sheet (ALLEGRO_BITMAP *image, int tile_width,
                                               int tile_height, int spacing,
                                               int margin);
GshmupSpriteSheet *gshmup_load_sprite_sheet (const gchar *filename);
void gshmup_sprite_sheet_add_animation (GshmupSpriteSheet *sprite_sheet,
                                        const gchar *key,
                                        GshmupAnimation *anim);
void gshmup_destroy_sprite_sheet (GshmupSpriteSheet *sprite_sheet);
ALLEGRO_BITMAP *gshmup_get_sprite_sheet_tile (GshmupSpriteSheet *sprite_sheet,
                                              int index);
GshmupAnimation *gshmup_get_sprite_sheet_animation (GshmupSpriteSheet *sprite_sheet,
                                                    const gchar *key);

#endif
