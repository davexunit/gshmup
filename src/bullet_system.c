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

static void
set_blend_mode (int blend_mode)
{
    switch (blend_mode) {
    case GSHMUP_BULLET_BLEND_ALPHA:
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);
        break;
    case GSHMUP_BULLET_BLEND_ADD:
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_ONE);
        break;
    }
}

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
init_bullet_movement (GshmupBullet *bullet, float speed, float direction,
                      float acceleration, float angular_velocity)
{
    float theta = gshmup_deg_to_rad (direction);

    bullet->vel = gshmup_vector2_from_polar (speed, theta);
    bullet->acc = gshmup_vector2_from_polar (acceleration, theta);
    al_build_transform (&bullet->angular_velocity, 0, 0, 1, 1,
                        gshmup_deg_to_rad (angular_velocity));
}

static bool
bullet_out_of_bounds (GshmupBullet *bullet)
{
    return !gshmup_rect_collide_point (bullet->parent->bounds, bullet->position);
}

void
gshmup_draw_bullet (GshmupBullet *bullet)
{
    bullet->sprite.position = bullet->position;
    gshmup_draw_sprite (&bullet->sprite);
}

void
gshmup_update_bullet (GshmupBullet *bullet)
{
    current_bullet = bullet;
    al_transform_coordinates (&bullet->angular_velocity,
                              &bullet->vel.x, &bullet->vel.y);
    al_transform_coordinates (&bullet->angular_velocity,
                              &bullet->acc.x, &bullet->acc.y);
    bullet->position = gshmup_vector2_add (bullet->position, bullet->vel);
    bullet->vel = gshmup_vector2_add (bullet->vel, bullet->acc);
    bullet->life_count++;
    bullet->kill = bullet->kill || bullet_out_of_bounds (bullet);

    if (bullet->directional) {
        bullet->sprite.rotation = gshmup_vector2_angle (bullet->vel);
    }
}

GshmupBulletSystem *
gshmup_create_bullet_system (int max_bullets, GshmupSpriteSheet *sprite_sheet)
{
    GshmupBulletSystem *system;

    system = (GshmupBulletSystem *) scm_gc_malloc (sizeof (GshmupBulletSystem),
                                                   "bullet system");
    system->sprite_sheet = sprite_sheet;
    system->bullets = gshmup_create_entity_pool (max_bullets);
    system->bounds = gshmup_create_rect (0, 0, 0, 0);

    return system;
}

void
gshmup_destroy_bullet_system (GshmupBulletSystem *system)
{
    gshmup_destroy_entity_pool (system->bullets);
    scm_gc_free (system, sizeof (GshmupBulletSystem), "bullet system");
}

void
gshmup_draw_bullet_system (GshmupBulletSystem *system)
{
    al_hold_bitmap_drawing (true);
    gshmup_draw_entity_pool (system->bullets);
    al_hold_bitmap_drawing (false);
}

void gshmup_draw_bullet_system_hitboxes (GshmupBulletSystem *system,
                                         ALLEGRO_COLOR fill_color,
                                         ALLEGRO_COLOR border_color)
{
    GshmupEntity *entity = system->bullets->active_entities;

    while (entity) {
        GshmupBullet *bullet = GSHMUP_BULLET (entity);
        GshmupRect hitbox = gshmup_rect_move (bullet->hitbox, bullet->position);

        gshmup_draw_rect (hitbox, fill_color, border_color);
        entity = bullet->next;
    }
}

void
gshmup_update_bullet_system (GshmupBulletSystem *system)
{
    gshmup_update_entity_pool (system->bullets);
}

void
gshmup_set_bullet_type (GshmupEntity *entity, GshmupBulletType *type)
{
    GshmupBulletSystem *parent = entity->bullet.parent;
    ALLEGRO_BITMAP *image = gshmup_get_sprite_sheet_tile (parent->sprite_sheet,
                                                         type->tile);

    gshmup_init_sprite (&entity->bullet.sprite, image);
    entity->bullet.directional = type->directional;
    entity->bullet.hitbox = type->hitbox;
    entity->bullet.blend_mode = type->blend_mode;
    entity->bullet.on_hit = type->on_hit;
}

