#include "entity.h"

#ifndef GSHMUP_ENEMY_H
#define GSHMUP_ENEMY_H

#include "common.h"
#include "sprite.h"

typedef struct {
    _GSHMUP_ENTITY_HEADER
    GshmupSprite sprite;
} GshmupEnemy;

void gshmup_init_enemy (GshmupEntity *entity, GshmupAnimation *anim);
void gshmup_draw_enemy (GshmupEnemy *enemy);
void gshmup_update_enemy (GshmupEnemy *enemy);

#endif
