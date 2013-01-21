#include "entity.h"

#ifndef GSHMUP_BULLET_SYSTEM_H
#define GSHMUP_BULLET_SYSTEM_H

#include "common.h"
#include "rect.h"
#include "sprite.h"
#include "entity_pool.h"

/* Blending modes */
enum {
    GSHMUP_BULLET_BLEND_ALPHA,
    GSHMUP_BULLET_BLEND_ADD,
};

/* Describes common properties for a whole class of bullets. */
typedef struct {
    int tile;
    int blend_mode;
    bool directional;
    GshmupRect hitbox;
} GshmupBulletType;

typedef struct {
    GshmupSpriteSheet *sprite_sheet;
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
    ALLEGRO_COLOR color;
    int blend_mode;
    GshmupBulletSystem *parent;
} GshmupBullet;

GshmupBulletType *check_bullet_type (SCM bullet_type);

void gshmup_draw_bullet (GshmupBullet *bullet);
void gshmup_update_bullet (GshmupBullet *bullet);

GshmupBulletSystem *gshmup_create_bullet_system (int max_bullets,
                                                 GshmupSpriteSheet *sprite_sheet);
void gshmup_destroy_bullet_system (GshmupBulletSystem *system);
void gshmup_draw_bullet_system (GshmupBulletSystem *system);
void gshmup_update_bullet_system (GshmupBulletSystem *system);
void gshmup_set_bullet_type (GshmupEntity *entity, GshmupBulletType *type);
void gshmup_emit_bullet (GshmupBulletSystem *system, GshmupVector2 position,
                         float speed, float direction, float acceleration,
                         float angular_velocity, float life,
                         GshmupBulletType *type);
int gshmup_get_bullet_system_size (GshmupBulletSystem *system);
int gshmup_get_bullet_system_free_size (GshmupBulletSystem *system);
int gshmup_get_bullet_system_max_free_size (GshmupBulletSystem *system);
void gshmup_bullet_system_init_scm (void);

#endif
