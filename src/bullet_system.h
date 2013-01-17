#include "entity.h"

#ifndef GSHMUP_BULLET_SYSTEM_H
#define GSHMUP_BULLET_SYSTEM_H

#include "common.h"
#include "rect.h"
#include "sprite.h"
#include "entity_pool.h"

typedef struct {
    GshmupEntityPool *bullets;
    GshmupRect bounds;
} GshmupBulletSystem;

typedef struct {
    _GSHMUP_ENTITY_HEADER
    int life;         /* Maximum lifetime. 0 is unlimited. */
    int life_count;   /* Total elapsed lifetime. */
    bool directional; /* Rotate sprite in bullet direction? */
    GshmupVector2 vel;
    GshmupVector2 acc;
    GshmupRect hitbox;
    ALLEGRO_TRANSFORM angular_velocity; /* Change in direction. */
    GshmupSprite sprite;
    GshmupBulletSystem *parent;
} GshmupBullet;

void gshmup_draw_bullet (GshmupBullet *bullet);
void gshmup_update_bullet (GshmupBullet *bullet);

GshmupBulletSystem *gshmup_create_bullet_system (int max_bullets);
void gshmup_destroy_bullet_system (GshmupBulletSystem *system);
void gshmup_draw_bullet_system (GshmupBulletSystem *system);
void gshmup_update_bullet_system (GshmupBulletSystem *system);
void gshmup_emit_bullet (GshmupBulletSystem *system, ALLEGRO_BITMAP *image,
                         GshmupVector2 position, float speed, float direction,
                         float acceleration, float angular_velocity, float life);
int gshmup_get_bullet_system_size (GshmupBulletSystem *system);
int gshmup_get_bullet_system_free_size (GshmupBulletSystem *system);
int gshmup_get_bullet_system_max_free_size (GshmupBulletSystem *system);

#endif
