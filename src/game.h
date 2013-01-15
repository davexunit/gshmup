#ifndef GSHMUP_GAME_H
#define GSHMUP_GAME_H

#include "common.h"
#include "rect.h"
#include "scene.h"

#define GAME_TITLE "GSHMUP"
#define GAME_WIDTH 480
#define GAME_HEIGHT 640
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 640

void gshmup_init_game (void);
void gshmup_run_game (void);
void gshmup_set_current_scene (GshmupScene *scene);
int gshmup_get_fps (void);

#endif
