#ifndef M_TOWN_TICKETS_H
#define M_TOWN_TICKETS_H

#include "types.h"
#include "m_play_h.h"
#include "libu64/gfxprint.h"

#ifdef __cplusplus
extern "C" {
#endif

#define mTT_MAX_TICKETS 999999999

extern void tickets_draw(GAME_PLAY* play);
extern void tickets_move(GAME_PLAY* play);
extern void tickets_debug(gfxprint_t* gfxprint);

#ifdef __cplusplus
}
#endif

#endif
