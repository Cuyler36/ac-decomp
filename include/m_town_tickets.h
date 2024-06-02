#ifndef M_TOWN_TICKETS_H
#define M_TOWN_TICKETS_H

#include "types.h"
#include "m_play_h.h"
#include "libu64/gfxprint.h"

#ifdef __cplusplus
extern "C" {
#endif

#define mTT_MAX_TICKETS 999999999
#define mTT_TASK_NUM 5

enum {
    mTT_STATE_IN_PROGRESS,
    mTT_STATE_FAILED,
    mTT_STATE_COMPLETED,

    mTT_STATE_NUM
};

enum {
    mTT_TASK_TYPE_NONE,

    mTT_TASK_TYPE_NUM
};

typedef struct town_ticket_task_s {
    /* 0x00 */ u8 type; /* 0-255 for type of task */
    /* 0x01 */ u8 state;
    /* 0x02 */ u16 progress;
    /* 0x04 */ u16 target_progress;
    /* 0x06 */ u16 ticket_count;
} mTT_task_c;

extern void tickets_draw(GAME_PLAY* play);
extern void tickets_move(GAME_PLAY* play);
extern void tickets_debug(gfxprint_t* gfxprint);

#ifdef __cplusplus
}
#endif

#endif
