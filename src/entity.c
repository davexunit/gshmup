#include "entity.h"

void
gshmup_draw_entity (GshmupEntity *entity)
{
    switch (entity->type)
    {
    case GSHMUP_ENTITY_PLAYER:
        gshmup_draw_player (GSHMUP_PLAYER (entity));
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
    default:
        break;
    }
}
