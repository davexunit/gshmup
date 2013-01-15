#ifndef GSHMUP_SCENE_H
#define GSHMUP_SCENE_H

/*
 * GshmupScene
 *
 * An interface between high level game state (main menu, shooter, high scores, etc.)
 * and the game draw/update loop.
 */
typedef struct GshmupScene GshmupScene;
struct GshmupScene {
    void (*init)(void);
    void (*destroy)(void);
    void (*draw)(void);
    void (*update)(void);
    void (*key_down)(int);
    void (*key_up)(int);
};

#endif
