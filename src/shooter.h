#ifndef GSHMUP_SHOOTER_H
#define GSHMUP_SHOOTER_H

#include "common.h"
#include "scene.h"

#define GSHMUP_MAX_STAGE_NAME_LENGTH 128
#define GSHMUP_MAX_STAGE_DESC_LENGTH 256

typedef struct {
    char name[GSHMUP_MAX_STAGE_NAME_LENGTH];
    char description[GSHMUP_MAX_STAGE_DESC_LENGTH];
} GshmupStage;

GshmupScene *gshmup_create_shooter_scene (void);
void gshmup_shooter_init_scm (void);

#endif
