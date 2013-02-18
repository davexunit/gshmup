#include "bullet_system.h"

static scm_t_bits bullet_type_tag;
static GshmupBulletSystem *current_bullets = NULL;
static GshmupBullet *current_bullet = NULL;
SCM_SYMBOL (sym_blend_alpha, "alpha");
SCM_SYMBOL (sym_blend_add, "add");

static int
scm_to_blend_mode (SCM blend_mode)
{
    int mode;

    if (scm_is_true (scm_eq_p (blend_mode, sym_blend_alpha))) {
        mode = GSHMUP_BULLET_BLEND_ALPHA;
    } else if (scm_is_true (scm_eq_p (blend_mode, sym_blend_add))) {
        mode = GSHMUP_BULLET_BLEND_ADD;
    } else {
        scm_error_scm (scm_from_latin1_symbol ("blend-mode-error"),
                       SCM_BOOL_F,
                       scm_from_locale_string ("Unrecognized blend mode"),
                       SCM_EOL, SCM_BOOL_F);
    }

    return mode;
}

/* static void */
/* set_blend_mode (int blend_mode) */
/* { */
/*     switch (blend_mode) { */
/*     case GSHMUP_BULLET_BLEND_ALPHA: */
/*         al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA); */
/*         break; */
/*     case GSHMUP_BULLET_BLEND_ADD: */
/*         al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_ONE); */
/*         break; */
/*     } */
/* } */

GshmupBulletType *
check_bullet_type (SCM bullet_type)
{
    scm_assert_smob_type (bullet_type_tag, bullet_type);

    return (GshmupBulletType *) SCM_SMOB_DATA (bullet_type);
}

SCM_DEFINE (make_bullet_type, "make-bullet-type", 5, 0, 0,
            (SCM tile, SCM hitbox, SCM blend_mode, SCM directional, SCM on_hit),
            "Make a new bullet type instance.")
{
    GshmupBulletType *bullet_type;

    bullet_type = (GshmupBulletType *) scm_gc_malloc (sizeof (GshmupBulletType),
                                                      "bullet_type");
    bullet_type->tile = scm_to_int (tile);
    bullet_type->hitbox = gshmup_scm_to_rect (hitbox);
    bullet_type->blend_mode = scm_to_blend_mode (blend_mode);
    bullet_type->directional = scm_to_bool (directional);
    bullet_type->on_hit = on_hit;

    SCM_RETURN_NEWSMOB (bullet_type_tag, bullet_type);
}

static size_t
free_bullet_type (SCM bullet_type_smob)
{
    GshmupBulletType *bullet_type = (GshmupBulletType *) SCM_SMOB_DATA (bullet_type_smob);

    scm_gc_free (bullet_type, sizeof (GshmupBulletType), "bullet_type");

    return 0;
}

static int
print_bullet_type (SCM bullet_type_smob, SCM port, scm_print_state *pstate)
{
   scm_puts ("#<bullet-type>", port);

   return 1;
}

static void
update_bullet_movement (GshmupBullet *bullet) {
    float theta = gshmup_deg_to_rad (bullet->direction);

    bullet->vel = gshmup_vector2_from_polar (bullet->speed, theta);
    bullet->acc = gshmup_vector2_from_polar (bullet->acceleration, theta);
    al_build_transform (&bullet->ang_vel, 0, 0, 1, 1,
                        gshmup_deg_to_rad (bullet->angular_velocity));
}

static bool
bullet_out_of_bounds (GshmupBullet *bullet)
{
    return !gshmup_rect_collide_point (bullet->parent->bounds, bullet->pos);
}

void
gshmup_draw_bullet (GshmupBullet *bullet)
{
    bullet->sprite.position = bullet->pos;
    gshmup_draw_sprite (&bullet->sprite);
}

static bool
bullet_out_of_time (GshmupBullet *bullet)
{
    return bullet->life != 0 && bullet->life_count >= bullet->life;
}

static bool
is_bullet_dead (GshmupBullet *bullet)
{
    return bullet->kill ||
        bullet_out_of_bounds (bullet) ||
        bullet_out_of_time (bullet);
}

