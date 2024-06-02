#include "m_town_tickets.h"

#include "m_common_data.h"
#include "m_font.h"
#include "m_msg.h"
#include "sys_matrix.h"
#include "m_player_lib.h"

#define TICKETS_DEBUG

#ifndef TICKETS_POSITION_DEBUG
static const f32 pos_x = -676.0f;
static const f32 pos_y = 708.0f;
#else
static f32 pos_x = -676.0f;
static f32 pos_y = 708.0f;
#endif

static f32 alpha = 0.0f;
static int timer = 0;
static int last_addressable_type = 0;
static BOOL disabled = FALSE;

static mTT_task_c tasks[mTT_TASK_NUM];

extern void mTT_disable(void) {
    disabled = TRUE;
}

extern void mTT_enable(void) {
    disabled = FALSE;
}

extern BOOL mTT_isDisabled(void) {
    return disabled;
}

extern mTT_task_c* mTT_getTask(int idx) {
    if (idx < 0 || idx >= mTT_TASK_NUM) {
        return NULL;
    }

    return &tasks[idx];
}

static void mTT_clearTask(mTT_task_c* const task) {
    task->type = mTT_TASK_TYPE_NONE;
    task->ticket_count = 0;
    task->progress = 0;
    task->target_progress = 0;
    task->state = mTT_STATE_IN_PROGRESS;
}

extern BOOL mTT_isTaskComplete(int idx) {
    const mTT_task_c* const task = mTT_getTask(idx);

    if (task != NULL) {
        return task->state == mTT_STATE_COMPLETED;
    }

    return FALSE;
}

extern BOOL mTT_checkUpdateTaskComplete(int idx) {
    mTT_task_c* const task = mTT_getTask(idx);

    if (task != NULL) {
        if (task->state == mTT_STATE_IN_PROGRESS && task->progress >= task->target_progress) {
            task->state = mTT_STATE_COMPLETED;
            return TRUE;
        }
    }

    return FALSE;
}

extern void mTT_incrementTaskState(int type, int progress) {
    int i;

    for (i = 0; i < mTT_TASK_NUM; i++) {
        if (tasks[i].type == type && tasks[i].state == mTT_STATE_IN_PROGRESS) {
            tasks[i].progress += progress;
            mTT_checkUpdateTaskComplete(i);
        }
    }
}

extern void mTT_checkCompletedTasks(void) {
    int i;

    for (i = 0; i < mTT_TASK_NUM; i++) {
        if (tasks[i].type != mTT_TASK_TYPE_NONE && tasks[i].state == mTT_STATE_COMPLETED) {
            Now_Private->town_tickets = MIN(Now_Private->town_tickets + tasks[i].ticket_count, mTT_MAX_TICKETS);
            mTT_clearTask(&tasks[i]);
        }
    }
}

static f32 tickets_calc_disp_alpha_rate(GAME_PLAY* play) {
    if (mDemo_CheckDemo() || mEv_CheckTitleDemo() > 0 || disabled) {
        last_addressable_type = mPlayer_ADDRESSABLE_FALSE_MOVEMENT;
        add_calc(&alpha, 0.0f, 1.0f - sqrtf(0.8), 0.1f,
                 0.005f); /* quickly fade out on demo screen or title or force disabled */
    } else if (last_addressable_type == mPlayer_ADDRESSABLE_TRUE && play->submenu.process_status == mSM_PROCESS_WAIT) {
        add_calc(&alpha, 1.0f, 1.0f - sqrtf(0.8), 0.0425f, 0.0005f); /* fade in */
    } else {
        add_calc(&alpha, 0.0f, 1.0f - sqrtf(0.8), 0.0425f, 0.0005f); /* fade out */
    }
}

static void tickets_move_debug(void) {
#ifdef TICKETS_DEBUG
    if (chkButton(BUTTON_Z)) {
        Private_c* priv = Now_Private;
        u32 tickets;
        u32 incr = 1;

        if (chkTrigger(BUTTON_DDOWN)) {
            priv->town_tickets = 0;
            return;
        } else if (chkTrigger(BUTTON_DUP)) {
            priv->town_tickets = mTT_MAX_TICKETS;
            return;
        }

        if (chkButton(BUTTON_A)) {
            incr = 10;
        } else if (chkButton(BUTTON_B)) {
            incr = 100;
        }

        if (chkTrigger(BUTTON_L)) {
            tickets = priv->town_tickets - incr;
            if (tickets < 0) {
                tickets = 0;
            }
            priv->town_tickets = tickets;
        } else if (chkTrigger(BUTTON_R)) {
            tickets = priv->town_tickets + incr;
            if (tickets > mTT_MAX_TICKETS) {
                tickets = mTT_MAX_TICKETS;
            }
            priv->town_tickets = tickets;
        }
    } else {
#ifdef TICKETS_POSITION_DEBUG
        if (chkButton(BUTTON_L)) {
            pos_x += 1.0f;
        } else if (chkButton(BUTTON_R)) {
            pos_x -= 1.0f;
        }

        if (chkButton(BUTTON_DLEFT)) {
            pos_y += 1.0f;
        } else if (chkButton(BUTTON_DRIGHT)) {
            pos_y -= 1.0f;
        }
#endif
    }
#endif
}