void
gshmup_emit_bullet (GshmupBulletSystem *system, GshmupVector2 position,
                    float speed, float direction, float acceleration,
                    float angular_velocity, float life, GshmupBulletType *type,
                    SCM thunk)
{
    GshmupEntity *entity = gshmup_entity_pool_new (system->bullets);

    entity->type = GSHMUP_ENTITY_BULLET;
    entity->bullet.parent = system;
    entity->bullet.position = position;
    entity->bullet.life = life;
    entity->bullet.life_count = 0;
    entity->bullet.kill = false;
    gshmup_set_bullet_type (entity, type);
    init_bullet_movement (GSHMUP_BULLET (entity), speed, direction,
                          acceleration, angular_velocity);

    if (scm_is_true (thunk) && scm_is_false (scm_eq_p (thunk, SCM_UNDEFINED))) {
        gshmup_schedule (entity->bullet.agenda, 0, thunk);
    }
}

int
gshmup_get_bullet_system_size (GshmupBulletSystem *system)
{
    return system->bullets->size;
}

int
gshmup_get_bullet_system_free_size (GshmupBulletSystem *system)
{
    return system->bullets->pool_size;
}

int
gshmup_get_bullet_system_max_free_size (GshmupBulletSystem *system)
{
    return system->bullets->max_pool_size;
}

void
gshmup_set_current_bullet_system (GshmupBulletSystem *system)
{
    current_bullets = system;
}

static bool
bullet_collision_check (GshmupBullet *bullet, GshmupRect rect)
{
    GshmupRect hitbox = gshmup_rect_move (bullet->hitbox, bullet->position);

    if (gshmup_rect_collide_rect (hitbox, rect)) {
        return true;
    }

    return false;
}

void
gshmup_bullet_system_collide_rect (GshmupBulletSystem *system, GshmupRect rect)
{
    GshmupEntity *entity = system->bullets->active_entities;

    while (entity) {
        GshmupBullet *bullet = GSHMUP_BULLET (entity);

        if (bullet_collision_check (bullet, rect)) {
            bullet->kill = true;

            if (scm_is_true (scm_procedure_p (bullet->on_hit))) {
                scm_call_0 (bullet->on_hit);
            }
        }

        entity = bullet->next;
    }
}

SCM_DEFINE (emit_bullet, "%emit-bullet", 4, 1, 0,
            (SCM pos, SCM speed, SCM direction, SCM type, SCM thunk),
            "Emit a bullet.")
{
    gshmup_emit_bullet (current_bullets,
                        gshmup_scm_to_vector2 (pos), scm_to_double (speed),
                        scm_to_double (direction), 0, 0, 0,
                        check_bullet_type (type), thunk);

    return SCM_UNSPECIFIED;
}

SCM_DEFINE (bullet_position, "bullet-position", 0, 0, 0,
            (void),
            "Return bullet position.")
{
    return gshmup_scm_from_vector2 (current_bullet->position);
}

SCM_DEFINE (bullet_speed, "bullet-speed", 0, 0, 0,
            (void),
            "Return bullet speed.")
{
    return scm_from_double (gshmup_vector2_mag (current_bullet->vel));
}

SCM_DEFINE (bullet_direction, "bullet-direction", 0, 0, 0,
            (void),
            "Return bullet direction in degrees.")
{
    float direction = gshmup_rad_to_deg (gshmup_vector2_angle (current_bullet->vel));

    return scm_from_double (direction);
}


SCM_DEFINE (set_bullet_position, "set-bullet-position", 1, 0, 0,
            (SCM position),
            "Change the current bullet's position.")
{
    current_bullet->position = gshmup_scm_to_vector2 (position);

    return SCM_UNSPECIFIED;
}

SCM_DEFINE (set_bullet_speed, "set-bullet-speed", 1, 0, 0,
            (SCM speed),
            "Change the current bullet's speed.")
{
    GshmupVector2 normal = gshmup_vector2_norm (current_bullet->vel);

    current_bullet->vel = gshmup_vector2_scale (normal, scm_to_double (speed));

    return SCM_UNSPECIFIED;
}

SCM_DEFINE (set_bullet_direction, "set-bullet-direction", 1, 0, 0,
            (SCM direction),
            "Change the current bullet's direction.")
{
    float theta = gshmup_deg_to_rad (scm_to_double (direction));
    float speed = gshmup_vector2_mag (current_bullet->vel);
    float acceleration = gshmup_vector2_mag (current_bullet->acc);

    current_bullet->vel = gshmup_vector2_from_polar (speed, theta);
    current_bullet->acc = gshmup_vector2_from_polar (acceleration, theta);

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
                  s_set_bullet_position,
                  s_set_bullet_speed,
                  s_set_bullet_direction,
                  NULL);
}
