#include "shooter.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "stage.h"
#include "bullet_system.h"
#include "background.h"
#include "splash_screen.h"

/* Game states */
enum {
    STATE_MAIN,
    STATE_GAME_OVER,
    STATE_STAGE_CLEAR
};

/* Assets */
static const int text_margin = 4;
static const int text_space = 12;
static ALLEGRO_COLOR text_color;
static ALLEGRO_BITMAP *background_image = NULL;
static ALLEGRO_BITMAP *fog_image = NULL;
static ALLEGRO_BITMAP *player_image = NULL;
static ALLEGRO_BITMAP *enemy_image = NULL;
static ALLEGRO_BITMAP *bullet_image = NULL;
static ALLEGRO_BITMAP *font_image = NULL;
static ALLEGRO_FONT *font = NULL;
static ALLEGRO_COLOR hitbox_fill_color;
static ALLEGRO_COLOR hitbox_border_color;
static GshmupSpriteSheet *bullet_sprites = NULL;
static GshmupSpriteSheet *player_sprites = NULL;
static GshmupSpriteSheet *enemy_sprites = NULL;
static GshmupAnimation *player_anim = NULL;
static GshmupAnimation *enemy_anim = NULL;
static GshmupBackground background;
static GshmupBackground fog;

/* Game objects */
static GshmupStage *stage = NULL;
static GshmupBulletSystem *player_bullets = NULL;
static GshmupBulletSystem *enemy_bullets = NULL;
static GshmupPlayer *player = NULL;
static GshmupEnemy *enemies = NULL;
static int state;

/* Guile */
SCM_VARIABLE (s_stages, "*stages*");
static SCM stage_list = SCM_BOOL_F;

static void
game_over (void)
{
    state = STATE_GAME_OVER;
}

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

    font_image = gshmup_load_image ("data/fonts/font.png");
    font = al_grab_font_from_bitmap (font_image, 6, ranges);
    player_image = gshmup_load_image ("data/sprites/player.png");
    player_sprites = gshmup_create_sprite_sheet (player_image, 32, 32, 0, 0);
    enemy_image = gshmup_load_image ("data/sprites/enemy_1.png");
    enemy_sprites = gshmup_create_sprite_sheet (enemy_image, 32, 32, 0, 0);
    bullet_image = gshmup_load_image ("data/sprites/bullets.png");
    bullet_sprites = gshmup_create_sprite_sheet (bullet_image, 32, 32, 0, 0);
    background_image = gshmup_load_image ("data/sprites/background.png");
    fog_image = gshmup_load_image ("data/sprites/fog.png");
}

static void
init_animations (void)
{
    int player_frames[] = { 0, 1, 2, 3 };
    int enemy_frames[] = { 0, 1, 2, 3 };

    player_anim = gshmup_create_animation (player_sprites, 2, 4, player_frames,
                                           GSHMUP_ANIM_LOOP);
    enemy_anim = gshmup_create_animation (enemy_sprites, 2, 4, enemy_frames,
                                          GSHMUP_ANIM_LOOP);
}

static void
init_player (void)
{
    player = gshmup_create_player (player_anim);
    player->entity.position = gshmup_create_vector2 (GAME_WIDTH / 2,
                                                     GAME_HEIGHT - 32);
    player->entity.hitbox = gshmup_create_rect (-1, -1, 3, 3);
    player->on_game_over = game_over;
    gshmup_set_current_player (player);
}

static void
init_enemies (void)
{
    enemies = NULL;
}

static void
init_player_bullets (void)
{
    static const float margin = 64;

    player_bullets = gshmup_create_bullet_system (400, bullet_sprites);
    player_bullets->bounds = gshmup_create_rect (-margin,
                                                 -margin,
                                                 GAME_WIDTH + margin,
                                                 GAME_HEIGHT + margin);
}

static void
init_enemy_bullets (void)
{
    static const float margin = 64;

    enemy_bullets = gshmup_create_bullet_system (3000, bullet_sprites);
    enemy_bullets->bounds = gshmup_create_rect (-margin,
                                                -margin,
                                                GAME_WIDTH + margin,
                                                GAME_HEIGHT + margin);
}

