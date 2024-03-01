#ifndef AC_DESIGN_H
#define AC_DESIGN_H

#include "types.h"
#include "m_actor.h"
#include "m_field_info.h"
#include "m_olib.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct design_actor_s DESIGN_ACTOR;

/* Keep in mind that 0x178 * 16 * 16 = 0x17800 (96k) bytes */

typedef struct design_s {
    s8 player_no;
    u8 design_no;
    u16 bg_y;
} aDS_design_c;

/* sizeof(DESIGN_ACTOR) == 0x574 */
struct design_actor_s {
    /* 0x000 */ ACTOR actor_class;
    /* 0x174 */ BlockOrUnit_c player_block;
    /* 0x17C */ aDS_design_c designs[UT_TOTAL_NUM];
};

extern ACTOR_PROFILE Design_Profile;

#ifdef __cplusplus
}
#endif

#endif
