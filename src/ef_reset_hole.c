#include "ef_effect_control.h"

static void eReset_Hole_init(xyz_t pos, int prio, s16 angle, GAME* game, u16 item_name, s16 arg0, s16 arg1) {
    // TODO
}

static void eReset_Hole_ct(eEC_Effect_c* effect, GAME* game, void* ct_arg) {
    // TODO
}

static void eReset_Hole_mv(eEC_Effect_c* effect, GAME* game) {
    // TODO
}

static void eReset_Hole_dw(eEC_Effect_c* effect, GAME* game) {
    // TODO
}

eEC_PROFILE_c iam_ef_reset_hole = {
    // clang-format off
    &eReset_Hole_init,
    &eReset_Hole_ct,
    &eReset_Hole_mv,
    &eReset_Hole_dw,
    24,
    eEC_NO_CHILD_ID,
    eEC_DEFAULT_DEATH_DIST,
    // clang-format on
};