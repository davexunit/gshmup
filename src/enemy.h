#include "entity.h"

#ifndef GSHMUP_ENEMY_H
#define GSHMUP_ENEMY_H

#include "common.h"
#include "sprite.h"
#include "rect.h"

typedef struct {
    _GSHMUP_ENTITY_HEADER
    GshmupSprite sprite;
    int max_health;
    int health;
    GshmupRect hitbox;
} GshmupEnemy;

void gshmup_init_enemy (GshmupEntity *entity, GshmupAnimation *anim, int max_health);
void gshmup_draw_enemy (GshmupEnemy *enemy);
void gshmup_update_enemy (GshmupEnemy *enemy);
void gshmup_set_current_enemy (GshmupEnemy *enemy);
void gshmup_enemy_init_scm (void);

#endif
