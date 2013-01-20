#include "entity.h"

static SCM s_make_agenda;
static SCM s_update_agenda;
static SCM s_clear_agenda;

GshmupEntity *
gshmup_create_entity (void)
{
    GshmupEntity *entity;

    entity = (GshmupEntity *) scm_gc_malloc (sizeof (GshmupEntity), "entity");
    entity->base.agenda = SCM_BOOL_F;
    gshmup_init_entity (entity);

    return entity;
}

void
gshmup_init_entity (GshmupEntity *entity)
{
    entity->base.position = gshmup_create_vector2 (0, 0);
    /*
     * Make a new agenda for scheduling scripting procedures.
     * Re-use the old one if it exists.
     */
    if (scm_is_true (entity->base.agenda)) {
        scm_call_1 (s_clear_agenda, entity->base.agenda);
    } else {
        entity->base.agenda = scm_call_0 (s_make_agenda);
    }
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
    case GSHMUP_ENTITY_ENEMY:
        gshmup_draw_enemy (GSHMUP_ENEMY (entity));
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
    gshmup_set_current_agenda (entity->base.agenda);
    scm_call_2 (s_update_agenda,
                entity->base.agenda, scm_from_int (1));

    switch (entity->type) {
    case GSHMUP_ENTITY_PLAYER:
        gshmup_update_player (GSHMUP_PLAYER (entity));
        break;
    case GSHMUP_ENTITY_ENEMY:
        gshmup_update_enemy (GSHMUP_ENEMY (entity));
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

void
gshmup_entity_clear_agenda (GshmupEntity *entity)
{
    scm_call_1 (s_clear_agenda, entity->base.agenda);
}

void
gshmup_entity_init_scm (void)
{
    const char *agenda_module = "gshmup agenda";

    /*
     * Caching this is extremely important. We use this procedure so
     * often that calling scm_c_public_ref when we need was causing
     * Guile's GC to collect every ~2 seconds.
     */
    s_make_agenda = scm_c_public_ref (agenda_module, "make-agenda");
    s_update_agenda = scm_c_public_ref (agenda_module, "update-agenda");
    s_clear_agenda = scm_c_public_ref (agenda_module, "clear-agenda");
}
