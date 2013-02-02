#include "player.h"

static GshmupPlayer *current_player = NULL;

GshmupEntity *
gshmup_create_player (GshmupAnimation *anim)
{
    GshmupEntity *entity = gshmup_create_entity (GSHMUP_ENTITY_PLAYER,
                                                 gshmup_draw_player,
                                                 gshmup_update_player);
    GshmupPlayer *player = GSHMUP_PLAYER (entity);

    player->shooting = false;
    player->speed = 0;
    player->lives = 0;
    player->credits = 0;
    player->score = 0;
    gshmup_init_animated_sprite (&player->sprite, anim);
    gshmup_play_animation (anim);

    for (int i = 0; i < 4; ++i) {
        player->dir[i] = false;
    }

    return entity;
}

void
gshmup_draw_player (GshmupEntity *entity)
{
    GshmupPlayer *player = GSHMUP_PLAYER (entity);

    player->sprite.position = player->position;
    gshmup_draw_sprite (&player->sprite);
}

void
gshmup_update_player (GshmupEntity *entity)
{
    GshmupPlayer *player = GSHMUP_PLAYER (entity);
    GshmupVector2 v;

    gshmup_update_animation (player->sprite.anim);
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

void
gshmup_set_current_player (GshmupPlayer *player)
{
    current_player = player;
}
