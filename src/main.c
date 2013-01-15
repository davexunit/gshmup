#include "common.h"
#include "rect.h"
#include "game.h"
#include "shooter.h"

SCM_DEFINE (gshmup_s_start, "gshmup-start", 0, 0, 0,
            (void),
            "Start playing gshmup!")
{
    printf ("Starting gshmup...\n");
    gshmup_init_vector2 ();
    gshmup_init_rect ();
    gshmup_init_game ();
    gshmup_set_current_scene (gshmup_create_shooter_scene ());
    gshmup_run_game ();
    printf ("Exiting...\n");

    return SCM_UNSPECIFIED;
}

void
gshmup_init (void)
{
#include "main.x"

    scm_c_export (s_gshmup_s_start, NULL);
    gshmup_game_init_scm ();
    gshmup_shooter_init_scm ();
}
