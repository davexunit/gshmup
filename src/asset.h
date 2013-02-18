#ifndef GSHMUP_ASSET_H
#define GSHMUP_ASSET_H

#include "common.h"

void gshmup_init_assets (void);
void gshmup_destroy_assets (void);
void gshmup_free_assets (void);
ALLEGRO_BITMAP *gshmup_load_image (const char *filename);

#endif
