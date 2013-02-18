#include "asset.h"

GHashTable *images;
GHashTable *sprite_sheets;

static void
destroy_image (gpointer data)
{
    ALLEGRO_BITMAP *image = (ALLEGRO_BITMAP *) data;

    al_destroy_bitmap (image);
}

static void
destroy_sprite_sheet (gpointer data)
{
    GshmupSpriteSheet *sprite_sheet = (GshmupSpriteSheet *) data;

    gshmup_destroy_sprite_sheet (sprite_sheet);
}

void
gshmup_init_assets (void)
{
    images = g_hash_table_new_full (g_str_hash, g_str_equal,
                                    g_free, destroy_image);
    sprite_sheets = g_hash_table_new_full (g_str_hash, g_str_equal,
                                           g_free, destroy_sprite_sheet);
}

void
gshmup_destroy_assets (void)
{
    g_hash_table_destroy (images);
}

void
gshmup_free_assets (void)
{
    g_hash_table_remove_all (images);
}

ALLEGRO_BITMAP *
gshmup_load_image_asset (const gchar *filename)
{
    ALLEGRO_BITMAP *image = (ALLEGRO_BITMAP *) g_hash_table_lookup (images, filename);

    if (!image) {
        image = al_load_bitmap (filename);
        g_hash_table_insert (images, g_strdup (filename), image);
    }

    return image;
}

GshmupSpriteSheet *
gshmup_load_sprite_sheet_asset (const gchar *filename)
{
    GshmupSpriteSheet *sprite_sheet =
        (GshmupSpriteSheet *) g_hash_table_lookup (sprite_sheets, filename);

    if (!sprite_sheet) {
        sprite_sheet = gshmup_load_sprite_sheet (filename);
        g_hash_table_insert (sprite_sheets, g_strdup (filename), sprite_sheet);
    }

    return sprite_sheet;
}
