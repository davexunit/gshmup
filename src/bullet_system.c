#include "bullet_system.h"

static GshmupBullet *
new_bullet (GshmupBulletSystem *system)
{
    GshmupBullet *bullet;

    if (system->free_bullets) {
        /* Remove bullet from front of free bullets list. */
        bullet = system->free_bullets;
        system->free_bullets = bullet->next;
        system->pool_size--;
    } else {
        bullet = (GshmupBullet *) malloc (sizeof (GshmupBullet));
    }

    /* Insert bullet at front of active bullets list. */
    bullet->next = system->active_bullets;
    system->active_bullets = bullet;
    system->bullet_count++;

    return bullet;
}

static void
free_bullet (GshmupBulletSystem *system, GshmupBullet *bullet)
{
    if (system->pool_size >= system->max_pool_size) {
        /* Free the memory if there's no room in the pool. */
        free (bullet);
    } else {
        system->pool_size++;
        bullet->next = system->free_bullets;
        system->free_bullets = bullet;
    }

    system->bullet_count--;
}

static void
free_bullets (GshmupBullet *bullets)
{
    GshmupBullet *bullet = bullets;

    while (bullet) {
        GshmupBullet *next = bullet->next;

        free (bullet);
        bullet = next;
    }
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

static void
draw_bullet (GshmupBullet *bullet)
{
    bullet->sprite.position = bullet->pos;
    gshmup_draw_sprite (&bullet->sprite);
}

static void
update_bullet (GshmupBullet *bullet)
{
    al_transform_coordinates (&bullet->angular_velocity,
                              &bullet->vel.x, &bullet->vel.y);
    al_transform_coordinates (&bullet->angular_velocity,
                              &bullet->acc.x, &bullet->acc.y);
    bullet->pos = gshmup_vector2_add (bullet->pos, bullet->vel);
    bullet->vel = gshmup_vector2_add (bullet->vel, bullet->acc);
    bullet->life_count++;
}

static bool
bullet_out_of_bounds (GshmupBulletSystem *system, GshmupBullet *bullet)
{
    return !gshmup_rect_collide_point (system->bounds, bullet->pos);
}

GshmupBulletSystem *
gshmup_create_bullet_system (int max_pool_size)
{
    GshmupBulletSystem *system;

    system = (GshmupBulletSystem *) malloc (sizeof (GshmupBulletSystem));
    system->max_pool_size = max_pool_size;
    system->pool_size = 0;
    system->bullet_count = 0;
    system->active_bullets = NULL;
    system->free_bullets = NULL;

    return system;
}

void
gshmup_destroy_bullet_system (GshmupBulletSystem *system)
{
    free_bullets (system->active_bullets);
    free_bullets (system->free_bullets);
    free (system);
}

void
gshmup_draw_bullet_system (GshmupBulletSystem *system)
{
    GshmupBullet *bullet = system->active_bullets;

    al_hold_bitmap_drawing (true);

    while (bullet) {
        draw_bullet (bullet);
        bullet = bullet->next;
    }

    al_hold_bitmap_drawing (false);
}

void
gshmup_update_bullet_system (GshmupBulletSystem *system)
{
    GshmupBullet *bullet = system->active_bullets;
    GshmupBullet *prev = NULL;

    while (bullet) {
        GshmupBullet *next = bullet->next;

        update_bullet (bullet);

        if (bullet->kill || bullet_out_of_bounds (system, bullet)) {
            /* Remove dead bullet from active list. */
            if (prev) {
                prev->next = next;
            } else {
                system->active_bullets = next;
            }

            /* Place dead bullet in free list. */
            free_bullet (system, bullet);

            /* Set bullet to prev so that iterator is correct. */
        } else {
            /*
             * Prev only changes when we aren't pruning a bullet from
             * the active list.
             */
            prev = bullet;
        }

        bullet = next;
    }
}

void
gshmup_emit_bullet (GshmupBulletSystem *system, ALLEGRO_BITMAP *image,
                    GshmupVector2 position, float speed, float direction,
                    float acceleration, float angular_velocity, float life)
{
    GshmupBullet *bullet = new_bullet (system);

    bullet->pos = position;
    bullet->life = life;
    bullet->life_count = 0;
    bullet->kill = false;
    gshmup_init_sprite (&bullet->sprite, image);
    init_bullet_movement (bullet, speed, direction, acceleration, angular_velocity);
}
