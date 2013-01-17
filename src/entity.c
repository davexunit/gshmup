#include "entity.h"

GshmupEntity *gshmup_create_entity (void)
{
    return (GshmupEntity *) scm_gc_malloc (sizeof (GshmupEntity), "entity");
}

void gshmup_destroy_entity (GshmupEntity *entity)
{
    scm_gc_free (entity, sizeof (GshmupEntity), "entity");
}

void
gshmup_draw_entity (GshmupEntity *entity)
{
    switch (entity->type)
    {
    case GSHMUP_ENTITY_PLAYER:
        gshmup_draw_player (GSHMUP_PLAYER (entity));
        break;
    case GSHMUP_ENTITY_BULLET:
        gshmup_draw_bullet (GSHMUP_BULLET (entity));
        break;
    default:
        break;
    }
}

void
gshmup_update_entity (GshmupEntity *entity)
{
    switch (entity->type)
    {
    case GSHMUP_ENTITY_PLAYER:
        gshmup_update_player (GSHMUP_PLAYER (entity));
        break;
    case GSHMUP_ENTITY_BULLET:
        gshmup_update_bullet (GSHMUP_BULLET (entity));
        break;
    default:
        break;
    }
}

void
gshmup_kill_entity (GshmupEntity *entity)
{
    entity->base.kill = true;
}
