#ifndef GSHMUP_PLAYER_H
#define GSHMUP_PLAYER_H

#include "common.h"
#include "sprite.h"
#include "rect.h"
#include "game.h"

enum {
    GSHMUP_PLAYER_UP,
    GSHMUP_PLAYER_DOWN,
    GSHMUP_PLAYER_LEFT,
    GSHMUP_PLAYER_RIGHT,
};

typedef struct {
    SCM agenda;
    GshmupVector2 position;
    GshmupSprite sprite;
    bool shooting;
    bool dir[4]; /* 4 directional movement. */
    float speed;
    int lives;
    int credits;
    int score;
    GshmupRect hitbox;
} GshmupPlayer;

GshmupPlayer *gshmup_create_player (GshmupAnimation *anim);
void gshmup_destroy_player (GshmupPlayer *player);
void gshmup_draw_player (GshmupPlayer *player);
void gshmup_update_player (GshmupPlayer *player);
void gshmup_player_set_direction (GshmupPlayer *player, int dir, bool flag);
void gshmup_set_current_player (GshmupPlayer *player);
void gshmup_player_init_scm (void);

#endif