void
gshmup_update_bullet (GshmupBullet *bullet)
{
    current_bullet = bullet;
    al_transform_coordinates (&bullet->ang_vel,
                              &bullet->vel.x, &bullet->vel.y);
    al_transform_coordinates (&bullet->ang_vel,
                              &bullet->acc.x, &bullet->acc.y);
    bullet->pos = gshmup_vector2_add (bullet->pos, bullet->vel);
    bullet->vel = gshmup_vector2_add (bullet->vel, bullet->acc);
    bullet->life_count++;
    bullet->kill = is_bullet_dead (bullet);

    if (bullet->directional) {
        bullet->sprite.rotation = gshmup_deg_to_rad (bullet->direction);
    }
}

GshmupBulletSystem *
gshmup_create_bullet_system (int reserved_size, GshmupSpriteSheet *sprite_sheet)
{
    GshmupBulletSystem *system;

    system = (GshmupBulletSystem *) scm_gc_malloc (sizeof (GshmupBulletSystem),
                                                   "bullet system");
    system->sprite_sheet = sprite_sheet;
    system->bullets = g_array_sized_new (false, false, sizeof (GshmupBullet),
                                         reserved_size);
    system->agendas = NULL;
    system->bounds = gshmup_create_rect (0, 0, 0, 0);
    system->len = 0;

    return system;
}

void
gshmup_destroy_bullet_system (GshmupBulletSystem *system)
{
    g_array_free (system->bullets, true);
    scm_gc_free (system, sizeof (GshmupBulletSystem), "bullet system");
}

void
gshmup_draw_bullet_system (GshmupBulletSystem *system)
{
    al_hold_bitmap_drawing (true);

    for (int i = 0; i < system->bullets->len; ++i) {
        GshmupBullet *bullet = &g_array_index (system->bullets, GshmupBullet, i);

        gshmup_draw_bullet (bullet);
    }

    al_hold_bitmap_drawing (false);
}

void
gshmup_draw_bullet_system_hitboxes (GshmupBulletSystem *system,
                                    ALLEGRO_COLOR fill_color,
                                    ALLEGRO_COLOR border_color)
{
    for (int i = 0; i < system->bullets->len; ++i) {
        GshmupBullet *bullet = &g_array_index (system->bullets, GshmupBullet, i);
        GshmupRect hitbox = gshmup_rect_move (bullet->hitbox, bullet->pos);

        gshmup_draw_rect (hitbox, fill_color, border_color);
    }
}

static void
remove_agenda (GshmupBulletSystem *system, GshmupBullet *bullet)
{
    /*
     * Removes the agenda from the agendas list.
     *
     * Right now it's just a shitty linear time search.
     */
    GshmupBulletAgenda *agenda = system->agendas;
    GshmupBulletAgenda *prev = NULL;
    GshmupBulletAgenda *next = NULL;

    while (agenda) {
        next = agenda->next;

        /* DESTROY! */
        if (agenda == bullet->agenda) {
            scm_gc_free (agenda, sizeof (GshmupBulletAgenda), "bullet agenda");

            if (prev) {
                prev->next = next;
            } else {
                system->agendas = next;
            }

            return;
        }

        prev = agenda;
        agenda = next;
    }
}

void
gshmup_update_bullet_system (GshmupBulletSystem *system)
{
    for (int i = 0; i < system->bullets->len; ++i) {
        GshmupBullet *bullet = &g_array_index (system->bullets, GshmupBullet, i);

        gshmup_update_bullet (bullet);

        if (bullet->agenda) {
            gshmup_update_agenda (bullet->agenda->agenda);
        }

        if (bullet->kill) {
            remove_agenda (system, bullet);
            g_array_remove_index_fast (system->bullets, i--);
        }
    }
}