static void
init_background (void)
{
    gshmup_init_background (&background, background_image, 1);
    gshmup_init_background (&fog, fog_image, 3);
}

static void
set_current_stage (void)
{
    stage = gshmup_check_stage_smob (scm_car (stage_list));
    gshmup_start_stage (stage);
}

static void
next_stage (void)
{
    stage_list = scm_cdr (stage_list);

    if (scm_is_null (stage_list)) {
        gshmup_set_current_scene (gshmup_create_splash_screen_scene ());
    } else {
        set_current_stage ();
        state = STATE_MAIN;
    }
}


static void
init_stage (void)
{
    /* We iterate over *stages* and store the current pair in stage_list. */
    stage_list = scm_variable_ref (s_stages);
    set_current_stage ();
}

static void
shooter_init (void)
{
    state = STATE_MAIN;
    text_color = al_map_rgba_f (1, 1, 1, 1);
    hitbox_fill_color = al_map_rgb_f (1, 1, 1);
    hitbox_border_color = al_map_rgb_f (0, 0, 0);
    load_resources ();
    init_animations ();
    init_player ();
    init_enemies ();
    init_player_bullets ();
    init_enemy_bullets ();
    init_background ();
    init_stage ();
}

static void
shooter_destroy (void)
{
    al_destroy_bitmap (player_image);
    al_destroy_bitmap (font_image);
    al_destroy_font (font);
    gshmup_destroy_player (player);
    gshmup_destroy_bullet_system (player_bullets);
    gshmup_destroy_enemies (enemies);
    gshmup_destroy_bullet_system (enemy_bullets);
    gshmup_destroy_animation (player_anim);
}

static void
draw_bullet_system_stats (GshmupBulletSystem *bullets, const char *name,
                          float x, float y, int flags)
{
    al_draw_textf (font, text_color, x, y, flags, "%s Bullets", name);
    al_draw_textf (font, text_color, x, y + text_space, flags, "%d",
                   gshmup_get_bullet_system_size (bullets));
}

static void
draw_hud (void)
{
    al_draw_text (font, text_color, text_margin, text_margin, 0, "Credits");
    al_draw_textf (font, text_color, text_margin, text_margin + text_space, 0,
                   "%d", player->credits);
    al_draw_text (font, text_color, GAME_WIDTH / 2, text_margin, ALLEGRO_ALIGN_CENTRE,
                  "Lives");
    al_draw_textf (font, text_color, GAME_WIDTH / 2, text_margin + text_space,
                   ALLEGRO_ALIGN_CENTRE, "%d", player->lives);
    al_draw_text (font, text_color, GAME_WIDTH - text_margin, text_margin,
                  ALLEGRO_ALIGN_RIGHT, "Score");
    al_draw_textf (font, text_color, GAME_WIDTH - text_margin, text_margin + text_space,
                   ALLEGRO_ALIGN_RIGHT, "%09d", player->score);
    al_draw_textf (font, text_color, GAME_WIDTH - text_margin,
                   GAME_HEIGHT - 12 - text_margin,
                   ALLEGRO_ALIGN_RIGHT, "FPS  %05.2f", gshmup_get_fps ());

    if (gshmup_debug_mode ()) {
        draw_bullet_system_stats (player_bullets, "Player",
                                  text_margin, text_margin + text_space * 2, 0);
        draw_bullet_system_stats (enemy_bullets, "Enemy",
                                  GAME_WIDTH - text_margin,
                                  text_margin + text_space * 2,
                                  ALLEGRO_ALIGN_RIGHT);
    }
}

static void
draw_player_hitbox (void)
{
    gshmup_draw_rect (gshmup_rect_move (player->entity.hitbox,
                                        player->entity.position),
                      hitbox_fill_color, hitbox_border_color);
}

