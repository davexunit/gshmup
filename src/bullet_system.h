#ifndef GSHMUP_BULLET_SYSTEM_H
#define GSHMUP_BULLET_SYSTEM_H

#include "common.h"
#include "rect.h"
#include "sprite.h"

typedef struct GshmupBullet GshmupBullet;
struct GshmupBullet {
    int life;         /* Maximum lifetime. 0 is unlimited. */
    int life_count;   /* Total elapsed lifetime. */
    bool kill;        /* Remove the bullet on next update? */
    bool directional; /* Rotate sprite in bullet direction? */
    GshmupVector2 pos;
    GshmupVector2 vel;
    GshmupVector2 acc;
    GshmupRect hitbox;
    ALLEGRO_TRANSFORM angular_velocity; /* Change in direction. */
    GshmupSprite sprite;
    GshmupBullet *next;
};

typedef struct {
    int max_pool_size;
    int pool_size;
    int bullet_count;
    GshmupBullet *active_bullets;
    GshmupBullet *free_bullets;
    GshmupRect bounds;
} GshmupBulletSystem;

GshmupBulletSystem *gshmup_create_bullet_system (int max_pool_size);
void gshmup_destroy_bullet_system (GshmupBulletSystem *system);
void gshmup_draw_bullet_system (GshmupBulletSystem *system);
void gshmup_update_bullet_system (GshmupBulletSystem *system);
void gshmup_emit_bullet (GshmupBulletSystem *system, ALLEGRO_BITMAP *image,
                         GshmupVector2 position, float speed, float direction,
                         float acceleration, float angular_velocity, float life);

#endif
