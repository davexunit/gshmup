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
    bool shooting;
    bool dir[4]; /* 4 directional movement. */
    float speed;
    int lives;
    int credits;
    int score;
    SCM agenda;
} GshmupPlayer;

GshmupPlayer *gshmup_create_player (ALLEGRO_BITMAP *image);
void gshmup_destroy_player (GshmupPlayer *player);
void gshmup_draw_player (GshmupPlayer *player);
void gshmup_update_player (GshmupPlayer *player);
void gshmup_player_set_direction (GshmupPlayer *player, int dir, bool flag);
void gshmup_player_shoot (GshmupPlayer *player);

#endif
