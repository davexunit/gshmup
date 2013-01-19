#include "enemy.h"

void
gshmup_init_enemy (GshmupEntity *entity, ALLEGRO_BITMAP *image)
{
    gshmup_init_entity (entity);
    entity->type = GSHMUP_ENTITY_ENEMY;
    gshmup_init_sprite (&entity->enemy.sprite, image);

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

}