extern void tickets_move(GAME_PLAY* play) {
    int addressable_type = mPlib_Get_address_able_display();
    int update;

    if (last_addressable_type != addressable_type) {
        update = FALSE;
        timer++;

        if (addressable_type == mPlayer_ADDRESSABLE_FALSE_USING_TOOL) {
            if (timer > 50) {
                update = TRUE;
            }
        } else if (addressable_type != mPlayer_ADDRESSABLE_TRUE) {
            if (timer > 30 || addressable_type == mPlayer_ADDRESSABLE_FALSE_TALKING) {
                update = TRUE;
            }
        } else if (timer > 50 || addressable_type == mPlayer_ADDRESSABLE_FALSE_TALKING) {
            update = TRUE;
        }

        if (update == TRUE) {
            timer = 0;
            last_addressable_type = addressable_type;
        }
    } else {
        timer = 0;
    }

    tickets_calc_disp_alpha_rate(play);
    tickets_move_debug();
}

extern void tickets_debug(gfxprint_t* gfxprint) {
#ifdef TICKETS_POSITION_DEBUG
    gfxprint_locate8x8(gfxprint, 8, 8);
    gfxprint_color(gfxprint, 255, 200, 200, 255);
    gfxprint_printf(gfxprint, "x: %.02f", pos_x);
    gfxprint_locate8x8(gfxprint, 8, 9);
    gfxprint_printf(gfxprint, "y: %.02f", pos_y);
#endif
}

// clang-format off
static u8 con_namefuti_TXT[] ATTRIBUTE_ALIGN(32) = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x39, 0xEF, 0x00, 0x3B, 0xFF, 0xFF, 0x09, 0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x00, 0x36, 0x9C, 0xEF,
    0x9D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x57, 0x9A, 0xCD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x00, 0x00, 0x34, 0x56, 0x78, 0x89, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x00, 0x00, 0xAA, 0xBB, 0xCC, 0xCD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x00, 0x00, 0xDD, 0xEE, 0xEE, 0xEE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x1D, 0x00, 0x00, 0x00, 0xBF, 0x00, 0x00, 0x06, 0xFF,
    0x00, 0x00, 0x0D, 0xFF, 0x00, 0x00, 0x3F, 0xFF, 0x00, 0x00, 0x6F, 0xFF, 0x00, 0x00, 0x7F, 0xFF,
    0xCF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x7F, 0xFF, 0x00, 0x00, 0x6F, 0xFF, 0x00, 0x00, 0x4F, 0xFF, 0x00, 0x00, 0x0E, 0xFF,
    0x00, 0x00, 0x08, 0xFF, 0x00, 0x00, 0x01, 0xDF, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x04,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3C, 0xFF, 0xFF, 0xFF, 0x00, 0x7E, 0xFF, 0xFF, 0x00, 0x01, 0x7C, 0xFF, 0x00, 0x00, 0x00, 0x49,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDF, 0xFF, 0xFF, 0xFF,
    0x03, 0x69, 0xCF, 0xFF, 0x00, 0x00, 0x01, 0x46, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0x8A, 0xCE, 0xFF, 0xFF, 0x00, 0x00, 0x13, 0x45, 0x00, 0x00, 0x00, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x78, 0x9A, 0xBB, 0xCD, 0x00, 0x00, 0x00, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDE, 0xEF, 0xFF, 0xFF, 0x00, 0x01, 0x11, 0x22,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x23, 0x33, 0x33, 0x34,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x44, 0x44, 0x44, 0x44,
};

static Vtx con_kaiwaname_v[] = {
    /* Outline */
    { -1792, 992, 0,   1,     0 << 5,  0 << 5,    124, 124, 124, 124 },
    { -1792, 554, 0,   1,     0 << 5, 32 << 5,    124, 124, 124, 124 },
    {  -224, 554, 0,   1,   128 << 5, 32 << 5,    124, 124, 124, 124 },
    {  -224, 992, 0,   1,   128 << 5,  0 << 5,    124, 124, 124, 124 },

    /* Background */
    { -1760, 960, 0,   1,     0 << 5,  0 << 5,    124, 124, 124, 124 },
    { -1760, 586, 0,   1,     0 << 5, 32 << 5,    124, 124, 124, 124 },
    {  -256, 586, 0,   1,   128 << 5, 32 << 5,    124, 124, 124, 124 },
    {  -256, 960, 0,   1,   128 << 5,  0 << 5,    124, 124, 124, 124 },
};

