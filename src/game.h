#ifndef GSHMUP_GAME_H
#define GSHMUP_GAME_H

#include "common.h"
#include "scene.h"

#define GAME_TITLE "GSHMUP"
/* #define GAME_WIDTH 450 */
/* #define GAME_HEIGHT 600 */
/* #define SCREEN_WIDTH 800 */
/* #define SCREEN_HEIGHT 600 */
#define GAME_WIDTH 240
#define GAME_HEIGHT 320
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 640

/*
 * Controls.
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
void gshmup_set_current_agenda (SCM agenda);
int gshmup_get_fps (void);
void gshmup_game_init_scm (void);
bool gshmup_debug_mode (void);
SCM gshmup_create_agenda (void);
void gshmup_schedule (SCM agenda, int dt, SCM thunk);
void gshmup_update_agenda (SCM agenda);
void gshmup_clear_agenda (SCM agenda);

#endif
