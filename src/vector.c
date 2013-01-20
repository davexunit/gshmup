#include "vector.h"

static scm_t_bits vector2_tag;

GshmupVector2
gshmup_create_vector2 (float x, float y)
{
    GshmupVector2 v;

    v.x = x;
    v.y = y;

    return v;
}

bool
gshmup_vector2_equal (GshmupVector2 a, GshmupVector2 b)
{
    return a.x == b.x && a.y == b.y;
}

float
gshmup_vector2_mag (GshmupVector2 v)
{
    return sqrt (v.x * v.x + v.y * v.y);
}

float
gshmup_vector2_dot (GshmupVector2 a, GshmupVector2 b)
{
    return a.x * b.x + a.y * b.y;
}

float
gshmup_vector2_cross (GshmupVector2 a, GshmupVector2 b)
{
    return a.x * b.y - b.x * a.y;
}

float
gshmup_vector2_angle (GshmupVector2 v)
{
    return atan2 (v.y, v.x);
}

GshmupVector2
gshmup_vector2_zero ()
{
    GshmupVector2 v;

    v.x = 0;
    v.y = 0;

    return v;
}

GshmupVector2
gshmup_vector2_add (GshmupVector2 a, GshmupVector2 b)
{
    GshmupVector2 c;

    c.x = a.x + b.x;
    c.y = a.y + b.y;

    return c;
}

GshmupVector2
gshmup_vector2_sub (GshmupVector2 a, GshmupVector2 b)
{
    GshmupVector2 c;

    c.x = a.x - b.x;
    c.y = a.y - b.y;

    return c;
}

GshmupVector2
gshmup_vector2_scale (GshmupVector2 v, float scalar)
{
    GshmupVector2 a;

    a.x = v.x * scalar;
    a.y = v.y * scalar;

    return a;
}

GshmupVector2
gshmup_vector2_norm (GshmupVector2 v)
{
    float m = gshmup_vector2_mag (v);
    GshmupVector2 n = gshmup_vector2_zero ();

    if (m != 0) {
        n.x = v.x / m;
        n.y = v.y / m;
    }

    return n;
}

GshmupVector2
gshmup_vector2_from_polar (float radius, float angle)
{
    GshmupVector2 v;

    v.x = cos (angle) * radius;
    v.y = sin (angle) * radius;

    return v;
}

GshmupVector2
gshmup_vector2_right_normal (GshmupVector2 v)
{
    GshmupVector2 right;

    right.x = -v.y;
    right.y = v.x;

    return right;
}

GshmupVector2
gshmup_vector2_left_normal (GshmupVector2 v)
{
    GshmupVector2 left;

    left.x = v.y;
    left.y = -v.x;

    return left;
}

GshmupVector2
gshmup_scm_to_vector2 (SCM s_v)
{
    scm_assert_smob_type (vector2_tag, s_v);

    return *((GshmupVector2 *) SCM_SMOB_DATA (s_v));
}

SCM
gshmup_scm_from_vector2 (GshmupVector2 v)
{
    SCM smob;
    GshmupVector2 *new_v = (GshmupVector2 *) malloc (sizeof (GshmupVector2));

    *new_v = v;

    SCM_NEWSMOB (smob, vector2_tag, new_v);

    return smob;
}

SCM_DEFINE (gshmup_s_make_vector2, "make-vector2", 2, 0, 0,
            (SCM x, SCM y),
            "Make a 2D vector.")
{
    return gshmup_scm_from_vector2 (gshmup_create_vector2 (scm_to_double (x),
                                                  scm_to_double (y)));
}

SCM_DEFINE (gshmup_s_vector2_from_polar, "vector2-from-polar", 2, 0, 0,
            (SCM radius, SCM angle),
            "Make a 2D vector from a polar coordinate pair.")
{
    GshmupVector2 v = gshmup_vector2_from_polar (scm_to_double (radius),
                                           gshmup_deg_to_rad (scm_to_double (angle)));

    return gshmup_scm_from_vector2 (v);
}

SCM_DEFINE (gshmup_s_vector2_x, "vector2-x", 1, 0, 0,
            (SCM v),
            "Return x coordinate.")
{
    GshmupVector2 c_v = gshmup_scm_to_vector2 (v);

    return scm_from_double (c_v.x);
}

SCM_DEFINE (gshmup_s_vector2_y, "vector2-y", 1, 0, 0,
            (SCM v),
            "Return y coordinate.")
{
    GshmupVector2 c_v = gshmup_scm_to_vector2 (v);

    return scm_from_double (c_v.y);
}


SCM_DEFINE (gshmup_s_vector2_add, "vector2-add", 0, 0, 1,
            (SCM vectors),
            "Sum of vectors.")
{
    GshmupVector2 sum = gshmup_vector2_zero ();

    while (scm_is_false (scm_null_p (vectors))) {
        SCM s_v = scm_car (vectors);
        GshmupVector2 v = gshmup_scm_to_vector2 (s_v);

        sum = gshmup_vector2_add (sum, v);

        vectors = scm_cdr (vectors);
    }

    return gshmup_scm_from_vector2 (sum);
}

