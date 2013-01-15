#ifndef GSHMUP_VECTOR2_H
#define GSHMUP_VECTOR2_H

#include "common.h"

typedef struct {
    float x, y;
} GshmupVector2;

/* C API. */
GshmupVector2 gshmup_create_vector2 (float x, float y);
bool gshmup_vector2_equal (GshmupVector2 a, GshmupVector2 b);
float gshmup_vector2_mag (GshmupVector2 v);
float gshmup_vector2_dot (GshmupVector2 a, GshmupVector2 b);
float gshmup_vector2_cross (GshmupVector2 a, GshmupVector2 b);
float gshmup_vector2_angle (GshmupVector2 v);
GshmupVector2 gshmup_scm_to_vector2 (SCM s_v);
SCM gshmup_scm_from_vector2 (GshmupVector2 v);
GshmupVector2 gshmup_vector2_zero ();
GshmupVector2 gshmup_vector2_add (GshmupVector2 a, GshmupVector2 b);
GshmupVector2 gshmup_vector2_sub (GshmupVector2 a, GshmupVector2 b);
GshmupVector2 gshmup_vector2_scale (GshmupVector2 v, float scalar);
GshmupVector2 gshmup_vector2_norm (GshmupVector2 v);
GshmupVector2 gshmup_vector2_from_polar (float radius, float angle);
GshmupVector2 gshmup_vector2_right_normal (GshmupVector2 v);
GshmupVector2 gshmup_vector2_left_normal (GshmupVector2 v);

/* Scheme bindings. */
SCM gshmup_s_make_vector2 (SCM x, SCM y);
SCM gshmup_s_vector2_from_polar (SCM radius, SCM angle);
SCM gshmup_s_vector2_x (SCM v);
SCM gshmup_s_vector2_y (SCM v);
SCM gshmup_s_vector2_add (SCM vectors);
SCM gshmup_s_vector2_sub (SCM v, SCM vectors);
SCM gshmup_s_vector2_scale (SCM v, SCM scalar);
SCM gshmup_s_vector2_norm (SCM v);
SCM gshmup_s_vector2_mag (SCM v);
SCM gshmup_s_vector2_angle (SCM v);
SCM gshmup_s_vector2_dot (SCM v1, SCM v2);
SCM gshmup_s_vector2_cross (SCM v1, SCM v2);
SCM gshmup_s_vector2_right_normal (SCM v);
SCM gshmup_s_vector2_left_normal (SCM v);
void gshmup_init_vector2 (void);

#endif
