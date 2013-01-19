#include "bullet_system.h"

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
    al_transform_coordinates (&bullet->angular_velocity,
                              &bullet->vel.x, &bullet->vel.y);
    al_transform_coordinates (&bullet->angular_velocity,
                              &bullet->acc.x, &bullet->acc.y);
    bullet->position = gshmup_vector2_add (bullet->position, bullet->vel);
    bullet->vel = gshmup_vector2_add (bullet->vel, bullet->acc);
    bullet->life_count++;
    bullet->kill = bullet->kill || bullet_out_of_bounds (bullet);
}

GshmupBulletSystem *
gshmup_create_bullet_system (int max_bullets)
{
    GshmupBulletSystem *system;

    system = (GshmupBulletSystem *) malloc (sizeof (GshmupBulletSystem));
    system->bullets = gshmup_create_entity_pool (max_bullets);
    system->bounds = gshmup_create_rect (0, 0, 0, 0);

    return system;
}

void
gshmup_destroy_bullet_system (GshmupBulletSystem *system)
{
    gshmup_destroy_entity_pool (system->bullets);
    free (system);
}

void
gshmup_draw_bullet_system (GshmupBulletSystem *system)
{
    al_hold_bitmap_drawing (true);
    gshmup_draw_entity_pool (system->bullets);
    al_hold_bitmap_drawing (false);
}

void
gshmup_update_bullet_system (GshmupBulletSystem *system)
{
    gshmup_update_entity_pool (system->bullets);
}

void
gshmup_emit_bullet (GshmupBulletSystem *system, ALLEGRO_BITMAP *image,
                    GshmupVector2 position, float speed, float direction,
                    float acceleration, float angular_velocity, float life)
{
    GshmupEntity *entity = gshmup_entity_pool_new (system->bullets);

    entity->type = GSHMUP_ENTITY_BULLET;
    entity->bullet.parent = system;
    entity->bullet.position = position;
    entity->bullet.life = life;
    entity->bullet.life_count = 0;
    entity->bullet.kill = false;
    gshmup_init_sprite (&entity->bullet.sprite, image);
    init_bullet_movement (GSHMUP_BULLET (entity), speed, direction,
                          acceleration, angular_velocity);
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
