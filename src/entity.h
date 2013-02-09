#ifndef GSHMUP_ENTITY_H
#define GSHMUP_ENTITY_H

#include "common.h"
#include "rect.h"
#include "sprite.h"

/*
 * Common properties for game objects.
 */
typedef struct {
    const char *name;
    GshmupVector2 position;
    GshmupRect hitbox;
    GshmupSpriteSheet *sprite_sheet;
    GshmupSprite sprite;
    SCM agenda;
} GshmupEntity;

void gshmup_entity_init_scm (void);

#endif
