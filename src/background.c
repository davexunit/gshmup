#include "background.h"
#include "game.h"

void
gshmup_init_background (GshmupBackground *bg, ALLEGRO_BITMAP *image,
                        float scroll_speed)
{
    bg->image = image;
    bg->scroll_y = 0;
    bg->scroll_speed = scroll_speed;
}

void
gshmup_draw_background (GshmupBackground *bg)
{
    al_draw_bitmap (bg->image, 0,
                    bg->scroll_y - al_get_bitmap_height (bg->image), 0);
    al_draw_bitmap (bg->image, 0, bg->scroll_y, 0);
}

void
gshmup_update_background (GshmupBackground *bg)
{
    bg->scroll_y += bg->scroll_speed;

    if (bg->scroll_y >= GAME_HEIGHT) {
        bg->scroll_y -= al_get_bitmap_height (bg->image);
    }
}
