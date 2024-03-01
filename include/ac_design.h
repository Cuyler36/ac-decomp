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

/* sizeof(aDS_design_c) == 0x08 */
typedef struct design_s {
    /* 0x00 */ s8 player_no;
    /* 0x01 */ u8 design_no;
    /* 0x02 */ u16 bg_y;
    /* 0x04 */ s16 rot_x;
    /* 0x06 */ s16 rot_z;
} aDS_design_c;

/* sizeof(DESIGN_ACTOR) == 0x97C */
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
