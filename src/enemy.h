#include "entity.h"

#ifndef GSHMUP_ENEMY_H
#define GSHMUP_ENEMY_H

#include "common.h"
#include "entity.h"
#include "sprite.h"
#include "rect.h"

typedef struct GshmupEnemy GshmupEnemy;

struct GshmupEnemy{
    GshmupEntity entity;
    bool kill;
    int max_health;
    int health;
    GshmupEnemy *next;
    SCM on_death;
};

GshmupEnemy *gshmup_create_enemy (GshmupAnimation *anim, int max_health);
void gshmup_destroy_enemy (GshmupEnemy *enemy);
void gshmup_destroy_enemies (GshmupEnemy *enemy);
void gshmup_draw_enemy (GshmupEnemy *enemy);
void gshmup_draw_enemies (GshmupEnemy *enemy);
void gshmup_update_enemy (GshmupEnemy *enemy);
GshmupEnemy *gshmup_update_enemies (GshmupEnemy *enemy);
void gshmup_set_current_enemy (GshmupEnemy *enemy);
void gshmup_enemy_init_scm (void);

#endif