static void
draw_main (void)
{
    gshmup_draw_background (&background);
    gshmup_draw_background (&fog);
    gshmup_draw_bullet_system (player_bullets);
    gshmup_draw_player (player);
    gshmup_draw_bullet_system (enemy_bullets);
    gshmup_draw_enemies (enemies);

    if (gshmup_debug_mode ()) {
        draw_player_hitbox ();
        gshmup_draw_bullet_system_hitboxes (player_bullets, hitbox_fill_color,
                                            hitbox_border_color);
        gshmup_draw_bullet_system_hitboxes (enemy_bullets, hitbox_fill_color,
                                            hitbox_border_color);
    }

    draw_hud ();
}

static void
draw_game_over_message ()
{
    al_draw_text (font, text_color, GAME_WIDTH / 2, GAME_HEIGHT / 2,
                  ALLEGRO_ALIGN_CENTRE, "GAME OVER");
}

static void
draw_game_over (void)
{
    gshmup_draw_background (&background);
    gshmup_draw_background (&fog);
    gshmup_draw_bullet_system (player_bullets);
    gshmup_draw_bullet_system (enemy_bullets);
    gshmup_draw_enemies (enemies);
    draw_hud ();
    draw_game_over_message ();
}

static void
draw_stage_clear_message (void)
{
    al_draw_text (font, text_color, GAME_WIDTH / 2, GAME_HEIGHT / 2,
                  ALLEGRO_ALIGN_CENTRE, "STAGE CLEAR!");
}

static void
draw_stage_clear (void)
{
    gshmup_draw_background (&background);
    gshmup_draw_background (&fog);
    gshmup_draw_bullet_system (player_bullets);
    gshmup_draw_player (player);
    gshmup_draw_bullet_system (enemy_bullets);
    gshmup_draw_enemies (enemies);
    draw_stage_clear_message ();
}

static void
shooter_draw (void)
{
    switch (state) {
    case STATE_MAIN:
        draw_main ();
        break;
    case STATE_GAME_OVER:
        draw_game_over ();
        break;
    case STATE_STAGE_CLEAR:
        draw_stage_clear ();
        break;
    }
}

static void
check_enemy_collisions (void)
{
    GshmupEnemy *enemy = enemies;

    while (enemy) {
        GshmupRect hitbox = gshmup_rect_move (enemy->entity.hitbox,
                                              enemy->entity.position);

        gshmup_set_current_enemy (enemy);
        gshmup_bullet_system_collide_rect (player_bullets, hitbox);
        enemy = enemy->next;
    }
}

static void
check_player_collisions (void)
{
    GshmupRect hitbox = gshmup_rect_move (player->entity.hitbox,
                                          player->entity.position);

    gshmup_bullet_system_collide_rect (enemy_bullets, hitbox);
}


static void
update_main (void)
{
    gshmup_update_stage (stage);
    gshmup_update_background (&background);
    gshmup_update_background (&fog);
    gshmup_set_current_bullet_system (player_bullets);
    gshmup_update_player (player);
    gshmup_update_bullet_system (player_bullets);
    gshmup_set_current_bullet_system (enemy_bullets);
    enemies = gshmup_update_enemies (enemies);
    gshmup_update_bullet_system (enemy_bullets);
    check_enemy_collisions ();
    check_player_collisions ();
}

static void
update_game_over (void)
{
    gshmup_update_background (&background);
    gshmup_update_background (&fog);
    gshmup_set_current_bullet_system (player_bullets);
    gshmup_update_bullet_system (player_bullets);
    gshmup_set_current_bullet_system (enemy_bullets);
    enemies = gshmup_update_enemies (enemies);
    gshmup_update_bullet_system (enemy_bullets);
}

static void
update_stage_clear (void)
{
    gshmup_update_background (&background);
    gshmup_update_background (&fog);
    gshmup_set_current_bullet_system (player_bullets);
    gshmup_update_player (player);
    gshmup_update_bullet_system (player_bullets);
}

static void
shooter_update (void)
{
    switch (state) {
    case STATE_MAIN:
        update_main ();
        break;
    case STATE_GAME_OVER:
        update_game_over ();
        break;
    case STATE_STAGE_CLEAR:
        update_stage_clear ();
        break;
    }
}

