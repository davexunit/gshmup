#include "player.h"

static const char *agenda_module = "gshmup agenda";

GshmupEntity *
gshmup_create_player (ALLEGRO_BITMAP *image)
{
    GshmupEntity *entity = gshmup_create_entity ();

    entity->type = GSHMUP_ENTITY_PLAYER;
    entity->player.position = gshmup_create_vector2 (0, 0);
    entity->player.shooting = false;
    entity->player.speed = 0;
    entity->player.lives = 0;
    entity->player.credits = 0;
    entity->player.score = 0;
    /* Make a new agenda for scheduling scripting procedures. */
    entity->player.agenda = scm_call_0 (scm_c_public_ref (agenda_module, "make-agenda"));
    gshmup_init_sprite (&entity->player.sprite, image);

    for (int i = 0; i < 4; ++i) {
        entity->player.dir[i] = false;
    }

    return entity;
}

void
gshmup_draw_player (GshmupPlayer *player)
{
    player->sprite.position = player->position;
    gshmup_draw_sprite (&player->sprite);
}

void
gshmup_update_player (GshmupPlayer *player)
{
    GshmupVector2 v;

    /* Update agenda. */
    gshmup_set_current_agenda (player->agenda);
    scm_call_2 (scm_c_public_ref (agenda_module, "update-agenda"),
                player->agenda, scm_from_int (1));
    /* Transform movement flags into a direction vector. */
    v.x = player->dir[GSHMUP_PLAYER_RIGHT] - player->dir[GSHMUP_PLAYER_LEFT];
    v.y = player->dir[GSHMUP_PLAYER_DOWN] - player->dir[GSHMUP_PLAYER_UP];
    /* Calculate new position. */
    v = gshmup_vector2_norm (v);
    v = gshmup_vector2_scale (v, player->speed);
    v = gshmup_vector2_add (v, player->position);
    /* Clamp position within screen bounds. */
    v.x = gshmup_clamp (v.x, 0, GAME_WIDTH);
    v.y = gshmup_clamp (v.y, 0, GAME_HEIGHT);
    player->position = v;
}

void
gshmup_player_set_direction (GshmupPlayer *player, int dir, bool flag)
{
    player->dir[dir] = flag;
}
