#include "game.h"
#include <time.h>

static ALLEGRO_DISPLAY *display = NULL;
static ALLEGRO_EVENT_QUEUE *event_queue = NULL;
static ALLEGRO_TIMER *timer = NULL;
static ALLEGRO_BITMAP *buffer = NULL;
static GshmupRect scale;
static float timestep = 1.0f / 60;
static float fps_time;
static int fps = 0;
static int last_fps = 0;
static bool running = false;
static bool redraw = true;
static GshmupScene *current_scene = NULL;
static int key_binds[GSHMUP_KEY_MAX] = {
    ALLEGRO_KEY_UP,
    ALLEGRO_KEY_DOWN,
    ALLEGRO_KEY_LEFT,
    ALLEGRO_KEY_RIGHT,
    ALLEGRO_KEY_Z,
    ALLEGRO_KEY_X,
    ALLEGRO_KEY_ENTER,
};
SCM_VARIABLE_INIT (init_hook, "game-init-hook", scm_make_hook (scm_from_int (0)));
SCM_VARIABLE (s_current_agenda, "current-agenda");

static void
game_destroy (void)
{
    al_destroy_timer (timer);
    al_destroy_event_queue (event_queue);
    al_destroy_display (display);
}

static void
game_update (void)
{
    redraw = true;
    current_scene->update();
}

static void
key_up (int keycode)
{
    for (int i = 0; i < GSHMUP_KEY_MAX; ++i) {
        int gshmup_key = key_binds[i];

        if (gshmup_key == keycode) {
            current_scene->key_up (i);
            break;
        }
    }
}

static void
key_down (int keycode)
{
    for (int i = 0; i < GSHMUP_KEY_MAX; ++i) {
        int gshmup_key = key_binds[i];

        if (gshmup_key == keycode) {
            current_scene->key_down (i);
            break;
        }
    }
}

static void
game_process_event (void)
{
    static ALLEGRO_EVENT event;

    al_wait_for_event(event_queue, &event);

    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        running = false;
    }
    else if (event.type == ALLEGRO_EVENT_TIMER) {
        game_update ();
    }
    else if (event.type == ALLEGRO_EVENT_KEY_UP) {
        key_up (event.keyboard.keycode);

    }
    else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
        if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            running = false;
        }

        key_down (event.keyboard.keycode);
    }
}

static void
game_update_fps (void)
{
    float time = al_get_time ();

    if (time - fps_time >= 1) {
        last_fps = fps;
        fps = 0;
        fps_time = time;
    }

    fps++;
}

static void
game_draw (void)
{
    al_set_target_bitmap (buffer);
    al_clear_to_color (al_map_rgb_f (.04, .04, .04));
    current_scene->draw ();
    al_set_target_backbuffer (display);
    al_clear_to_color (al_map_rgb (0, 0, 0));
    al_draw_scaled_bitmap (buffer, 0, 0, GAME_WIDTH, GAME_HEIGHT,
                           scale.x, scale.y, scale.width, scale.height, 0);
    al_flip_display ();
    game_update_fps ();
}

static void
init_scale (void)
{
    int ww = al_get_display_width (display);
    int wh = al_get_display_height (display);
    int sx = ww / GAME_WIDTH;
    int sy = wh / GAME_HEIGHT;
    int s = fmin (sx, sy);

    /* Calculate how much the buffer should be scaled. */
    scale.width = GAME_WIDTH * s;
    scale.height = GAME_HEIGHT * s;
    scale.x = (ww - scale.width) / 2;
    scale.y = (wh - scale.height) / 2;
}

void gshmup_init_game (void)
{
    /* Initialize Allegro. */
    if (!al_init ()) {
        fprintf (stderr, "failed to initialize allegro!\n");
        exit (-1);
    }

    if (!al_init_primitives_addon ()) {
        fprintf (stderr, "failed to initialize primitives addon!\n");
        exit (-1);
    }

    if (!al_init_image_addon ()) {
        fprintf (stderr, "failed to initialize image addon!\n");
        exit (-1);
    }

    al_init_font_addon ();

    if (!al_init_ttf_addon ()) {
        fprintf (stderr, "failed to initialize ttf addon!\n");
        exit (-1);
    }

    if (!al_install_audio ()) {
        fprintf (stderr, "failed to initialize audio addon!\n");
        exit (-1);
    }

    if (!al_init_acodec_addon ()) {
        fprintf (stderr, "failed to initialize audio codecs addon!\n");
        exit (-1);
    }

    if (!al_reserve_samples (16)) {
        fprintf (stderr, "failed to reserve samples!\n");
        exit (-1);
    }

    if(!al_install_keyboard ()) {
        fprintf (stderr, "failed to initialize keyboard!\n");
        exit (-1);
    }

    display = al_create_display (SCREEN_WIDTH, SCREEN_HEIGHT);
    buffer = al_create_bitmap (GAME_WIDTH, GAME_HEIGHT);
    init_scale ();

    if (!display) {
        fprintf (stderr, "failed to create display!\n");
    }

    al_set_window_title (display, GAME_TITLE);

    /* Register event sources. */
    timer = al_create_timer (timestep);
    event_queue = al_create_event_queue ();
    al_register_event_source (event_queue,
                              al_get_display_event_source (display));
    al_register_event_source (event_queue,
                              al_get_timer_event_source (timer));
    al_register_event_source (event_queue, al_get_keyboard_event_source ());

    /* Run init hook. */
    scm_run_hook (scm_variable_ref (init_hook), scm_list_n (SCM_UNDEFINED));
}

void
gshmup_run_game (void)
{
    running = true;

    al_start_timer (timer);
    fps_time = al_get_time ();

    while (running) {
        game_process_event ();

        if (redraw && al_is_event_queue_empty (event_queue)) {
            redraw = false;
            game_draw ();
        }
    }

    game_destroy ();
}

void
gshmup_set_current_scene (GshmupScene *scene)
{
    if (current_scene) {
        current_scene->destroy ();
        free (current_scene);
    }

    current_scene = scene;
    current_scene->init ();
}

void
gshmup_set_current_agenda (SCM agenda)
{
    s_current_agenda = agenda;
}

int
gshmup_get_fps (void)
{
    return last_fps;
}

SCM_DEFINE (gshmup_s_bind_key, "%bind-key", 2, 0, 0,
            (SCM gshmup_key, SCM keycode),
            "Maps a keycode to a gshmup control.")
{
    key_binds[scm_to_int (gshmup_key)] = scm_to_int (keycode);

    return SCM_UNSPECIFIED;
}

SCM_DEFINE (gshmup_s_game_schedule, "game-schedule", 2, 0, 0,
            (SCM thunk, SCM dt),
            "Schedule @var{thunk} to be called in @{dt} frames.")
{
    scm_call_3 (scm_c_public_ref ("gshmup agenda", "agenda-schedule"),
                s_current_agenda, thunk, dt);

    return SCM_UNSPECIFIED;
}

void
gshmup_game_init_scm (void)
{
#include "game.x"

    scm_c_export ("game-init-hook",
                  s_gshmup_s_game_schedule,
                  NULL);
}
