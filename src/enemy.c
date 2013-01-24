#include "enemy.h"

static GshmupEnemy *current_enemy = NULL;

void
gshmup_init_enemy (GshmupEntity *entity, GshmupAnimation *anim, int max_health)
{
    gshmup_init_entity (entity);
    entity->type = GSHMUP_ENTITY_ENEMY;
    entity->enemy.max_health = max_health;
    entity->enemy.health = max_health;
    gshmup_init_animated_sprite (&entity->enemy.sprite, anim);
    gshmup_play_animation (anim);
}

void
gshmup_draw_enemy (GshmupEnemy *enemy)
{
    enemy->sprite.position = enemy->position;
    gshmup_draw_sprite (&enemy->sprite);
}

void
gshmup_update_enemy (GshmupEnemy *enemy)
{
    gshmup_update_animation (enemy->sprite.anim);
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

void
gshmup_enemy_init_scm (void)
{
#include "enemy.x"

    scm_c_export (s_damage_enemy,
                  NULL);
}
