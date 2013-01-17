#include "entity.h"

#ifndef GSHMUP_ENTITY_POOL_H
#define GSHMUP_ENTITY_POOL_H

#include "common.h"

/*
 * It is very common to work with many entities at once.
 * This structure maintains a list of activie entities (those whose
 * kill flag have not been set). Since it is often advantageous to
 * recycle memory, when a GshmupEntityPool is updated, it prunes dead
 * entities and places them in a free list to be used later.
 */
typedef struct {
    int size;
    int pool_size;
    int max_pool_size;
    GshmupEntity *active_entities;
    GshmupEntity *free_entities;
} GshmupEntityPool;

GshmupEntityPool *gshmup_create_entity_pool (int max_pool_size);
void gshmup_destroy_entity_pool (GshmupEntityPool *pool);
void gshmup_draw_entity_pool (GshmupEntityPool *pool);
void gshmup_update_entity_pool (GshmupEntityPool *pool);
void gshmup_clear_entity_pool (GshmupEntityPool *pool);
void gshmup_entity_pool_free (GshmupEntityPool *pool, GshmupEntity *entity);
GshmupEntity *gshmup_entity_pool_new (GshmupEntityPool *pool);

#endif
