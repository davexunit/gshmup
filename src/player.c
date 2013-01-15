#include "player.h"

void
gshmup_init_player (GshmupPlayer *player, ALLEGRO_BITMAP *image)
{
    player->position = gshmup_create_vector2 (0, 0);
    player->speed = 0;
    player->credits = 0;
    player->score = 0;
    gshmup_init_sprite (&player->sprite, image);

    for (int i = 0; i < 4; ++i) {
        player->dir[i] = false;
    }
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
