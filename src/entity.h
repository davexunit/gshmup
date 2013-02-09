#ifndef GSHMUP_ENTITY_H
#define GSHMUP_ENTITY_H

#include "common.h"
#include "rect.h"
#include "sprite.h"

#define GSHMUP_MAX_NAME_LENGTH 128

/*
 * Common properties for game objects.
 */
typedef struct {
    char name[GSHMUP_MAX_NAME_LENGTH];
    GshmupVector2 position;
    GshmupRect hitbox;
    GshmupSpriteSheet *sprite_sheet;
    GshmupSprite sprite;
    SCM agenda;
} GshmupEntity;

GshmupEntity gshmup_create_entity (const char *name);
void gshmup_draw_entity (GshmupEntity *entity);
void gshmup_update_entity (GshmupEntity *entity);
void gshmup_entity_init_scm (void);

#endif
