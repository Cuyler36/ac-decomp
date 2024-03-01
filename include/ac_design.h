#ifndef AC_DESIGN_H
#define AC_DESIGN_H

#include "types.h"
#include "m_actor.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct design_actor_s DESIGN_ACTOR;

/* Keep in mind that 0x178 * 16 * 16 = 0x17800 (96k) bytes */

/* sizeof(DESIGN_ACTOR) == 0x178 */
struct design_actor_s {
    /* 0x000 */ ACTOR actor_class;
    /* 0x174 */ u16 pl_no;
    /* 0x176 */ u16 design_no;
};

extern ACTOR_PROFILE Design_Profile;

#ifdef __cplusplus
}
#endif

#endif
