#ifndef GSHMUP_BULLET_SYSTEM_H
#define GSHMUP_BULLET_SYSTEM_H

#include "common.h"
#include "game.h"
#include "rect.h"
#include "sprite.h"

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

typedef struct GshmupBulletAgenda GshmupBulletAgenda;

/*
 * List nodes that store an agenda for a bullet.
 *
 * This is necessary because we need to use scm_gc_malloc when we are storing
 * SCM objects. The bullets array uses a glib array, which is great, but we
 * can't store SCMs in there.
 */
struct GshmupBulletAgenda {
    SCM agenda;
    GshmupBulletAgenda *next;
};

typedef struct {
    GshmupSpriteSheet *sprite_sheet;
    GArray *bullets;
    GshmupBulletAgenda *agendas;
    GshmupRect bounds;
    int len;
} GshmupBulletSystem;

typedef struct {
    GshmupBulletSystem *parent;
    int life;         /* Maximum lifetime. 0 is unlimited. */
    int life_count;   /* Total elapsed lifetime. */
    int blend_mode;
    bool directional; /* Rotate sprite in bullet direction? */
    bool kill;
    /* Euler angles and such. */
    float speed;
    float direction;
    float acceleration;
    float angular_velocity;
    /* Vectors for fast updates. */
    GshmupVector2 pos;
    GshmupVector2 vel;
    GshmupVector2 acc;
    ALLEGRO_TRANSFORM ang_vel;
    GshmupRect hitbox;
    GshmupSprite sprite;
    ALLEGRO_COLOR color;
    GshmupBulletAgenda *agenda;
    SCM on_hit;
} GshmupBullet;

GshmupBulletType *check_bullet_type (SCM bullet_type);

void gshmup_draw_bullet (GshmupBullet *bullet);
void gshmup_update_bullet (GshmupBullet *bullet);

GshmupBulletSystem *gshmup_create_bullet_system (int reserved_size,
                                                 GshmupSpriteSheet *sprite_sheet);
void gshmup_destroy_bullet_system (GshmupBulletSystem *system);
void gshmup_draw_bullet_system (GshmupBulletSystem *system);
void gshmup_draw_bullet_system_hitboxes (GshmupBulletSystem *system,
                                         ALLEGRO_COLOR fill_color,
                                         ALLEGRO_COLOR border_color);
void gshmup_update_bullet_system (GshmupBulletSystem *system);
void gshmup_set_bullet_type (GshmupBullet *bullet, GshmupBulletType *type);
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
