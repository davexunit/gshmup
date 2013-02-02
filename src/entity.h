#ifndef GSHMUP_ENTITY_H
#define GSHMUP_ENTITY_H

/*
 * Entities represent the objects in the game such as the player,
 * enemy, and bullets.
 *
 * GshmupEntity is a union of all possible entity types, providing a
 * simple implementation of polymorphism.
 */

typedef enum {
    GSHMUP_ENTITY_ANY,
    GSHMUP_ENTITY_PLAYER,
    GSHMUP_ENTITY_ENEMY,
    GSHMUP_ENTITY_ITEM,
    GSHMUP_ENTITY_BULLET,
    GSHMUP_ENTITY_PARTICLE,
} GshmupEntityType;

typedef union GshmupEntity GshmupEntity;

#define _GSHMUP_ENTITY_HEADER \
    GshmupEntityType type; \
    SCM agenda; \
    bool kill; \
    GshmupVector2 position; \
    GshmupEntity *next;

#include "player.h"
#include "enemy.h"
#include "bullet_system.h"

union GshmupEntity {
    GshmupEntityType type;
    /*
     * The base struct allows changing all common member variables in
     * _GSHMUP_ENTITY_TYPE without caring about the entity type.
     */
    struct {
        _GSHMUP_ENTITY_HEADER
    } base;
    GshmupPlayer player;
    GshmupEnemy enemy;
    GshmupBullet bullet;
};

GshmupEntity *gshmup_create_entity (void);
void gshmup_init_entity (GshmupEntity *entity);
void gshmup_destroy_entity (GshmupEntity *entity);
void gshmup_draw_entity (GshmupEntity *entity);
void gshmup_update_entity (GshmupEntity *entity);
void gshmup_entity_schedule (GshmupEntity *entity, int dt, SCM thunk);
void gshmup_kill_entity (GshmupEntity *entity);
void gshmup_entity_clear_agenda (GshmupEntity *entity);
void gshmup_entity_init_scm (void);

/* Type cast macros. */
#define GSHMUP_PLAYER(entity) (&entity->player)
#define GSHMUP_ENEMY(entity) (&entity->enemy)
#define GSHMUP_BULLET(entity) (&entity->bullet)

#endif
