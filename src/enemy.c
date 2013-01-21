#include "enemy.h"

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
