#include "entity.h"

static const char *agenda_module = "gshmup agenda";

GshmupEntity *
gshmup_create_entity (void)
{
    GshmupEntity *entity;

    entity = (GshmupEntity *) scm_gc_malloc (sizeof (GshmupEntity), "entity");
    gshmup_init_entity (entity);

    return entity;
}

void
gshmup_init_entity (GshmupEntity *entity)
{
    /* Make a new agenda for scheduling scripting procedures. */
    entity->base.agenda = scm_call_0 (scm_c_public_ref (agenda_module, "make-agenda"));
}

void gshmup_destroy_entity (GshmupEntity *entity)
{
    scm_gc_free (entity, sizeof (GshmupEntity), "entity");
}

void
gshmup_draw_entity (GshmupEntity *entity)
{
    switch (entity->type) {
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
    /* Update agenda. */
    scm_call_2 (scm_c_public_ref (agenda_module, "update-agenda"),
                entity->base.agenda, scm_from_int (1));

    switch (entity->type) {
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