static Gfx tickets_mode[] = {
    gsSPLoadGeometryMode(G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH),
    gsDPSetOtherMode(G_AD_DISABLE | G_CD_DISABLE | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE, G_AC_NONE | G_ZS_PIXEL | G_RM_CLD_SURF | G_RM_CLD_SURF2),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetCombineLERP(PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIM_LOD_FRAC, 0, 0, 0, 0, COMBINED, 0, 0, 0, COMBINED),
    gsSPEndDisplayList(),
};

static Gfx con_kaiwaname_modelT[] = {
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPLoadTextureBlock_4b_Dolphin(con_namefuti_TXT, G_IM_FMT_I, 64, 32, 15, GX_MIRROR, GX_MIRROR, 0, 0),
    gsSPVertex(&con_kaiwaname_v[0], 4, 0),
    gsSPNTrianglesInit_5b(2,       // tri count
                          0, 1, 2, // tri0
                          2, 3, 0, // tri1
                          0, 0, 0  // tri2
                          ),
    gsSPEndDisplayList(),
};

static Gfx con_kaiwaname_model2T[] = {
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPLoadTextureBlock_4b_Dolphin(con_namefuti_TXT, G_IM_FMT_I, 64, 32, 15, GX_MIRROR, GX_MIRROR, 0, 0),
    gsSPVertex(&con_kaiwaname_v[4], 4, 0),
    gsSPNTrianglesInit_5b(2,       // tri count
                          0, 1, 2, // tri0
                          2, 3, 0, // tri1
                          0, 0, 0  // tri2
                          ),
    gsSPEndDisplayList(),
};

// clang-format on

extern void tickets_draw(GAME_PLAY* play) {
    if (mFI_GET_TYPE(mFI_GetFieldId()) == mFI_FIELDTYPE_FG && mEv_CheckFirstIntro() != TRUE && alpha >= 0.01f) {
        GRAPH* g = play->game.graph;
        int poly_render = FALSE;
        Mtx* m = (Mtx*)GRAPH_ALLOC_TYPE(g, Mtx, 1);

        /* TODO: submenu mode enums */
        if (play->submenu.process_status != mSM_PROCESS_WAIT && play->submenu.mode > 2) {
            poly_render = TRUE;
        }

        OPEN_DISP(g);

        if (m != NULL) {
            mFont_CulcOrthoMatrix(m);

            if (poly_render == TRUE) {
                gSPMatrix(NOW_POLY_OPA_DISP++, m, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
            } else {
                gSPMatrix(NOW_FONT_DISP++, m, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
            }
        }

        Matrix_scale(1.0f, 1.0f, 1.0f, 0);
        Matrix_translate(pos_x, pos_y, 0.0f, 1);

        {
            Gfx** gfx_pp;
            u8 a;
            Gfx* gfx;
            u32 rendermode0;
            u32 rendermode1;
            u8 tickets[9];

            if (poly_render == TRUE) {
                gfx = NOW_POLY_OPA_DISP;
            } else {
                gfx = NOW_FONT_DISP;
            }

            gfx_pp = &gfx;

            gSPMatrix(gfx_pp[0]++, _Matrix_to_Mtx_new(g), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            if (alpha >= 0.4f) {
                rendermode0 = G_RM_PASS;
                rendermode1 = G_RM_XLU_SURF2;
            } else {
                rendermode0 = G_RM_PASS;
                rendermode1 = G_RM_CLD_SURF2;
            }

            gSPDisplayList(gfx_pp[0]++, tickets_mode);
            gDPSetRenderMode(gfx_pp[0]++, rendermode0, rendermode1);
            a = alpha * 255.0f;

            gDPSetEnvColor(gfx_pp[0]++, 165, 255, 255, a);

            /* Draw outline */
            gDPSetPrimColor(gfx_pp[0]++, 0, a, 190, 220, 255, a);
            gSPDisplayList(gfx_pp[0]++, con_kaiwaname_modelT);

            /* Draw main */
            gDPSetPrimColor(gfx_pp[0]++, 0, a, 90, 120, 210, a);
            gSPDisplayList(gfx_pp[0]++, con_kaiwaname_model2T);

            if (poly_render == TRUE) {
                SET_POLY_OPA_DISP(*gfx_pp);
            } else {
                Matrix_scale(1.0f, 1.0f, 1.0f, 0);
                gSPMatrix(gfx_pp[0]++, _Matrix_to_Mtx_new(g), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

                SET_FONT_DISP(*gfx_pp);
            }

            /* Draw text */
            mFont_UnintToString(tickets, sizeof(tickets), Now_Private->town_tickets, sizeof(tickets) - 2, TRUE, FALSE,
                                TRUE);

            // clang-format off
            mFont_SetLineStrings(
                (GAME*)play,
                tickets, sizeof(tickets),
                20.0f, 20.0f,
                190, 220, 255, a,
                FALSE,
                TRUE,
                0.875f, 0.875f,
                poly_render ? mFont_MODE_POLY : mFont_MODE_FONT
            );
            // clang-format on
        }

        CLOSE_DISP(g);
    }
}
