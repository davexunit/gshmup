#include "shooter.h"
#include "game.h"
#include "player.h"
#include "bullet_system.h"
#include "background.h"

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
static GshmupBulletSystem *player_bullets = NULL;
static GshmupEntity *player = NULL;
static GshmupEntityPool *enemies = NULL;
static GshmupBulletSystem *enemy_bullets = NULL;
static GshmupBackground background;
static GshmupBackground fog;
SCM_VARIABLE_INIT (s_init_hook, "shooter-init-hook",
                   scm_make_hook (scm_from_int (0)));
SCM_VARIABLE_INIT (s_shoot_hook, "player-shoot-hook",
                   scm_make_hook (scm_from_int (0)));
SCM_VARIABLE_INIT (s_player_lives, "player-lives", scm_from_int (3));
SCM_VARIABLE_INIT (s_player_credits, "player-credits", scm_from_int (3));
SCM_VARIABLE_INIT (s_player_speed, "player-speed", scm_from_double (6));
static SCM agenda;

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
    player_sprites = gshmup_create_sprite_sheet (player_image, 32, 32, 0, 0);
    enemy_image = al_load_bitmap ("data/sprites/enemy_1.png");
    enemy_sprites = gshmup_create_sprite_sheet (enemy_image, 32, 32, 0, 0);
    bullet_image = al_load_bitmap ("data/sprites/bullets.png");
    bullet_sprites = gshmup_create_sprite_sheet (bullet_image, 32, 32, 0, 0);
    background_image = al_load_bitmap ("data/sprites/background.png");
    fog_image = al_load_bitmap ("data/sprites/fog.png");
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
    GshmupEntity *entity = gshmup_create_player (player_anim);

    entity->player.lives = scm_to_int (scm_variable_ref (s_player_lives));
    entity->player.credits = scm_to_int (scm_variable_ref (s_player_credits));
    entity->player.speed = scm_to_double (scm_variable_ref (s_player_speed));
    entity->player.strength = 10;
    entity->player.position = gshmup_create_vector2 (GAME_WIDTH / 2, GAME_HEIGHT - 32);
    entity->player.hitbox = gshmup_create_rect (-1, -1, 3, 3);
    player = entity;
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
init_enemies (void)
{
    enemies = gshmup_create_entity_pool (20);
}

static void
init_background (void)
{
    gshmup_init_background (&background, background_image, 1);
    gshmup_init_background (&fog, fog_image, 3);
}

static void
shooter_init (void)
{
    text_color = al_map_rgba_f (1, 1, 1, 1);
    hitbox_fill_color = al_map_rgb_f (1, 1, 1);
    hitbox_border_color = al_map_rgb_f (0, 0, 0);
    load_resources ();
    init_animations ();
    init_player ();
    init_player_bullets ();
    init_enemies ();
    init_enemy_bullets ();
    init_background ();
    agenda = gshmup_create_agenda ();
    gshmup_set_current_agenda (agenda);
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
    gshmup_destroy_bullet_system (enemy_bullets);
    gshmup_destroy_animation (player_anim);
}

static void
draw_bullet_system_stats (GshmupBulletSystem *bullets, const char *name,
                          float y)
{
    al_draw_textf (font, text_color, text_margin, y, 0, "%s Bullets", name);
    al_draw_textf (font, text_color, text_margin, y + text_space, 0, "%d",
                   gshmup_get_bullet_system_size (bullets));
    al_draw_textf (font, text_color, GAME_WIDTH - text_margin, y, ALLEGRO_ALIGN_RIGHT,
                   "%s Bullet Pool", name);
    al_draw_textf (font, text_color, GAME_WIDTH - text_margin, y + text_space,
                   ALLEGRO_ALIGN_RIGHT, "%05d/%05d",
                   gshmup_get_bullet_system_free_size (bullets),
                   gshmup_get_bullet_system_max_free_size (bullets));
}

