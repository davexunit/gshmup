#include "enemy.h"

static GshmupEnemy *current_enemy = NULL;

GshmupEnemy *
gshmup_create_enemy (GshmupAnimation *anim, int max_health)
{
    GshmupEnemy *enemy = (GshmupEnemy *) scm_gc_malloc (sizeof (GshmupEnemy),
                                                        "enemy");

    enemy->entity.position = gshmup_create_vector2 (0, 0);
    enemy->entity.agenda = gshmup_create_agenda ();
    enemy->entity.hitbox = gshmup_create_rect (0, 0, 0, 0);
    enemy->max_health = max_health;
    enemy->health = max_health;
    enemy->kill = false;
    enemy->next = NULL;
    gshmup_init_animated_sprite (&enemy->entity.sprite, anim);
    gshmup_play_animation (anim);

    return enemy;
}

void
gshmup_destroy_enemy (GshmupEnemy *enemy)
{
    scm_gc_free (enemy, sizeof (GshmupEnemy), "enemy");
}

void
gshmup_destroy_enemies (GshmupEnemy *enemy)
{
    while(enemy) {
        GshmupEnemy *next = enemy->next;

        gshmup_destroy_enemy (enemy);
        enemy = next;
    }
}

void
gshmup_draw_enemy (GshmupEnemy *enemy)
{
    enemy->entity.sprite.position = enemy->entity.position;
    gshmup_draw_sprite (&enemy->entity.sprite);
}

void
gshmup_draw_enemies (GshmupEnemy *enemy)
{
    while (enemy) {
        gshmup_draw_enemy (enemy);
        enemy = enemy->next;
    }
}

void
gshmup_update_enemy (GshmupEnemy *enemy)
{
    current_enemy = enemy;
    gshmup_update_animation (enemy->entity.sprite.anim);
    gshmup_update_agenda (enemy->entity.agenda);
}

GshmupEnemy *
gshmup_update_enemies (GshmupEnemy *enemy)
{
    GshmupEnemy *enemies = enemy;
    GshmupEnemy *prev = NULL;

    while (enemy) {
        GshmupEnemy *next = enemy->next;

        gshmup_update_enemy (enemy);

        if (enemy->kill) {
            gshmup_destroy_enemy (enemy);

            if (prev) {
                prev->next = next;
            } else {
                enemies = NULL;
            }
        }

        prev = enemy;
        enemy = next;
    }

    return enemies;
}

void
gshmup_set_current_enemy (GshmupEnemy *enemy)
{
    current_enemy = enemy;
}

SCM_DEFINE (damage_enemy, "damage-enemy", 1, 0, 0,
            (SCM damage),
            "Reduce enemy health points by @var{damage}")
{
    current_enemy->health -= scm_to_int (damage);

    if (current_enemy->health <= 0) {
        current_enemy->kill = true;
    }

    return SCM_UNSPECIFIED;
}

SCM_DEFINE (enemy_position, "enemy-position", 0, 0, 0,
            (void),
            "Return the position of the current enemy.")
{
    return gshmup_scm_from_vector2 (current_enemy->entity.position);
}

SCM_DEFINE (move_enemy, "move-enemy", 1, 0, 0,
            (SCM v),
            "Add @var{v} to the currrent enemy's position.")
{
    current_enemy->entity.position =
        gshmup_vector2_add (current_enemy->entity.position,
                            gshmup_scm_to_vector2 (v));

    return SCM_UNSPECIFIED;
}

void
gshmup_enemy_init_scm (void)
{
#include "enemy.x"

    scm_c_export (s_damage_enemy,
                  s_enemy_position,
                  s_move_enemy,
                  NULL);
}
