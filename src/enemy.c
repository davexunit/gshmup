#include "enemy.h"
#include "asset.h"

static GshmupEnemy *current_enemy = NULL;
static scm_t_bits enemy_type_tag;

/* GshmupEnemyType smob is for Guile scripts. */
GshmupEnemyType *
gshmup_check_enemy_type_smob (SCM enemy_type_smob)
{
    scm_assert_smob_type (enemy_type_tag, enemy_type_smob);

    return (GshmupEnemyType *) SCM_SMOB_DATA (enemy_type_smob);
}

SCM_DEFINE (make_enemy_type, "make-enemy-type", 4, 0, 0,
            (SCM sprite_sheet, SCM max_health, SCM hitbox, SCM on_death),
            "Creates a new enemy type.")
{
    SCM smob;
    GshmupEnemyType *enemy_type =
        (GshmupEnemyType *) scm_gc_malloc (sizeof (GshmupEnemyType),
                                           "enemy type");

    enemy_type->sprite_sheet_filename = scm_to_latin1_string (sprite_sheet);
    enemy_type->max_health = scm_to_int (max_health);
    enemy_type->hitbox = gshmup_scm_to_rect (hitbox);
    enemy_type->on_death = on_death;

    SCM_NEWSMOB (smob, enemy_type_tag, enemy_type);

    return smob;
}

static size_t
free_enemy_type (SCM enemy_type_smob)
{
    GshmupEnemyType *enemy_type = (GshmupEnemyType *) SCM_SMOB_DATA (enemy_type_smob);

    g_free (enemy_type->sprite_sheet_filename);

    return 0;
}

static int
print_enemy_type (SCM enemy_type_smob, SCM port, scm_print_state *pstate)
{
    GshmupEnemyType *enemy_type = (GshmupEnemyType *) SCM_SMOB_DATA (enemy_type_smob);
    scm_puts ("#<enemy-type max-health: ", port);
    scm_display (scm_from_int (enemy_type->max_health), port);
    scm_puts (" on-death: ", port);
    scm_display (enemy_type->on_death, port);
    scm_puts (">", port);

    return 1;
}

static void
init_sprite (GshmupEnemy *enemy, GshmupEnemyType *type)
{
    int frames[] = { 0, 1, 2, 3 };
    GshmupSpriteSheet *sprite_sheet =
        gshmup_load_sprite_sheet_asset (type->sprite_sheet_filename);
    GshmupAnimation *anim = gshmup_create_animation (sprite_sheet, 2, 4,
                                                     frames, GSHMUP_ANIM_LOOP);
    gshmup_init_animated_sprite (&enemy->entity.sprite, anim);
    gshmup_play_animation (anim);
}

/* GshmupEnemy is for instances of GshmupEnemyTypes. */
GshmupEnemy *
gshmup_create_enemy (GshmupEnemyType *type, SCM script)
{
    GshmupEnemy *enemy = (GshmupEnemy *) scm_gc_malloc (sizeof (GshmupEnemy),
                                                        "enemy");

    enemy->entity = gshmup_create_entity ("Enemy");
    enemy->entity.hitbox = type->hitbox;
    enemy->health = type->max_health;
    enemy->kill = false;
    enemy->on_death = type->on_death;
    enemy->next = NULL;
    init_sprite (enemy, type);

    /* Schedule script if one is provided. */
    if (scm_is_true (scm_procedure_p (script))) {
        gshmup_schedule (enemy->entity.agenda, 0, script);
    }

    return enemy;
}

void
gshmup_destroy_enemy (GshmupEnemy *enemy)
{
    gshmup_destroy_animation (enemy->entity.sprite.anim);
    g_free (enemy->entity.name);
    scm_gc_free (enemy, sizeof (GshmupEnemy), "enemy");
}

void
gshmup_destroy_enemies (GshmupEnemy *enemy)
{
    while(enemy) {
        GshmupEnemy *next = enemy->next;

        gshmup_destroy_enemy (enemy);
        enemy = next;
    }
}

void
gshmup_draw_enemy (GshmupEnemy *enemy)
{
    gshmup_draw_entity (&enemy->entity);
}

void
gshmup_draw_enemies (GshmupEnemy *enemy)
{
    while (enemy) {
        gshmup_draw_enemy (enemy);
        enemy = enemy->next;
    }
}

void
gshmup_update_enemy (GshmupEnemy *enemy)
{
    current_enemy = enemy;
    gshmup_update_entity (&enemy->entity);
}

GshmupEnemy *
gshmup_update_enemies (GshmupEnemy *enemy)
{
    GshmupEnemy *enemies = enemy;
    GshmupEnemy *prev = NULL;

    while (enemy) {
        GshmupEnemy *next = enemy->next;

        gshmup_update_enemy (enemy);

        if (enemy->kill) {
            gshmup_destroy_enemy (enemy);

            if (prev) {
                prev->next = next;
            } else {
                enemies = NULL;
            }
        }

        prev = enemy;
        enemy = next;
    }

    return enemies;
}

void
gshmup_set_current_enemy (GshmupEnemy *enemy)
{
    current_enemy = enemy;
}

SCM_DEFINE (damage_enemy, "damage-enemy", 1, 0, 0,
            (SCM damage),
            "Reduce enemy health points by @var{damage}")
{
    current_enemy->health -= scm_to_int (damage);

    if (current_enemy->health <= 0) {
        current_enemy->kill = true;

        if (scm_is_true (scm_procedure_p (current_enemy->on_death))) {
            scm_call_0 (current_enemy->on_death);
        }
    }

    return SCM_UNSPECIFIED;
}

SCM_DEFINE (kill_enemy, "kill-enemy", 0, 0, 0,
            (void),
            "Sets enemy's kill flag.")
{
    current_enemy->kill = true;

    return SCM_UNSPECIFIED;
}

void
gshmup_enemy_init_scm (void)
{
    enemy_type_tag = scm_make_smob_type ("<enemy-type>", sizeof (GshmupEnemyType));
    scm_set_smob_mark (enemy_type_tag, 0);
    scm_set_smob_free (enemy_type_tag, free_enemy_type);
    scm_set_smob_print (enemy_type_tag, print_enemy_type);

#include "enemy.x"

    scm_c_export (s_make_enemy_type,
                  s_damage_enemy,
                  s_kill_enemy,
                  NULL);
}
