#ifndef GSHMUP_PLAYER_H
#define GSHMUP_PLAYER_H

#include "common.h"
#include "sprite.h"
#include "game.h"

enum {
    GSHMUP_PLAYER_UP = 0,
    GSHMUP_PLAYER_DOWN,
    GSHMUP_PLAYER_LEFT,
    GSHMUP_PLAYER_RIGHT,
};

typedef struct {
    GshmupSprite sprite;
    GshmupVector2 position;
    bool dir[4]; /* 4 directional movement. */
    float speed;
    int credits;
    int score;
} GshmupPlayer;

void gshmup_init_player (GshmupPlayer *player, ALLEGRO_BITMAP *image);
void gshmup_draw_player (GshmupPlayer *player);
void gshmup_update_player (GshmupPlayer *player);
void gshmup_player_set_direction (GshmupPlayer *player, int dir, bool flag);

#endif
