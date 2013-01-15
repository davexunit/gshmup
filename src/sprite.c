#include "sprite.h"

void
gshmup_init_sprite (GshmupSprite *sprite, ALLEGRO_BITMAP *image)
{
    sprite->image = image;
    sprite->position = gshmup_create_vector2 (0, 0);
    sprite->anchor = gshmup_create_vector2 (al_get_bitmap_width (image) / 2,
                                            al_get_bitmap_height (image) / 2);
    sprite->scale = gshmup_create_vector2 (1, 1);
    sprite->rotation = 0;
    sprite->color = al_map_rgba_f (1, 1, 1, 1);
}

void
gshmup_draw_sprite (GshmupSprite *sprite)
{
    al_draw_tinted_scaled_rotated_bitmap (sprite->image,
                                          gshmup_color_mult_alpha (sprite->color),
                                          sprite->anchor.x, sprite->anchor.y,
                                          sprite->position.x, sprite->position.y,
                                          sprite->scale.x, sprite->scale.y,
                                          sprite->rotation, 0);
}
