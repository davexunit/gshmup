#include "stage.h"
#include "string.h"

static scm_t_bits stage_tag;

GshmupStage *
gshmup_check_stage_smob (SCM stage_smob)
{
    scm_assert_smob_type (stage_tag, stage_smob);

    return (GshmupStage *) SCM_SMOB_DATA (stage_smob);
}

SCM_DEFINE (make_stage, "make-stage", 3, 0, 0,
            (SCM name, SCM description, SCM script),
            "Creates a new stage.")
{
    SCM smob;
    GshmupStage *stage = (GshmupStage *) scm_gc_malloc (sizeof (GshmupStage),
                                                        "stage");

    stage->name = scm_to_latin1_string (name);
    stage->description = scm_to_latin1_string (description);
    stage->script = script;
    stage->agenda = gshmup_create_agenda ();

    SCM_NEWSMOB (smob, stage_tag, stage);

    return smob;
}

void
gshmup_start_stage (GshmupStage *stage)
{
    gshmup_clear_agenda (stage->agenda);

    if (scm_is_true (scm_procedure_p (stage->script))) {
        gshmup_schedule (stage->agenda, 0, stage->script);
    }
}

void
gshmup_update_stage (GshmupStage *stage)
{
    gshmup_update_agenda (stage->agenda);
}

static size_t
free_stage (SCM stage_smob)
{
    GshmupStage *stage = (GshmupStage *) SCM_SMOB_DATA (stage_smob);

    g_free (stage->name);
    g_free (stage->description);

    return 0;
}

static int
print_stage (SCM stage_smob, SCM port, scm_print_state *pstate)
{
    GshmupStage *stage = (GshmupStage *) SCM_SMOB_DATA (stage_smob);
    scm_puts ("#<stage name: ", port);
    scm_display (scm_from_latin1_string (stage->name), port);
    scm_puts (" description: ", port);
    scm_display (scm_from_latin1_string (stage->description), port);
    scm_puts (" script: ", port);
    scm_display (stage->script, port);
    scm_puts (">", port);

    return 1;
}

void
gshmup_stage_init_scm (void)
{
    stage_tag = scm_make_smob_type ("<stage>", sizeof (GshmupStage));
    scm_set_smob_mark (stage_tag, 0);
    scm_set_smob_free (stage_tag, free_stage);
    scm_set_smob_print (stage_tag, print_stage);

#include "stage.x"

    scm_c_export (s_make_stage,
                  NULL);
}
