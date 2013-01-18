#include "entity_pool.h"

GshmupEntityPool *
gshmup_create_entity_pool (int max_pool_size)
{
    GshmupEntityPool *pool;

    pool = (GshmupEntityPool *) scm_gc_malloc (sizeof (GshmupEntityPool),
                                                       "entity pool");
    pool->max_pool_size = max_pool_size;
    pool->pool_size = 0;
    pool->size = 0;
    pool->active_entities = NULL;
    pool->free_entities = NULL;

    return pool;
}

static void
destroy_entities (GshmupEntity *entities)
{
    GshmupEntity *entity = entities;

    while (entity) {
        GshmupEntity *next = entity->base.next;

        gshmup_destroy_entity (entity);
        entity = next;
    }
}

void
gshmup_destroy_entity_pool (GshmupEntityPool *pool)
{
    destroy_entities (pool->active_entities);
    destroy_entities (pool->free_entities);
    scm_gc_free (pool, sizeof (GshmupEntityPool), "entity pool");
}

void
gshmup_draw_entity_pool (GshmupEntityPool *pool)
{
    GshmupEntity *entity = pool->active_entities;

    while (entity) {
        gshmup_draw_entity (entity);
        entity = entity->base.next;
    }
}

void
gshmup_update_entity_pool (GshmupEntityPool *pool)
{
    GshmupEntity *entity = pool->active_entities;
    GshmupEntity *prev = NULL;

    while (entity) {
        GshmupEntity *next = entity->base.next;

        gshmup_update_entity (entity);

        if (entity->base.kill) {
            /* Remove dead entity from active list. */
            if (prev) {
                prev->base.next = next;
            } else {
                pool->active_entities = next;
            }

            /* Place dead entity in free list. */
            gshmup_entity_pool_free (pool, entity);
        } else {
            /*
             * Prev only changes when we aren't pruning a entity from
             * the active list.
             */
            prev = entity;
        }

        entity = next;
    }
}

void
gshmup_clear_entity_pool (GshmupEntityPool *pool)
{
    GshmupEntity *entity = pool->active_entities;

    while (entity) {
        GshmupEntity *next = entity->base.next;

        gshmup_entity_pool_free (pool, entity);
        entity = next;
    }
}

void
gshmup_entity_pool_free (GshmupEntityPool *pool, GshmupEntity *entity)
{
    if (pool->pool_size >= pool->max_pool_size) {
        /* Free the memory if there's no room in the pool. */
        gshmup_destroy_entity (entity);
    } else {
        pool->pool_size++;
        entity->base.next = pool->free_entities;
        pool->free_entities = entity;
    }

    pool->size--;
}

GshmupEntity *
gshmup_entity_pool_new (GshmupEntityPool *pool)
{
    GshmupEntity *entity;

    if (pool->free_entities) {
        /* Remove entity from front of free entities list. */
        entity = pool->free_entities;
        gshmup_init_entity (entity);
        pool->free_entities = entity->base.next;
        pool->pool_size--;
    } else {
        entity = gshmup_create_entity ();
    }

    /* Insert entity at front of active entities list. */
    entity->base.next = pool->active_entities;
    pool->active_entities = entity;
    pool->size++;

    return entity;
}
