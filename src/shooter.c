#include "shooter.h"
#include "game.h"
#include "player.h"
#include "bullet_system.h"

ALLEGRO_BITMAP *player_image = NULL;
ALLEGRO_BITMAP *bullet_image = NULL;
ALLEGRO_BITMAP *font_image = NULL;
ALLEGRO_FONT *font = NULL;
GshmupPlayer player;
GshmupBulletSystem *player_bullets = NULL;
SCM_VARIABLE_INIT (init_hook, "shooter-init-hook", scm_make_hook (scm_from_int (0)));

static void
load_resources (void)
{
    int ranges[] = {
        32, 47,
        48, 63,
        64, 79,
        80, 95,
        96, 111,
        112, 127,
    };

    font_image = al_load_bitmap ("data/fonts/font.png");
    font = al_grab_font_from_bitmap (font_image, 6, ranges);
    player_image = al_load_bitmap ("data/sprites/player.png");
    bullet_image = al_load_bitmap ("data/sprites/bullet.png");
}

static void
init_player (void)
{
    gshmup_init_player (&player, player_image);
    player.credits = 3;
    player.speed = 5;
    player.position = gshmup_create_vector2 (GAME_WIDTH / 2, GAME_HEIGHT - 32);
}

static void
init_player_bullets (void)
{
    static const float margin = 64;

    player_bullets = gshmup_create_bullet_system (1000);
    player_bullets->bounds = gshmup_create_rect (-margin,
                                                 -margin,
                                                 GAME_WIDTH + margin,
                                                 GAME_HEIGHT + margin);
}

static void
shooter_init (void)
{
    load_resources ();
    init_player ();
    init_player_bullets ();
    scm_run_hook (scm_variable_ref (init_hook), scm_list_n (SCM_UNDEFINED));
}

static void
shooter_destroy (void)
{
    al_destroy_bitmap (player_image);
    al_destroy_bitmap (font_image);
    al_destroy_font (font);
}

static void
draw_hud (void)
{
    static const float margin = 4;
    ALLEGRO_COLOR color = al_map_rgba_f (1, 1, 1, 1);

    al_draw_textf (font, color, margin, margin, 0, "Credits  %d", player.credits);
    al_draw_textf (font, color, GAME_WIDTH - margin, margin, ALLEGRO_ALIGN_RIGHT,
                   "Score  %09d", player.score);
    al_draw_textf (font, color, margin, 16, 0, "Player Bullets  %d",
                   player_bullets->bullet_count);
    al_draw_textf (font, color, GAME_WIDTH - margin, 16, ALLEGRO_ALIGN_RIGHT,
                   "Player Bullet Pool  %d/%d",
                   player_bullets->pool_size, player_bullets->max_pool_size);
    al_draw_textf (font, color, margin, margin, 0, "Credits  %d", player.credits);
    al_draw_textf (font, color, GAME_WIDTH - margin, GAME_HEIGHT - 12 - margin,
                   ALLEGRO_ALIGN_RIGHT, "FPS  %02d", gshmup_get_fps ());
}

static void
shooter_draw (void)
{
    gshmup_draw_bullet_system (player_bullets);
    gshmup_draw_player (&player);
    draw_hud ();
}

static void
shooter_update (void)
{
    gshmup_update_player (&player);
    gshmup_update_bullet_system (player_bullets);
}

static void
emit_bullet (void)
{
    GshmupVector2 pos = player.position;
    float speed = 12;

    gshmup_emit_bullet (player_bullets, bullet_image, pos, speed, 268, 0, 0, 0);
    gshmup_emit_bullet (player_bullets, bullet_image, pos, speed, 270, 0, 0, 0);
    gshmup_emit_bullet (player_bullets, bullet_image, pos, speed, 272, 0, 0, 0);
}

static void
shooter_key_down (int keycode)
{
    switch (keycode) {
    case ALLEGRO_KEY_UP:
        gshmup_player_set_direction (&player, GSHMUP_PLAYER_UP, true);
        break;
    case ALLEGRO_KEY_DOWN:
        gshmup_player_set_direction (&player, GSHMUP_PLAYER_DOWN, true);
        break;
    case ALLEGRO_KEY_LEFT:
        gshmup_player_set_direction (&player, GSHMUP_PLAYER_LEFT, true);
        break;
    case ALLEGRO_KEY_RIGHT:
        gshmup_player_set_direction (&player, GSHMUP_PLAYER_RIGHT, true);
        break;
    case ALLEGRO_KEY_Z:
        emit_bullet ();
        break;
    }
}

static void
shooter_key_up (int keycode)
{
    switch (keycode) {
    case ALLEGRO_KEY_UP:
        gshmup_player_set_direction (&player, GSHMUP_PLAYER_UP, false);
        break;
    case ALLEGRO_KEY_DOWN:
        gshmup_player_set_direction (&player, GSHMUP_PLAYER_DOWN, false);
        break;
    case ALLEGRO_KEY_LEFT:
        gshmup_player_set_direction (&player, GSHMUP_PLAYER_LEFT, false);
        break;
    case ALLEGRO_KEY_RIGHT:
        gshmup_player_set_direction (&player, GSHMUP_PLAYER_RIGHT, false);
        break;
    }
}

GshmupScene *
gshmup_create_shooter_scene (void)
{
    GshmupScene *scene = (GshmupScene *) malloc (sizeof (GshmupScene));

    scene->init = shooter_init;
    scene->destroy = shooter_destroy;
    scene->draw = shooter_draw;
    scene->update = shooter_update;
    scene->key_down = shooter_key_down;
    scene->key_up = shooter_key_up;

    return scene;
}

void gshmup_shooter_init_scm (void)
{
#include "shooter.x"

    scm_c_export ("shooter-init-hook", NULL);
}
