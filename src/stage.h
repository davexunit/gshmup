#ifndef GSHMUP_STAGE_H
#define GSHMUP_STAGE_H

#include "common.h"

/*
 * A stage contains the script that spawns enemies for the player to
 * fight.
 */
typedef struct {
    char *name;
    char *description;
    SCM script;
    SCM agenda;
} GshmupStage;

GshmupStage *gshmup_check_stage_smob (SCM stage_smob);
void gshmup_start_stage (GshmupStage *stage);
void gshmup_update_stage (GshmupStage *stage);
void gshmup_stage_init_scm (void);

#endif
