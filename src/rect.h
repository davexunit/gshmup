#ifndef GSHMUP_RECT_H
#define GSHMUP_RECT_H

#include "common.h"

typedef struct {
    float x, y;
    float width, height;
} GshmupRect;

/* C API. */
GshmupRect gshmup_create_rect (float x, float y, float width, float height);
GshmupRect gshmup_rect_move (GshmupRect rect, GshmupVector2 delta);
GshmupRect gshmup_rect_scale (GshmupRect rect, GshmupVector2 scale);
GshmupRect gshmup_rect_center (GshmupRect rect, GshmupVector2 center);
GshmupVector2 gshmup_get_rect_center (GshmupRect rect);
bool gshmup_rect_collide_point (GshmupRect rect, GshmupVector2 p);
bool gshmup_rect_collide_rect (GshmupRect rect, GshmupRect other);
void gshmup_draw_rect (GshmupRect rect, ALLEGRO_COLOR fill_color,
                       ALLEGRO_COLOR border_color);


/* Scheme bindings. */
GshmupRect gshmup_scm_to_rect (SCM rect_smob);
SCM gshmup_scm_from_rect (GshmupRect rect);
SCM gshmup_s_make_rect (SCM x, SCM y, SCM width, SCM height);
SCM gshmup_s_rect_x (SCM rect);
SCM gshmup_s_rect_y (SCM rect);
SCM gshmup_s_rect_width (SCM rect);
SCM gshmup_s_rect_height (SCM rect);
SCM gshmup_s_rect_move (SCM rect, SCM delta);
SCM gshmup_s_rect_center (SCM rect, SCM center);
SCM gshmup_s_rect_collide_point (SCM rect, SCM point);
SCM gshmup_s_rect_collide_rect (SCM rect1, SCM rect2);
void gshmup_init_rect (void);

#endif
