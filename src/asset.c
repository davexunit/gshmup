#include "asset.h"

GHashTable *images;

static void
destroy_image (gpointer data)
{
    ALLEGRO_BITMAP *image = (ALLEGRO_BITMAP *) data;

    al_destroy_bitmap (image);
}

void
gshmup_init_assets (void)
{
    images = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, destroy_image);
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
gshmup_load_image (const char *filename)
{
    ALLEGRO_BITMAP *image = (ALLEGRO_BITMAP *) g_hash_table_lookup (images, filename);

    if (!image) {
        image = al_load_bitmap (filename);
        g_hash_table_insert (images, g_strdup (filename), image);
    }

    return image;
}