void
gshmup_set_bullet_type (GshmupBullet *bullet, GshmupBulletType *type)
{
    GshmupBulletSystem *parent = bullet->parent;
    ALLEGRO_BITMAP *image = gshmup_get_sprite_sheet_tile (parent->sprite_sheet,
                                                          type->tile);

    bullet->sprite = gshmup_create_sprite (image);
    bullet->directional = type->directional;
    bullet->hitbox = type->hitbox;
    bullet->blend_mode = type->blend_mode;
    bullet->on_hit = type->on_hit;
}

static void
add_agenda (GshmupBulletSystem *system, GshmupBullet *bullet)
{
    GshmupBulletAgenda *bullet_agenda =
        (GshmupBulletAgenda *) scm_gc_malloc (sizeof (GshmupBulletAgenda),
                                              "bullet agenda");

    bullet->agenda = bullet_agenda;
    bullet_agenda->agenda = gshmup_create_agenda ();
    bullet_agenda->next = system->agendas;
    system->agendas = bullet_agenda;
}

void
gshmup_emit_bullet (GshmupBulletSystem *system, GshmupVector2 position,
                    float speed, float direction, float acceleration,
                    float angular_velocity, float life, GshmupBulletType *type,
                    SCM thunk)
{
    GshmupBullet bullet;

    bullet.parent = system;
    bullet.life = life;
    bullet.life_count = 0;
    bullet.blend_mode = 0;
    bullet.kill = false;
    bullet.pos = position;
    bullet.life_count = 0;
    bullet.kill = false;
    bullet.agenda = NULL;
    bullet.speed = speed;
    bullet.direction = direction;
    bullet.acceleration = acceleration;
    bullet.angular_velocity = angular_velocity;
    gshmup_set_bullet_type (&bullet, type);
    update_bullet_movement (&bullet);

    /* Only scripted bullets require an agenda. */
    if (scm_is_true (thunk) && scm_is_false (scm_eq_p (thunk, SCM_UNDEFINED))) {
        add_agenda (system, &bullet);
        gshmup_schedule (bullet.agenda->agenda, 0, thunk);
    }

    g_array_append_val (system->bullets, bullet);
}

int
gshmup_get_bullet_system_size (GshmupBulletSystem *system)
{
    return system->bullets->len;
}

void
gshmup_set_current_bullet_system (GshmupBulletSystem *system)
{
    current_bullets = system;
}

static bool
bullet_collision_check (GshmupBullet *bullet, GshmupRect rect)
{
    GshmupRect hitbox = gshmup_rect_move (bullet->hitbox, bullet->pos);

    return gshmup_rect_collide_rect (hitbox, rect);
}

void
gshmup_bullet_system_collide_rect (GshmupBulletSystem *system, GshmupRect rect)
{
    for (int i = 0; i < system->bullets->len; ++i) {
        GshmupBullet *bullet = &g_array_index (system->bullets, GshmupBullet, i);

        if (bullet_collision_check (bullet, rect)) {
            bullet->kill = true;

            /* Call bullet collision Guile callback procedure. */
            if (scm_is_true (scm_procedure_p (bullet->on_hit))) {
                scm_call_0 (bullet->on_hit);
            }

            return;
        }
    }
}

SCM_DEFINE (emit_bullet, "%emit-bullet", 8, 1, 0,
            (SCM type, SCM pos, SCM speed, SCM direction, SCM acceleration,
             SCM angular_velocity, SCM life, SCM script),
            "Emit a bullet.")
{
    gshmup_emit_bullet (current_bullets,
                        gshmup_scm_to_vector2 (pos),
                        scm_to_double (speed),
                        scm_to_double (direction),
                        scm_to_double (acceleration),
                        scm_to_double (angular_velocity),
                        scm_to_double (life),
                        check_bullet_type (type),
                        script);

    return SCM_UNSPECIFIED;
}

SCM_DEFINE (bullet_position, "bullet-position", 0, 0, 0,
            (void),
            "Return bullet position.")
{
    return gshmup_scm_from_vector2 (current_bullet->pos);
}

SCM_DEFINE (bullet_speed, "bullet-speed", 0, 0, 0,
            (void),
            "Return the current bullet's speed.")
{
    return scm_from_double (gshmup_vector2_mag (current_bullet->vel));
}

