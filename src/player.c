#include "player.h"

static const char *agenda_module = "gshmup agenda";

GshmupPlayer *
gshmup_create_player (ALLEGRO_BITMAP *image)
{
    GshmupPlayer *player = (GshmupPlayer *) scm_gc_malloc (sizeof (GshmupPlayer),
                                                           "player");
    player->position = gshmup_create_vector2 (0, 0);
    player->shooting = false;
    player->speed = 0;
    player->lives = 0;
    player->credits = 0;
    player->score = 0;
    /* Make a new agenda for scheduling scripting procedures. */
    player->agenda = scm_call_0 (scm_c_public_ref (agenda_module, "make-agenda"));
    gshmup_init_sprite (&player->sprite, image);

    for (int i = 0; i < 4; ++i) {
        player->dir[i] = false;
    }

    return player;
}

void
gshmup_destroy_player (GshmupPlayer *player)
{
    scm_gc_free (player, sizeof (GshmupPlayer), "player");
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
