#ifndef GSHMUP_ASSET_H
#define GSHMUP_ASSET_H

#include "common.h"
#include "sprite_sheet.h"

void gshmup_init_assets (void);
void gshmup_destroy_assets (void);
void gshmup_free_assets (void);
ALLEGRO_BITMAP *gshmup_load_image_asset (const gchar *filename);
GshmupSpriteSheet *gshmup_load_sprite_sheet_asset (const gchar *filename);
void gshmup_asset_init_scm (void);


#endif
