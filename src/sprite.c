#include "sprite.h"

static void
init_sprite (GshmupSprite *sprite)
{
    sprite->position = gshmup_create_vector2 (0, 0);
    sprite->scale = gshmup_create_vector2 (1, 1);
    sprite->rotation = 0;
    sprite->color = al_map_rgba_f (1, 1, 1, 1);
}

void
gshmup_init_sprite (GshmupSprite *sprite, ALLEGRO_BITMAP *image)
{
    sprite->type = GSHMUP_SPRITE_STATIC;
    sprite->image = image;
    sprite->anchor = gshmup_create_vector2 (al_get_bitmap_width (image) / 2,
                                            al_get_bitmap_height (image) / 2);

    init_sprite (sprite);
}

void
gshmup_init_animated_sprite (GshmupSprite *sprite, GshmupAnimation *anim)
{
    sprite->type = GSHMUP_SPRITE_ANIMATED;
    sprite->anim = anim;
    sprite->anchor = gshmup_create_vector2 (anim->sprite_sheet->tile_width / 2,
                                            anim->sprite_sheet->tile_height / 2);

    init_sprite (sprite);
}

void
gshmup_draw_sprite (GshmupSprite *sprite)
{
    ALLEGRO_BITMAP *image = NULL;

    switch(sprite->type) {
    case GSHMUP_SPRITE_STATIC:
        image = sprite->image;
        break;
    case GSHMUP_SPRITE_ANIMATED:
        image = gshmup_get_animation_image (sprite->anim);
        break;
    }

    al_draw_tinted_scaled_rotated_bitmap (image,
                                          gshmup_color_mult_alpha (sprite->color),
                                          sprite->anchor.x, sprite->anchor.y,
                                          sprite->position.x, sprite->position.y,
                                          sprite->scale.x, sprite->scale.y,
                                          sprite->rotation, 0);
}
