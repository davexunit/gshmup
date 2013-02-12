#include "stage.h"
#include "string.h"

GshmupStage *
gshmup_create_stage (const char *name, const char *description, SCM script)
{
    GshmupStage *stage = (GshmupStage *) scm_gc_malloc (sizeof (GshmupStage),
                                                        "stage");

    stage->name = (char *) malloc (strlen (name) + 1);
    stage->description = (char *) malloc (strlen (description) + 1);
    strcpy (stage->name, name);
    strcpy (stage->description, description);
    stage->script = script;
    stage->agenda = gshmup_create_agenda ();

    if (scm_is_true (scm_procedure_p (script))) {
        gshmup_schedule (stage->agenda, 0, script);
    }

    return stage;
}

void
gshmup_destroy_stage (GshmupStage *stage)
{
    free (stage->name);
    free (stage->description);
    scm_gc_free (stage, sizeof (GshmupStage), "stage");
}

void
gshmup_update_stage (GshmupStage *stage)
{
    gshmup_update_agenda (stage->agenda);
}
