#include "enemy.h"

static GshmupEnemy *current_enemy = NULL;

GshmupEnemy *
gshmup_create_enemy (GshmupAnimation *anim, int max_health,
                     SCM script, SCM on_death)
{
    GshmupEnemy *enemy = (GshmupEnemy *) scm_gc_malloc (sizeof (GshmupEnemy),
                                                        "enemy");

    enemy->entity = gshmup_create_entity ("Enemy");
    enemy->max_health = max_health;
    enemy->health = max_health;
    enemy->kill = false;
    enemy->next = NULL;
    enemy->on_death = on_death;
    gshmup_init_animated_sprite (&enemy->entity.sprite, anim);
    gshmup_play_animation (anim);

    /* Schedule script if one is provided. */
    if (scm_is_true (scm_procedure_p (script))) {
        gshmup_schedule (enemy->entity.agenda, 0, script);
    }

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
    gshmup_draw_entity (&enemy->entity);
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
    gshmup_update_entity (&enemy->entity);
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

        if (scm_is_true (scm_procedure_p (current_enemy->on_death))) {
            scm_call_0 (current_enemy->on_death);
        }
    }

    return SCM_UNSPECIFIED;
}

SCM_DEFINE (kill_enemy, "kill-enemy", 0, 0, 0,
            (void),
            "Sets enemy's kill flag.")
{
    current_enemy->kill = true;

    return SCM_UNSPECIFIED;
}

void
gshmup_enemy_init_scm (void)
{
#include "enemy.x"

    scm_c_export (s_damage_enemy,
                  s_kill_enemy,
                  NULL);
}
