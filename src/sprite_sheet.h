#ifndef GSHMUP_SPRITE_SHEET_H
#define GSHMUP_SPRITE_SHEET_H

#include "common.h"

/*
 * GshmupSpriteSheet
 *
 * Splits an image into a grid of many small tiles.
 */
typedef struct {
    ALLEGRO_BITMAP *image;
    int tile_width;
    int tile_height;
    int spacing;
    int margin;
    int num_tiles;
    ALLEGRO_BITMAP **tiles;
} GshmupSpriteSheet;

GshmupSpriteSheet *gshmup_create_sprite_sheet (ALLEGRO_BITMAP *image, int tile_width,
                                               int tile_height, int spacing,
                                               int margin);
GshmupSpriteSheet *gshmup_load_sprite_sheet (const gchar *filename);
void gshmup_destroy_sprite_sheet (GshmupSpriteSheet *sprite_sheet);
ALLEGRO_BITMAP *gshmup_get_sprite_sheet_tile (GshmupSpriteSheet *sprite_sheet,
                                              int index);

#endif
