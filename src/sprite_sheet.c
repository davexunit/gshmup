#include "sprite_sheet.h"

static void
init_sprite_sheet_tiles (GshmupSpriteSheet *sprite_sheet)
{
    int image_width = al_get_bitmap_width (sprite_sheet->image);
    int image_height = al_get_bitmap_height (sprite_sheet->image);
    int tile_width = sprite_sheet->tile_width;
    int tile_height = sprite_sheet->tile_height;
    int spacing = sprite_sheet->spacing;
    int margin = sprite_sheet->margin;
    int columns = (image_width - margin) / (tile_width + spacing);
    int rows = (image_height - margin) / (tile_height + spacing);

    sprite_sheet->num_tiles = rows * columns;
    sprite_sheet->tiles = (ALLEGRO_BITMAP ** ) malloc (sizeof (ALLEGRO_BITMAP **)
                                                       * sprite_sheet->num_tiles);

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < columns; ++x) {
            int index = y * columns + x;
            int tile_x = margin + x * (tile_width + spacing);
            int tile_y = margin + y * (tile_height + spacing);

            ALLEGRO_BITMAP *tile = al_create_sub_bitmap (sprite_sheet->image,
                                                         tile_x, tile_y,
                                                         tile_width, tile_height);
            sprite_sheet->tiles[index] = tile;
        }
    }
}

GshmupSpriteSheet *
gshmup_create_sprite_sheet (ALLEGRO_BITMAP *image, int tile_width, int tile_height,
                            int spacing, int margin)
{
    GshmupSpriteSheet *sprite_sheet = (GshmupSpriteSheet *) malloc (sizeof (GshmupSpriteSheet));

    sprite_sheet->image = image;
    sprite_sheet->tile_width = tile_width;
    sprite_sheet->tile_height = tile_height;
    sprite_sheet->spacing = spacing;
    sprite_sheet->margin = margin;
    init_sprite_sheet_tiles (sprite_sheet);

    return sprite_sheet;
}

void
gshmup_destroy_sprite_sheet (GshmupSpriteSheet *sprite_sheet)
{
    free (sprite_sheet->tiles);
    free (sprite_sheet);
}

ALLEGRO_BITMAP *
gshmup_get_sprite_sheet_tile (GshmupSpriteSheet *sprite_sheet, int index)
{
    return sprite_sheet->tiles[index];
}
