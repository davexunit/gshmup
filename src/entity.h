#ifndef GSHMUP_ENTITY_H
#define GSHMUP_ENTITY_H

/*
 * Entities represent the objects in the game such as the player,
 * enemy, and bullets.
 *
 * GshmupEntity is a union of all possible entity types.
 */

typedef enum {
    GSHMUP_ENTITY_PLAYER,
    GSHMUP_ENTITY_ENEMY,
    GSHMUP_ENTITY_ITEM,
    GSHMUP_ENTITY_BULLET,
    GSHMUP_ENTITY_PARTICLE,
} GshmupEntityType;

#define _GSHMUP_ENTITY_HEADER \
    GshmupEntityType type;

typedef union GshmupEntity GshmupEntity;

#include "player.h"

union GshmupEntity {
    GshmupEntityType type;
    GshmupPlayer player;
    /* GshmupBullet bullet; */
};

void gshmup_draw_entity (GshmupEntity *entity);
void gshmup_update_entity (GshmupEntity *entity);

/* Type cast macros. */
#define GSHMUP_PLAYER(entity) (&entity->player)

#endif
