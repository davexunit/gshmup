#include "sprite.h"

GshmupAnimation *
gshmup_create_animation (GshmupSpriteSheet *sprite_sheet, int duration,
                         int num_frames, int *frames, int anim_mode)
{
    GshmupAnimation *anim = (GshmupAnimation *) malloc (sizeof (GshmupAnimation));

    anim->sprite_sheet = sprite_sheet;
    anim->duration = duration;
    anim->timer = 0;
    anim->num_frames = num_frames;
    anim->current_frame = 0;
    anim->anim_mode = anim_mode;
    anim->playing = false;
    anim->frames = (int *) malloc (sizeof (int) * num_frames);

    /* Copy frames. */
    for (int i = 0; i < num_frames; ++i) {
        anim->frames[i] = frames[i];
    }

    return anim;
}

void
gshmup_destroy_animation (GshmupAnimation *anim)
{
    free (anim->frames);
    free (anim);
}

ALLEGRO_BITMAP *
gshmup_get_animation_image (GshmupAnimation *anim)
{
    return gshmup_get_sprite_sheet_tile (anim->sprite_sheet,
                                         anim->frames[anim->current_frame]);
}

static void
next_frame (GshmupAnimation *anim)
{
    anim->current_frame++;

    if (anim->current_frame >= anim->num_frames) {
        anim->current_frame = 0;

        if (anim->anim_mode == GSHMUP_ANIM_ONCE) {
            anim->playing = false;
        }
    }
}

void
gshmup_update_animation (GshmupAnimation *anim)
{
    if (anim->playing) {
        anim->timer++;

        if (anim->timer >= anim->duration) {
            next_frame (anim);
            anim->timer = 0;
        }
    }
}

void
gshmup_play_animation (GshmupAnimation *anim)
{
    anim->playing = true;
    anim->current_frame = 0;
}
void
gshmup_stop_animation (GshmupAnimation *anim)
{
    anim->playing = false;
}

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
