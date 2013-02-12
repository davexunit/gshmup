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

GshmupStage *gshmup_create_stage (const char *name, const char *description,
                                  SCM script);
void gshmup_destroy_stage (GshmupStage *stage);
void gshmup_update_stage (GshmupStage *stage);

#endif
