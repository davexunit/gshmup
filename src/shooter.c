#include "shooter.h"
#include "game.h"
#include "player.h"
#include "bullet_system.h"

ALLEGRO_BITMAP *player_image = NULL;
ALLEGRO_BITMAP *enemy_image = NULL;
ALLEGRO_BITMAP *bullet_image = NULL;
ALLEGRO_BITMAP *font_image = NULL;
ALLEGRO_FONT *font = NULL;
GshmupBulletSystem *player_bullets = NULL;
GshmupEntity *player = NULL;
GshmupEntityPool *enemies = NULL;
GshmupBulletSystem *current_bullets = NULL; /* Used by Guile procedures. */
SCM_VARIABLE_INIT (s_init_hook, "shooter-init-hook", scm_make_hook (scm_from_int (0)));
SCM_VARIABLE_INIT (s_shoot_hook, "player-shoot-hook", scm_make_hook (scm_from_int (0)));
SCM_VARIABLE_INIT (s_player_lives, "player-lives", scm_from_int (3));
SCM_VARIABLE_INIT (s_player_credits, "player-credits", scm_from_int (3));
SCM_VARIABLE_INIT (s_player_speed, "player-speed", scm_from_double (6));

static void
load_resources (void)
{
    /* Character code ranges in the bitmap font we're using. */
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
    enemy_image = al_load_bitmap ("data/sprites/enemy.png");
    bullet_image = al_load_bitmap ("data/sprites/bullet.png");
}

