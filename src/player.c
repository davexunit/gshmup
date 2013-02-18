#include "player.h"

static GshmupPlayer *current_player = NULL;
SCM_VARIABLE_INIT (s_lives_per_credit, "*lives-per-credit*", scm_from_int (3));
SCM_VARIABLE_INIT (s_num_credits, "*num-credits*", scm_from_int (3));
SCM_VARIABLE_INIT (s_player_speed, "*player-speed*", scm_from_double (6));
SCM_VARIABLE (s_player_on_shoot, "*player-on-shoot*");

GshmupPlayer *
gshmup_create_player (GshmupAnimation *anim)
{
    GshmupPlayer *player = (GshmupPlayer *) scm_gc_malloc (sizeof (GshmupPlayer),
                                                           "player");

    player->entity = gshmup_create_entity ("Player");
    player->shooting = false;
    player->lives = scm_to_int (scm_variable_ref (s_lives_per_credit)) - 1;
    player->credits = gshmup_get_initial_player_credits () - 1;
    player->speed = scm_to_double (scm_variable_ref (s_player_speed));
    player->score = 0;
    player->on_shoot = scm_variable_ref (s_player_on_shoot);
    player->on_game_over = NULL;
    gshmup_player_stop (player);
    gshmup_init_animated_sprite (&player->entity.sprite, anim);
    gshmup_play_animation (anim);

    return player;
}

void
gshmup_destroy_player (GshmupPlayer *player)
{
    g_free (player->entity.name);
    scm_gc_free (player, sizeof (GshmupPlayer), "player");
}

void
gshmup_draw_player (GshmupPlayer *player)
{
    gshmup_draw_entity (&player->entity);
}

void
gshmup_update_player (GshmupPlayer *player)
{
    GshmupVector2 v;

    /* Transform movement flags into a direction vector. */
    v.x = player->dir[GSHMUP_PLAYER_RIGHT] - player->dir[GSHMUP_PLAYER_LEFT];
    v.y = player->dir[GSHMUP_PLAYER_DOWN] - player->dir[GSHMUP_PLAYER_UP];
    /* Calculate new position. */
    v = gshmup_vector2_norm (v);
    v = gshmup_vector2_scale (v, player->speed);
    v = gshmup_vector2_add (v, player->entity.position);
    /* Clamp position within screen bounds. */
    v.x = gshmup_clamp (v.x, 0, GAME_WIDTH);
    v.y = gshmup_clamp (v.y, 0, GAME_HEIGHT);
    player->entity.position = v;

    gshmup_update_entity (&player->entity);
}

void
gshmup_player_set_direction (GshmupPlayer *player, int dir, bool flag)
{
    player->dir[dir] = flag;
}

void
gshmup_player_stop (GshmupPlayer *player)
{
    for (int i = 0; i < 4; ++i) {
        player->dir[i] = false;
    }
}

void gshmup_set_player_shooting (GshmupPlayer *player, bool shoot)
{
    player->shooting = shoot;

    if (shoot) {
        if (scm_is_true (scm_procedure_p (player->on_shoot))) {
            gshmup_schedule (player->entity.agenda, 0, player->on_shoot);
        }
    } else {
        gshmup_clear_agenda (player->entity.agenda);
    }
}

void
gshmup_set_current_player (GshmupPlayer *player)
{
    current_player = player;
}

SCM_DEFINE (kill_player, "kill-player", 0, 0, 0,
            (void),
            "Decrement player life counter. Game over if there are no lives left.")
{
    if (--current_player->lives < 0) {
        if (--current_player->credits < 0) {
            if (current_player->on_game_over) {
                current_player->on_game_over ();
            }

            current_player->lives = 0;
            current_player->credits = 0;
        } else {
            current_player->lives = gshmup_get_initial_player_credits () - 1;
        }
    }

    return SCM_UNSPECIFIED;
}

int
gshmup_get_initial_player_credits (void)
{
    return scm_to_int (scm_variable_ref (s_num_credits));
}

void
gshmup_player_init_scm (void)
{
#include "player.x"

    scm_c_export ("*lives-per-credit*",
                  "*num-credits*",
                  "*player-speed*",
                  "*player-on-shoot*",
                  s_kill_player,
                  NULL);
}