static void
key_down_main (int keycode)
{
    switch (keycode) {
    case GSHMUP_KEY_UP:
        gshmup_player_set_direction (player, GSHMUP_PLAYER_UP, true);
        break;
    case GSHMUP_KEY_DOWN:
        gshmup_player_set_direction (player, GSHMUP_PLAYER_DOWN, true);
        break;
    case GSHMUP_KEY_LEFT:
        gshmup_player_set_direction (player, GSHMUP_PLAYER_LEFT, true);
        break;
    case GSHMUP_KEY_RIGHT:
        gshmup_player_set_direction (player, GSHMUP_PLAYER_RIGHT, true);
        break;
    case GSHMUP_KEY_SHOOT:
        gshmup_set_player_shooting (player, true);
        break;
    }
}

static void
key_down_game_over (int keycode)
{
    switch (keycode) {
    case GSHMUP_KEY_SHOOT:
    case GSHMUP_KEY_START:
        gshmup_set_current_scene (gshmup_create_splash_screen_scene ());
        break;
    }
}

static void
key_down_stage_clear (int keycode)
{
    switch (keycode) {
    case GSHMUP_KEY_SHOOT:
    case GSHMUP_KEY_START:
        next_stage ();
        break;
    }
}

static void
shooter_key_down (int keycode)
{
    switch (state) {
    case STATE_MAIN:
        key_down_main (keycode);
        break;
    case STATE_GAME_OVER:
        key_down_game_over (keycode);
        break;
    case STATE_STAGE_CLEAR:
        key_down_stage_clear (keycode);
        break;
    }
}

static void
key_up_main (int keycode)
{
    switch (keycode) {
    case GSHMUP_KEY_UP:
        gshmup_player_set_direction (player, GSHMUP_PLAYER_UP, false);
        break;
    case GSHMUP_KEY_DOWN:
        gshmup_player_set_direction (player, GSHMUP_PLAYER_DOWN, false);
        break;
    case GSHMUP_KEY_LEFT:
        gshmup_player_set_direction (player, GSHMUP_PLAYER_LEFT, false);
        break;
    case GSHMUP_KEY_RIGHT:
        gshmup_player_set_direction (player, GSHMUP_PLAYER_RIGHT, false);
        break;
    case GSHMUP_KEY_SHOOT:
        gshmup_set_player_shooting (player, false);
        break;
    }
}

static void
shooter_key_up (int keycode)
{
    switch (state) {
    case STATE_MAIN:
        key_up_main (keycode);
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
    return gshmup_scm_from_vector2 (player->entity.position);
}

SCM_DEFINE (player_shooting_p, "player-shooting?", 0, 0, 0,
            (void),
            "Return @code{#t} if the player is currently shooting.")
{
    return scm_from_bool (player->shooting);
}

SCM_DEFINE (spawn_enemy, "spawn-enemy", 3, 0, 0,
            (SCM type, SCM position, SCM script),
            "Spawn an enemy and run the AI procedure @var{script}.")
{
    GshmupEnemyType *enemy_type = gshmup_check_enemy_type_smob (type);
    GshmupEnemy *enemy = gshmup_create_enemy (enemy_anim, enemy_type, script);

    enemy->entity.position = gshmup_scm_to_vector2 (position);
    enemy->next = enemies;
    enemies = enemy;

    return SCM_UNSPECIFIED;
}

SCM_DEFINE (clear_enemies, "clear-enemies", 0, 0, 0,
            (void),
            "Remove all enemies from the game.")
{
    gshmup_destroy_enemies (enemies);
    enemies = NULL;

    return SCM_UNSPECIFIED;
}

SCM_DEFINE (end_stage, "end-stage", 0, 0, 0,
            (void),
            "Ends the current stage. Typically called when the boss dies.")
{
    state = STATE_STAGE_CLEAR;
    gshmup_player_stop (player);
    player->shooting = false;
    gshmup_destroy_enemies (enemies);
    enemies = NULL;

    return SCM_UNSPECIFIED;
}

void gshmup_shooter_init_scm (void)
{
#include "shooter.x"

    scm_c_export ("player-shoot-hook",
                  "*stages*",
                  s_player_position,
                  s_player_shooting_p,
                  s_spawn_enemy,
                  s_clear_enemies,
                  s_end_stage,
                  NULL);
}