static void
init_player (void)
{
    GshmupEntity *entity = gshmup_create_player (player_image);

    entity->player.lives = scm_to_int (scm_variable_ref (s_player_lives));
    entity->player.credits = scm_to_int (scm_variable_ref (s_player_credits));
    entity->player.speed = scm_to_double (scm_variable_ref (s_player_speed));
    entity->player.position = gshmup_create_vector2 (GAME_WIDTH / 2, GAME_HEIGHT - 32);
    player = entity;
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
init_enemies (void)
{
    enemies = gshmup_create_entity_pool (20);
}

static void
shooter_init (void)
{
    load_resources ();
    init_player ();
    init_player_bullets ();
    init_enemies ();
    scm_run_hook (scm_variable_ref (s_init_hook), scm_list_n (SCM_UNDEFINED));
}

static void
shooter_destroy (void)
{
    al_destroy_bitmap (player_image);
    al_destroy_bitmap (font_image);
    al_destroy_font (font);
    gshmup_destroy_entity (player);
    gshmup_destroy_bullet_system (player_bullets);
    gshmup_destroy_entity_pool (enemies);
}

static void
draw_hud (void)
{
    static const float margin = 4;
    ALLEGRO_COLOR color = al_map_rgba_f (1, 1, 1, 1);
    GshmupPlayer *p = GSHMUP_PLAYER (player);

    al_draw_textf (font, color, margin, margin, 0, "Credits  %d", p->credits);
    al_draw_textf (font, color, GAME_WIDTH / 2, margin, ALLEGRO_ALIGN_CENTRE,
                   "Lives  %d", p->lives);
    al_draw_textf (font, color, GAME_WIDTH - margin, margin, ALLEGRO_ALIGN_RIGHT,
                   "Score  %09d", p->score);
    al_draw_textf (font, color, margin, 16, 0, "Player Bullets  %d",
                   gshmup_get_bullet_system_size (player_bullets));
    al_draw_textf (font, color, GAME_WIDTH - margin, 16, ALLEGRO_ALIGN_RIGHT,
                   "Player Bullet Pool  %05d/%05d",
                   gshmup_get_bullet_system_free_size (player_bullets),
                   gshmup_get_bullet_system_max_free_size (player_bullets));
    al_draw_textf (font, color, margin, margin, 0, "Credits  %d", p->credits);
    al_draw_textf (font, color, GAME_WIDTH - margin, GAME_HEIGHT - 12 - margin,
                   ALLEGRO_ALIGN_RIGHT, "FPS  %02d", gshmup_get_fps ());
}

static void
shooter_draw (void)
{
    gshmup_draw_bullet_system (player_bullets);
    gshmup_draw_entity (player);
    gshmup_draw_entity_pool (enemies);
    draw_hud ();
}

static void
shooter_update (void)
{
    current_bullets = player_bullets;
    gshmup_update_entity (player);
    gshmup_update_bullet_system (player_bullets);
    gshmup_update_entity_pool (enemies);
}

static void
player_shoot (void)
{
    GshmupPlayer *p = GSHMUP_PLAYER (player);

    if (!p->shooting) {
        p->shooting = true;
        current_bullets = player_bullets;
        gshmup_set_current_agenda (player->player.agenda);
        scm_run_hook (scm_variable_ref (s_shoot_hook), scm_list_n (SCM_UNDEFINED));
    }
}

static void
player_stop_shoot (void)
{
    GSHMUP_PLAYER (player)->shooting = false;
    gshmup_entity_clear_agenda (player);
}

static void
shooter_key_down (int keycode)
{
    GshmupPlayer *p = GSHMUP_PLAYER (player);

    switch (keycode) {
    case GSHMUP_KEY_UP:
        gshmup_player_set_direction (p, GSHMUP_PLAYER_UP, true);
        break;
    case GSHMUP_KEY_DOWN:
        gshmup_player_set_direction (p, GSHMUP_PLAYER_DOWN, true);
        break;
    case GSHMUP_KEY_LEFT:
        gshmup_player_set_direction (p, GSHMUP_PLAYER_LEFT, true);
        break;
    case GSHMUP_KEY_RIGHT:
        gshmup_player_set_direction (p, GSHMUP_PLAYER_RIGHT, true);
        break;
    case GSHMUP_KEY_SHOOT:
        player_shoot ();
        break;
    }
}

static void
shooter_key_up (int keycode)
{
    GshmupPlayer *p = GSHMUP_PLAYER (player);

    switch (keycode) {
    case GSHMUP_KEY_UP:
        gshmup_player_set_direction (p, GSHMUP_PLAYER_UP, false);
        break;
    case GSHMUP_KEY_DOWN:
        gshmup_player_set_direction (p, GSHMUP_PLAYER_DOWN, false);
        break;
    case GSHMUP_KEY_LEFT:
        gshmup_player_set_direction (p, GSHMUP_PLAYER_LEFT, false);
        break;
    case GSHMUP_KEY_RIGHT:
        gshmup_player_set_direction (p, GSHMUP_PLAYER_RIGHT, false);
        break;
    case GSHMUP_KEY_SHOOT:
        player_stop_shoot ();
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

SCM_DEFINE (player_position, "player-position", 0, 0, 0,
            (void),
            "Return player position vector.")
{
    return gshmup_scm_from_vector2 (GSHMUP_PLAYER (player)->position);
}

SCM_DEFINE (player_shooting_p, "player-shooting?", 0, 0, 0,
            (void),
            "Return @code{#t} if the player is currently shooting.")
{
    return scm_from_bool (GSHMUP_PLAYER (player)->shooting);
}

SCM_DEFINE (emit_bullet, "emit-bullet", 3, 0, 0,
            (SCM pos, SCM speed, SCM direction),
            "Emit a bullet.")
{
    gshmup_emit_bullet (current_bullets, bullet_image,
                        gshmup_scm_to_vector2 (pos), scm_to_double (speed),
                        scm_to_double (direction), 0, 0, 0);

    return SCM_UNSPECIFIED;
}

SCM_DEFINE (spawn_enemy, "spawn-enemy", 2, 0, 0,
            (SCM pos, SCM thunk),
            "Spawn an enemy and run the AI procedure @var{thunk}.")
{
    GshmupEntity *entity = gshmup_entity_pool_new (enemies);

    gshmup_init_enemy (entity, enemy_image);
    entity->enemy.position = gshmup_scm_to_vector2 (pos);

    if (scm_is_true (scm_procedure_p (thunk))) {
        current_bullets = player_bullets;
        gshmup_set_current_agenda (entity->enemy.agenda);
        scm_call_0 (thunk);
    }

    return SCM_UNSPECIFIED;
}

SCM_DEFINE (clear_enemies, "clear-enemies", 0, 0, 0,
            (void),
            "Remove all enemies from the game.")
{
    gshmup_clear_entity_pool (enemies);

    return SCM_UNSPECIFIED;
}

void gshmup_shooter_init_scm (void)
{
#include "shooter.x"

    scm_c_export ("shooter-init-hook",
                  "player-shoot-hook",
                  "player-lives",
                  "player-credits",
                  "player-speed",
                  s_player_position,
                  s_player_shooting_p,
                  s_emit_bullet,
                  s_spawn_enemy,
                  s_clear_enemies,
                  NULL);
}
