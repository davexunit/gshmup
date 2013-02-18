#include "entity.h"

#ifndef GSHMUP_ENEMY_H
#define GSHMUP_ENEMY_H

#include "common.h"
#include "entity.h"
#include "sprite.h"
#include "rect.h"

typedef struct {
    gchar *name;
    gchar *sprite_sheet_filename;
    gchar *animation;
    gint max_health;
    GshmupRect hitbox;
    SCM on_death;
} GshmupEnemyType;

typedef struct GshmupEnemy GshmupEnemy;

struct GshmupEnemy {
    GshmupEntity entity;
    gboolean kill;
    gint health;
    SCM on_death;
    GshmupEnemy *next;
};

GshmupEnemyType *gshmup_check_enemy_type_smob (SCM enemy_type_smob);
GshmupEnemy *gshmup_create_enemy (GshmupEnemyType *type, SCM script);
void gshmup_destroy_enemy (GshmupEnemy *enemy);
void gshmup_destroy_enemies (GshmupEnemy *enemy);
void gshmup_draw_enemy (GshmupEnemy *enemy);
void gshmup_draw_enemies (GshmupEnemy *enemy);
void gshmup_update_enemy (GshmupEnemy *enemy);
GshmupEnemy *gshmup_update_enemies (GshmupEnemy *enemy);
void gshmup_set_current_enemy (GshmupEnemy *enemy);
void gshmup_enemy_init_scm (void);

#endif
