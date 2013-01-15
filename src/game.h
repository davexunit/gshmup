#ifndef GSHMUP_GAME_H
#define GSHMUP_GAME_H

#include "common.h"
#include "rect.h"
#include "scene.h"

#define GAME_TITLE "GSHMUP"
#define GAME_WIDTH 450
#define GAME_HEIGHT 600
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

/*
 * Keyboard control keys.
 */
enum {
    GSHMUP_KEY_UP,
    GSHMUP_KEY_DOWN,
    GSHMUP_KEY_LEFT,
    GSHMUP_KEY_RIGHT,
    GSHMUP_KEY_SHOOT,
    GSHMUP_KEY_BOMB,
    GSHMUP_KEY_START,
    GSHMUP_KEY_MAX
};

void gshmup_init_game (void);
void gshmup_run_game (void);
void gshmup_set_current_scene (GshmupScene *scene);
int gshmup_get_fps (void);
void gshmup_game_init_scm (void);

#endif
