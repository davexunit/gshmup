#include "entity.h"
#include "game.h"

static GshmupEntity *current_entity = NULL;

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
    entity->base.kill = false;

    /*
     * Make a new agenda for scheduling scripting procedures.
     * Re-use the old one if it exists.
     */
    if (scm_is_true (entity->base.agenda)) {
        gshmup_clear_agenda (entity->base.agenda);
    } else {
        entity->base.agenda = gshmup_create_agenda ();
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
    default:
        break;
    }
}

void
gshmup_update_entity (GshmupEntity *entity)
{
    switch (entity->type) {
    default:
        break;
    }

    /* Update agenda. */
    current_entity = entity;
    gshmup_update_agenda (entity->base.agenda);
}

void
gshmup_entity_schedule (GshmupEntity *entity, int dt, SCM thunk)
{
    gshmup_schedule (entity->base.agenda, dt, thunk);
}

void
gshmup_kill_entity (GshmupEntity *entity)
{
    entity->base.kill = true;
}

void
gshmup_entity_clear_agenda (GshmupEntity *entity)
{
    gshmup_clear_agenda (entity->base.agenda);
}

SCM_DEFINE (entity_position, "entity-position", 0, 0, 0,
            (void),
            "Return the position of the current entity.")
{
    return gshmup_scm_from_vector2 (current_entity->base.position);
}

SCM_DEFINE (move_entity, "move-entity", 1, 0, 0,
            (SCM v),
            "Add @var{v} to the currrent entity's position.")
{
    current_entity->base.position = gshmup_vector2_add (current_entity->base.position,
                                                        gshmup_scm_to_vector2 (v));

    return SCM_UNSPECIFIED;
}

SCM_DEFINE (kill_entity, "kill-entity", 0, 0, 0,
            (void),
            "Sets kill flag on the current entity.")
{
    current_entity->base.kill = true;

    return SCM_UNSPECIFIED;
}

void
gshmup_entity_init_scm (void)
{
#include "entity.x"

    scm_c_export (s_entity_position,
                  s_move_entity,
                  s_kill_entity,
                  NULL);
}
