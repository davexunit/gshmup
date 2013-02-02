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
    SCM on_hit;
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
    SCM on_hit;
} GshmupBullet;

GshmupBulletType *check_bullet_type (SCM bullet_type);

void gshmup_draw_bullet (GshmupEntity *entity);
void gshmup_update_bullet (GshmupEntity *entity);

GshmupBulletSystem *gshmup_create_bullet_system (int max_bullets,
                                                 GshmupSpriteSheet *sprite_sheet);
void gshmup_destroy_bullet_system (GshmupBulletSystem *system);
void gshmup_draw_bullet_system (GshmupBulletSystem *system);
void gshmup_draw_bullet_system_hitboxes (GshmupBulletSystem *system,
                                         ALLEGRO_COLOR fill_color,
                                         ALLEGRO_COLOR border_color);
void gshmup_update_bullet_system (GshmupBulletSystem *system);
void gshmup_set_bullet_type (GshmupEntity *entity, GshmupBulletType *type);
void gshmup_emit_bullet (GshmupBulletSystem *system, GshmupVector2 position,
                         float speed, float direction, float acceleration,
                         float angular_velocity, float life,
                         GshmupBulletType *type, SCM thunk);
int gshmup_get_bullet_system_size (GshmupBulletSystem *system);
int gshmup_get_bullet_system_free_size (GshmupBulletSystem *system);
int gshmup_get_bullet_system_max_free_size (GshmupBulletSystem *system);
void gshmup_set_current_bullet_system (GshmupBulletSystem *system);
void gshmup_bullet_system_collide_rect (GshmupBulletSystem *system, GshmupRect rect);
void gshmup_bullet_system_init_scm (void);

#endif
