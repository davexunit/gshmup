#include "splash_screen.h"
#include "shooter.h"
#include "player.h"

ALLEGRO_BITMAP *background = NULL;
ALLEGRO_BITMAP *title = NULL;
ALLEGRO_BITMAP *font_image = NULL;
ALLEGRO_FONT *font = NULL;
ALLEGRO_COLOR font_color;
const char *text = "Press shoot button";

/* Configuration variables. */
SCM_VARIABLE (s_splash_screen_background, "*splash-screen-background*");
SCM_VARIABLE (s_splash_screen_title, "*splash-screen-title*");

static void
splash_init (void)
{
    const char *background_file =
        scm_to_locale_string (scm_variable_ref (s_splash_screen_background));
    const char *title_file =
        scm_to_locale_string (scm_variable_ref (s_splash_screen_title));
    /* Character code ranges in the bitmap font we're using. */
    int ranges[] = {
        32, 47,
        48, 63,
        64, 79,
        80, 95,
        96, 111,
        112, 127,
    };

    background = al_load_bitmap (background_file);
    title = al_load_bitmap (title_file);
    font_image = al_load_bitmap ("data/fonts/font.png");
    font = al_grab_font_from_bitmap (font_image, 6, ranges);
    font_color = al_map_rgba_f (1, 1, 1, 1);
}

static void
splash_destroy (void)
{
    al_destroy_bitmap (background);
    al_destroy_bitmap (title);
}

static void
splash_draw (void)
{
    float title_x = (GAME_WIDTH - al_get_bitmap_width (title)) / 2;
    float title_y = 100;

    al_draw_bitmap (background, 0, 0, 0);
    al_draw_bitmap (title, title_x, title_y, 0);
    al_draw_text (font, font_color,
                  GAME_WIDTH / 2, title_y + al_get_bitmap_height (title),
                  ALLEGRO_ALIGN_CENTRE, text);
    al_draw_textf (font, font_color, GAME_WIDTH / 2, GAME_HEIGHT - 16,
                   ALLEGRO_ALIGN_CENTRE, "Credits  %d",
                   gshmup_get_initial_player_credits ());
}

static void
splash_update (void)
{

}

static void
splash_key_down (int keycode)
{
    switch (keycode) {
    case GSHMUP_KEY_SHOOT:
    case GSHMUP_KEY_START:
        gshmup_set_current_scene (gshmup_create_shooter_scene ());
        break;
    }
}

static void
splash_key_up (int keycode)
{

}

GshmupScene *
gshmup_create_splash_screen_scene (void)
{
    GshmupScene *scene = (GshmupScene *) malloc (sizeof (GshmupScene));

    scene->init = splash_init;
    scene->destroy = splash_destroy;
    scene->draw = splash_draw;
    scene->update = splash_update;
    scene->key_down = splash_key_down;
    scene->key_up = splash_key_up;

    return scene;
}

void
gshmup_splash_screen_init_scm (void)
{
#include "splash_screen.x"

    scm_c_export ("*splash-screen-background*",
                  "*splash-screen-title*",
                  NULL);
}
