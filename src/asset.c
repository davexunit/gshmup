#include "asset.h"

GHashTable *images = NULL;
GHashTable *sprite_sheets = NULL;
gchar *asset_path = NULL;

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
    g_hash_table_destroy (sprite_sheets);
}

void
gshmup_free_assets (void)
{
    g_hash_table_remove_all (images);
    g_hash_table_remove_all (sprite_sheets);
}

ALLEGRO_BITMAP *
gshmup_load_image_asset (const gchar *filename)
{
    ALLEGRO_BITMAP *image;

    image = (ALLEGRO_BITMAP *) g_hash_table_lookup (images, filename);

    if (!image) {
        gchar *full_filename = g_build_filename (asset_path, "images",
                                                 filename, NULL);

        image = al_load_bitmap (full_filename);
        g_hash_table_insert (images, g_strdup (filename), image);
        g_free (full_filename);
    }

    return image;
}

GshmupSpriteSheet *
gshmup_load_sprite_sheet_asset (const gchar *filename)
{
    GshmupSpriteSheet *sprite_sheet =
        (GshmupSpriteSheet *) g_hash_table_lookup (sprite_sheets, filename);

    if (!sprite_sheet) {
        gchar *full_filename = g_build_filename (asset_path, "sprites",
                                                 filename, NULL);

        sprite_sheet = gshmup_load_sprite_sheet (full_filename);
        g_hash_table_insert (sprite_sheets, g_strdup (filename), sprite_sheet);
    }

    return sprite_sheet;
}

SCM_DEFINE (set_asset_path, "set-asset-path", 1, 0, 0,
            (SCM path_list),
            "Build a path to the assets directory from a list of directories.")
{
    SCM item = path_list;
    gsize length = scm_to_int (scm_length (path_list)) + 1;
    gchar **dirs = g_new (gchar *, length);
    gint i = 0;

    /* Null-terminated array. */
    dirs[length - 1] = NULL;

    /*
     * The asset path is given in the form of a list of directories.
     * We build a filename from that in a portable way with glib.
     */
    while (!scm_is_null (item)) {
        dirs[i] = scm_to_latin1_string (scm_car (item));
        i++;
        item = scm_cdr (item);
    }

    asset_path = g_build_filenamev (dirs);
    g_strfreev (dirs);

    return scm_from_latin1_string (asset_path);
}

void
gshmup_asset_init_scm (void)
{
    #include "asset.x"

    scm_c_export (s_set_asset_path,
                  NULL);
}
