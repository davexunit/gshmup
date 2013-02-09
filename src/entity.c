#include "entity.h"
#include "string.h"

/* static GshmupEntity *current_entity = NULL; */

GshmupEntity
gshmup_create_entity (const char *name)
{
    GshmupEntity entity;

    strncpy (entity.name, name, GSHMUP_MAX_NAME_LENGTH - 1);
    entity.name[GSHMUP_MAX_NAME_LENGTH] = '\0';
    entity.position = gshmup_create_vector2 (0, 0);
    entity.hitbox = gshmup_create_rect (0, 0, 0, 0);
    entity.sprite_sheet = NULL;
    entity.agenda = gshmup_create_agenda ();

    return entity;
}

void
gshmup_draw_entity (GshmupEntity *entity)
{
    entity->sprite.position = entity->position;
    gshmup_draw_sprite (&entity->sprite);
}

void
gshmup_update_entity (GshmupEntity *entity)
{
    gshmup_update_agenda (entity->agenda);

    if (entity->sprite.type == GSHMUP_SPRITE_ANIMATED) {
        gshmup_update_animation (entity->sprite.anim);
    }
}

void
gshmup_entity_init_scm (void)
{
#include "entity.x"
}