SCM_DEFINE (gshmup_s_vector2_sub, "vector2-sub", 1, 0, 1,
            (SCM v, SCM vectors),
            "Difference of vectors.")
{
    GshmupVector2 difference = gshmup_scm_to_vector2 (v);

    while (scm_is_false (scm_null_p (vectors))) {
        SCM s_v = scm_car (vectors);
        GshmupVector2 v = gshmup_scm_to_vector2 (s_v);

        difference = gshmup_vector2_sub (difference, v);

        vectors = scm_cdr (vectors);
    }

    return gshmup_scm_from_vector2 (difference);
}

SCM_DEFINE (gshmup_s_vector2_scale, "vector2-scale", 2, 0, 0,
            (SCM v, SCM scalar),
            "Multiply vector by a scalar.")
{
    return gshmup_scm_from_vector2 (gshmup_vector2_scale (gshmup_scm_to_vector2 (v),
                                                    scm_to_double (scalar)));
}

SCM_DEFINE (gshmup_s_vector2_norm, "vector2-norm", 1, 0, 0,
            (SCM v),
            "Return normalized vector.")
{
    return gshmup_scm_from_vector2 (gshmup_vector2_norm (gshmup_scm_to_vector2 (v)));
}

SCM_DEFINE (gshmup_s_vector2_mag, "vector2-mag", 1, 0, 0,
            (SCM v),
            "Return vector magnitude.")
{
    return scm_from_double (gshmup_vector2_mag (gshmup_scm_to_vector2 (v)));
}

SCM_DEFINE (gshmup_s_vector2_angle, "vector2-angle", 1, 0, 0,
            (SCM v),
            "Return vector direction in degrees.")
{
    float theta = gshmup_vector2_angle (gshmup_scm_to_vector2 (v));

    return scm_from_double (gshmup_rad_to_deg (theta));
}

SCM_DEFINE (gshmup_s_vector2_dot, "vector2-dot", 2, 0, 0,
            (SCM v1, SCM v2),
            "Return dot product of 2 vectors.")
{
    return scm_from_double (gshmup_vector2_dot (gshmup_scm_to_vector2 (v1),
                                             gshmup_scm_to_vector2 (v2)));
}

SCM_DEFINE (gshmup_s_vector2_cross, "vector2-cross", 2, 0, 0,
            (SCM v1, SCM v2),
            "Return cross product of 2 vectors.")
{
    return scm_from_double (gshmup_vector2_cross (gshmup_scm_to_vector2 (v1),
                                               gshmup_scm_to_vector2 (v2)));
}

SCM_DEFINE (gshmup_s_vector2_right_normal, "vector2-right-normal", 1, 0, 0,
            (SCM v),
            "Return right normal.")
{
    return gshmup_scm_from_vector2 (gshmup_vector2_right_normal (gshmup_scm_to_vector2 (v)));
}

SCM_DEFINE (gshmup_s_vector2_left_normal, "vector2-left-normal", 1, 0, 0,
            (SCM v),
            "Return left normal.")
{
    return gshmup_scm_from_vector2 (gshmup_vector2_left_normal (gshmup_scm_to_vector2 (v)));
}

static size_t
free_vector2 (SCM v)
{
    GshmupVector2 *vector = (GshmupVector2 *) SCM_SMOB_DATA (v);

    free (vector);

    return 0;
}

static int
print_vector2 (SCM v, SCM port, scm_print_state *pstate)
{
    scm_puts ("#<vector2 x: ", port);
    scm_display (gshmup_s_vector2_x (v), port);
    scm_puts (" y: ", port);
    scm_display (gshmup_s_vector2_y (v), port);
    scm_puts (">", port);

    return 1;
}

static SCM
vector2_equal_p (SCM v1, SCM v2)
{
    return scm_from_bool (gshmup_vector2_equal (gshmup_scm_to_vector2 (v1),
                                             gshmup_scm_to_vector2 (v2)));
}

void
gshmup_init_vector2 (void)
{
    vector2_tag = scm_make_smob_type ("vector2", sizeof (GshmupVector2));
    scm_set_smob_mark (vector2_tag, 0);
    scm_set_smob_free (vector2_tag, free_vector2);
    scm_set_smob_print (vector2_tag, print_vector2);
    scm_set_smob_equalp (vector2_tag, vector2_equal_p);

#include "vector.x"

    scm_c_export (s_gshmup_s_make_vector2,
                  s_gshmup_s_vector2_from_polar,
                  s_gshmup_s_vector2_x,
                  s_gshmup_s_vector2_y,
                  s_gshmup_s_vector2_add,
                  s_gshmup_s_vector2_sub,
                  s_gshmup_s_vector2_scale,
                  s_gshmup_s_vector2_norm,
                  s_gshmup_s_vector2_mag,
                  s_gshmup_s_vector2_angle,
                  s_gshmup_s_vector2_dot,
                  s_gshmup_s_vector2_cross,
                  s_gshmup_s_vector2_left_normal,
                  s_gshmup_s_vector2_right_normal,
                  NULL);
}
