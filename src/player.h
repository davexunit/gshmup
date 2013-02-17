#ifndef GSHMUP_PLAYER_H
#define GSHMUP_PLAYER_H

#include "common.h"
#include "sprite.h"
#include "rect.h"
#include "entity.h"

enum {
    GSHMUP_PLAYER_UP,
    GSHMUP_PLAYER_DOWN,
    GSHMUP_PLAYER_LEFT,
    GSHMUP_PLAYER_RIGHT,
};

typedef struct {
    GshmupEntity entity;
    bool shooting;
    bool dir[4]; /* 4 directional movement. */
    float speed;
    int lives;
    int credits;
    int score;
    SCM on_shoot;
    void (*on_game_over) (void);
} GshmupPlayer;

GshmupPlayer *gshmup_create_player (GshmupAnimation *anim);
void gshmup_destroy_player (GshmupPlayer *player);
void gshmup_draw_player (GshmupPlayer *player);
void gshmup_update_player (GshmupPlayer *player);
void gshmup_player_set_direction (GshmupPlayer *player, int dir, bool flag);
void gshmup_player_stop (GshmupPlayer *player);
void gshmup_set_player_shooting (GshmupPlayer *player, bool shoot);
void gshmup_set_current_player (GshmupPlayer *player);
int gshmup_get_initial_player_credits (void);
void gshmup_player_init_scm (void);

#endif