SCM_DEFINE (bullet_direction, "bullet-direction", 0, 0, 0,
            (void),
            "Return the current bullet's direction in degrees.")
{
    return scm_from_double (current_bullet->direction);
}

SCM_DEFINE (bullet_acceleration, "bullet-acceleration", 0, 0, 0,
            (void),
            "Return the current bullet's acceleration.")
{
    return scm_from_double (current_bullet->acceleration);
}

SCM_DEFINE (bullet_angular_velocity, "bullet-angular-velocity", 0, 0, 0,
            (void),
            "Return the current bullet's angular velocity in degrees.")
{
    return scm_from_double (current_bullet->angular_velocity);
}

SCM_DEFINE (set_bullet_position, "set-bullet-position", 1, 0, 0,
            (SCM position),
            "Set the current bullet's position.")
{
    current_bullet->pos = gshmup_scm_to_vector2 (position);

    return SCM_UNSPECIFIED;
}

SCM_DEFINE (set_bullet_speed, "set-bullet-speed", 1, 0, 0,
            (SCM speed),
            "Set the current bullet's speed.")
{
    current_bullet->speed = scm_to_double (speed);
    update_bullet_movement (current_bullet);

    return SCM_UNSPECIFIED;
}

SCM_DEFINE (set_bullet_direction, "set-bullet-direction", 1, 0, 0,
            (SCM direction),
            "Set the current bullet's direction.")
{
    current_bullet->direction = scm_to_double (direction);
    update_bullet_movement (current_bullet);

    return SCM_UNSPECIFIED;
}

SCM_DEFINE (set_bullet_acceleration, "set-bullet-acceleration", 1, 0, 0,
            (SCM acceleration),
            "Set the current bullet's acceleration.")
{
    current_bullet->acceleration = scm_to_double (acceleration);
    update_bullet_movement (current_bullet);

    return SCM_UNSPECIFIED;
}

SCM_DEFINE (set_bullet_angular_velocity, "set-bullet-angular-velocity", 1, 0, 0,
            (SCM angular_velocity),
            "Set the current bullet's angular velocity.")
{
    current_bullet->angular_velocity = scm_to_double (angular_velocity);
    update_bullet_movement (current_bullet);

    return SCM_UNSPECIFIED;
}

SCM_DEFINE (set_bullet_life, "set-bullet-life", 1, 0, 0,
            (SCM life),
            "Set the current bullet's time to live.")
{
    current_bullet->life = scm_to_int (life);

    return SCM_UNSPECIFIED;
}

SCM_DEFINE (set_bullet_type, "set-bullet-type", 1, 0, 0,
            (SCM type),
            "Set the current bullet's type")
{
    gshmup_set_bullet_type (current_bullet, check_bullet_type (type));

    return SCM_UNSPECIFIED;
}

SCM_DEFINE (kill_bullet, "kill-bullet", 0, 0, 0,
            (void),
            "Sets kill flag for the current bullet.")
{
    current_bullet->kill = true;

    return SCM_UNSPECIFIED;
}

void
gshmup_bullet_system_init_scm (void)
{
    bullet_type_tag = scm_make_smob_type ("<bullet-type>", sizeof (GshmupBulletType));
    scm_set_smob_mark (bullet_type_tag, 0);
    scm_set_smob_free (bullet_type_tag, free_bullet_type);
    scm_set_smob_print (bullet_type_tag, print_bullet_type);

#include "bullet_system.x"

    scm_c_export (s_make_bullet_type,
                  s_emit_bullet,
                  s_bullet_position,
                  s_bullet_speed,
                  s_bullet_direction,
                  s_bullet_acceleration,
                  s_bullet_angular_velocity,
                  s_set_bullet_position,
                  s_set_bullet_speed,
                  s_set_bullet_direction,
                  s_set_bullet_acceleration,
                  s_set_bullet_angular_velocity,
                  s_set_bullet_life,
                  s_set_bullet_type,
                  s_kill_bullet,
                  NULL);
}
