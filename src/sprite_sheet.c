#include "sprite_sheet.h"

GshmupAnimation *
gshmup_create_animation (GshmupSpriteSheet *sprite_sheet, int duration,
                         int num_frames, int *frames, int anim_mode)
{
    GshmupAnimation *anim = (GshmupAnimation *) malloc (sizeof (GshmupAnimation));

    anim->sprite_sheet = sprite_sheet;
    anim->duration = duration;
    anim->num_frames = num_frames;
    anim->anim_mode = anim_mode;
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
gshmup_get_animation_image (GshmupAnimation *anim, gint frame)
{
    return gshmup_get_sprite_sheet_tile (anim->sprite_sheet,
                                         anim->frames[frame]);
}

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

static void
hash_destroy_animation (gpointer data)
{
    GshmupAnimation *anim = (GshmupAnimation *) data;

    gshmup_destroy_animation (anim);
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
    sprite_sheet->animations = g_hash_table_new_full (g_str_hash, g_str_equal,
                                                      g_free, hash_destroy_animation);
    init_sprite_sheet_tiles (sprite_sheet);

    return sprite_sheet;
}

void
load_animation (GshmupSpriteSheet *sprite_sheet, GKeyFile *key_file,
                const gchar *group)
{
    /* Parse out key from group name. "Animation_idle" becomes "idle". */
    GRegex *pattern = g_regex_new ("Animation_", 0, 0, NULL);
    gchar *name = g_regex_replace_literal (pattern, group, -1, 0, "", 0, NULL);
    gsize anim_length = 0;
    gint *frames = g_key_file_get_integer_list (key_file, group, "frames",
                                                &anim_length, NULL);
    gint duration = g_key_file_get_integer (key_file, group, "duration", NULL);
    gchar *type_str = g_key_file_get_string (key_file, group, "type", NULL);
    gint type = GSHMUP_ANIM_LOOP;

    /* Loop or play animation once? */
    if (g_strcmp0 (type_str, "loop") == 0) {
        type = GSHMUP_ANIM_LOOP;
    } else if (g_strcmp0 (type_str, "once") == 0) {
        type = GSHMUP_ANIM_ONCE;
    }

    GshmupAnimation *anim = gshmup_create_animation (sprite_sheet, duration,
                                                     anim_length, frames,
                                                     type);
    gshmup_sprite_sheet_add_animation (sprite_sheet, name, anim);
    g_free (pattern);
    g_free (name);
    g_free (frames);
    g_free (type_str);
}

void
load_animations (GshmupSpriteSheet *sprite_sheet, GKeyFile *key_file)
{
    GRegex *pattern = g_regex_new ("Animation_.+", 0, 0, NULL);
    gsize length = 0;
    gchar **groups = g_key_file_get_groups (key_file, &length);

    for (int i = 0; i < length; ++i) {
        gchar *group = groups[i];

        if (g_regex_match (pattern, group, 0, NULL)) {
            load_animation (sprite_sheet, key_file, group);
        }
    }

    g_free (pattern);
    g_strfreev (groups);
}

GshmupSpriteSheet *
gshmup_load_sprite_sheet (const gchar *filename)
{
    GshmupSpriteSheet *sprite_sheet = NULL;
    ALLEGRO_BITMAP *image = NULL;
    GKeyFile *key_file = g_key_file_new ();
    static const gchar *group = "Sprite Sheet";
    gchar *dirname = g_path_get_dirname (filename);
    gchar *image_rel_filename;
    gchar *image_filename;
    gint tile_width;
    gint tile_height;
    gint spacing;
    gint margin;

    g_key_file_load_from_file (key_file, filename, G_KEY_FILE_NONE, NULL);
    tile_width = g_key_file_get_integer (key_file, group, "tile_width", NULL);
    tile_height = g_key_file_get_integer (key_file, group, "tile_height", NULL);
    spacing = g_key_file_get_integer (key_file, group, "spacing", NULL);
    margin = g_key_file_get_integer (key_file, group, "margin", NULL);
    image_rel_filename = g_key_file_get_string (key_file, group, "image", NULL);
    image_filename = g_build_filename (dirname, image_rel_filename, NULL);
    image = al_load_bitmap (image_filename);

    if (image) {
        sprite_sheet = gshmup_create_sprite_sheet (image, tile_width, tile_height,
                                                   spacing, margin);
    }

    load_animations (sprite_sheet, key_file);
    g_free (image_rel_filename);
    g_free (image_filename);
    g_free (dirname);
    g_key_file_free (key_file);

    return sprite_sheet;
}

void
gshmup_sprite_sheet_add_animation (GshmupSpriteSheet *sprite_sheet,
                                   const gchar *key,
                                   GshmupAnimation *anim)
{
    g_hash_table_insert (sprite_sheet->animations, g_strdup (key), anim);
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

GshmupAnimation *
gshmup_get_sprite_sheet_animation (GshmupSpriteSheet *sprite_sheet,
                                   const gchar *key)
{
    return (GshmupAnimation *) g_hash_table_lookup (sprite_sheet->animations, key);
}