static void
draw_hud (void)
{
    GshmupPlayer *p = GSHMUP_PLAYER (player);

    al_draw_text (font, text_color, text_margin, text_margin, 0, "Credits");
    al_draw_textf (font, text_color, text_margin, text_margin + text_space, 0,
                   "%d", p->credits);
    al_draw_text (font, text_color, GAME_WIDTH / 2, text_margin, ALLEGRO_ALIGN_CENTRE,
                  "Lives");
    al_draw_textf (font, text_color, GAME_WIDTH / 2, text_margin + text_space,
                   ALLEGRO_ALIGN_CENTRE, "%d", p->lives);
    al_draw_text (font, text_color, GAME_WIDTH - text_margin, text_margin,
                  ALLEGRO_ALIGN_RIGHT, "Score");
    al_draw_textf (font, text_color, GAME_WIDTH - text_margin, text_margin + text_space,
                   ALLEGRO_ALIGN_RIGHT, "%09d", p->score);
    al_draw_textf (font, text_color, GAME_WIDTH - text_margin,
                   GAME_HEIGHT - 12 - text_margin,
                   ALLEGRO_ALIGN_RIGHT, "FPS  %02d", gshmup_get_fps ());
    if (gshmup_debug_mode ()) {
        draw_bullet_system_stats (player_bullets, "Player",
                                  text_margin + text_space * 2);
        draw_bullet_system_stats (enemy_bullets, "Enemy",
                                  text_margin + text_space * 4);
    }
}

static void
draw_player_hitbox (void)
{
    GshmupPlayer *p = GSHMUP_PLAYER (player);

    gshmup_draw_rect (gshmup_rect_move (p->hitbox, p->position),
                      hitbox_fill_color, hitbox_border_color);
}

static void
shooter_draw (void)
{
    gshmup_draw_background (&background);
    gshmup_draw_background (&fog);
    gshmup_draw_bullet_system (player_bullets);
    gshmup_draw_entity (player);
    gshmup_draw_bullet_system (enemy_bullets);
    gshmup_draw_entity_pool (enemies);

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
check_enemy_collisions (void)
{
    GshmupEntity *entity = enemies->active_entities;

    while (entity) {
        GshmupEnemy *enemy = GSHMUP_ENEMY (entity);
        GshmupRect hitbox = gshmup_rect_move (enemy->hitbox, enemy->position);

        gshmup_set_current_enemy (enemy);
        gshmup_bullet_system_collide_rect (player_bullets, hitbox);
        entity = enemy->next;
    }
}

static void
check_player_collisions (void)
{
    GshmupPlayer *p = GSHMUP_PLAYER (player);
    GshmupRect hitbox = gshmup_rect_move (p->hitbox, p->position);

    gshmup_bullet_system_collide_rect (enemy_bullets, hitbox);
}


static void
shooter_update (void)
{
    gshmup_update_agenda (agenda);
    gshmup_update_background (&background);
    gshmup_update_background (&fog);
    gshmup_set_current_bullet_system (player_bullets);
    gshmup_update_entity (player);
    gshmup_update_bullet_system (player_bullets);
    gshmup_set_current_bullet_system (enemy_bullets);
    gshmup_update_entity_pool (enemies);
    gshmup_update_bullet_system (enemy_bullets);
    check_enemy_collisions ();
    check_player_collisions ();
}

static void
player_shoot (void)
{
    GshmupPlayer *p = GSHMUP_PLAYER (player);

    if (!p->shooting) {
        p->shooting = true;
        gshmup_set_current_bullet_system (player_bullets);
        gshmup_set_current_agenda (p->agenda);
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

SCM_DEFINE (spawn_enemy, "spawn-enemy", 4, 0, 0,
            (SCM pos, SCM max_health, SCM hitbox, SCM thunk),
            "Spawn an enemy and run the AI procedure @var{thunk}.")
{
    GshmupEntity *entity = gshmup_entity_pool_new (enemies);

    gshmup_init_enemy (entity, enemy_anim, scm_to_int (max_health));
    entity->enemy.position = gshmup_scm_to_vector2 (pos);
    entity->enemy.hitbox = gshmup_scm_to_rect (hitbox);

    if (scm_is_true (scm_procedure_p (thunk))) {
        gshmup_entity_schedule (entity, 0, thunk);
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

SCM_DEFINE (kill_player, "kill-player", 0, 0, 0,
            (void),
            "Decrement player life counter. Game over if there are no lives left.")
{
    player->player.lives--;

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
                  s_spawn_enemy,
                  s_clear_enemies,
                  s_kill_player,
                  NULL);
}
