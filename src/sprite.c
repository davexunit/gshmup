#include "sprite.h"

static void
init_sprite (GshmupSprite *sprite)
{
    sprite->position = gshmup_create_vector2 (0, 0);
    sprite->scale = gshmup_create_vector2 (1, 1);
    sprite->rotation = 0;
    sprite->color = al_map_rgba_f (1, 1, 1, 1);
}

GshmupSprite
gshmup_create_sprite (ALLEGRO_BITMAP *image)
{
    GshmupSprite sprite;

    sprite.type = GSHMUP_SPRITE_STATIC;
    sprite.image = image;
    sprite.anchor = gshmup_create_vector2 (al_get_bitmap_width (image) / 2,
                                           al_get_bitmap_height (image) / 2);

    init_sprite (&sprite);

    return sprite;
}

GshmupSprite
gshmup_create_sprite_animated (GshmupAnimation *anim)
{
    GshmupSprite sprite;

    sprite.type = GSHMUP_SPRITE_ANIMATED;
    sprite.anim_state.anim = anim;
    sprite.anim_state.current_frame = 0;
    sprite.anim_state.timer = 0;
    sprite.anim_state.playing = FALSE;
    sprite.anchor = gshmup_create_vector2 (anim->sprite_sheet->tile_width / 2,
                                           anim->sprite_sheet->tile_height / 2);

    init_sprite (&sprite);

    return sprite;
}

static void
next_frame (GshmupSprite *sprite)
{
    GshmupAnimation *anim = sprite->anim_state.anim;

    sprite->anim_state.current_frame++;

    if (sprite->anim_state.current_frame >= anim->num_frames) {
        sprite->anim_state.current_frame = 0;

        if (anim->anim_mode == GSHMUP_ANIM_ONCE) {
            sprite->anim_state.playing = false;
        }
    }
}

static void
update_animation (GshmupSprite *sprite)
{
    GshmupAnimation *anim = sprite->anim_state.anim;

    if (sprite->anim_state.playing) {
        sprite->anim_state.timer++;

        if (sprite->anim_state.timer >= anim->duration) {
            next_frame (sprite);
            sprite->anim_state.timer = 0;
        }
    }
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
        update_animation (sprite);
        image = gshmup_get_animation_image (sprite->anim_state.anim,
                                            sprite->anim_state.current_frame);
        break;
    }

    al_draw_tinted_scaled_rotated_bitmap (image,
                                          gshmup_color_mult_alpha (sprite->color),
                                          sprite->anchor.x, sprite->anchor.y,
                                          sprite->position.x, sprite->position.y,
                                          sprite->scale.x, sprite->scale.y,
                                          sprite->rotation, 0);
}

void
gshmup_sprite_play_animation (GshmupSprite *sprite)
{
    sprite->anim_state.playing = true;
    sprite->anim_state.current_frame = 0;
}

void
gshmup_sprite_stop_animation (GshmupSprite *sprite)
{
    sprite->anim_state.playing = false;
}
