#include "ac_design.h"

#include "m_name_table.h"
#include "m_common_data.h"
#include "dolphin/os.h"

static void Design_Actor_ct(ACTOR* actorx, GAME* game);
static void Design_Actor_dt(ACTOR* actorx, GAME* game);
static void Design_Actor_move(ACTOR* actorx, GAME* game);
static void Design_Actor_draw(ACTOR* actorx, GAME* game);

ACTOR_PROFILE Design_Profile = {
    mAc_PROFILE_DESIGN,
    ACTOR_PART_CONTROL,
    ACTOR_STATE_CAN_MOVE_IN_DEMO_SCENES | ACTOR_STATE_NO_DRAW_WHILE_CULLED | ACTOR_STATE_NO_MOVE_WHILE_CULLED,
    EMPTY_NO,
    ACTOR_OBJ_BANK_KEEP,
    sizeof(DESIGN_ACTOR),
    &Design_Actor_ct,
    &Design_Actor_dt,
    &Design_Actor_move,
    &Design_Actor_draw,
    NULL,
};

static void aDS_UpdatePatternInfo(DESIGN_ACTOR* design_actor);

static void Design_Actor_ct(ACTOR* actorx, GAME* game) {
    DESIGN_ACTOR* design = (DESIGN_ACTOR*)actorx;
    int i;

    actorx->gravity = 0.0f;

    for (i = 0; i < UT_TOTAL_NUM; i++) {
        design->designs[i].player_no = -1;
    }

    aDS_UpdatePatternInfo(design);
}

static void Design_Actor_dt(ACTOR* actorx, GAME* game) {
    // nothing
}

static void aDS_UpdatePatternInfo(DESIGN_ACTOR* design_actor) {
    const mActor_name_t* fg_p = mFI_BkNumtoUtFGTop(design_actor->player_block.x, design_actor->player_block.z);

    if (fg_p != NULL) {
        xyz_t center_pos;
        aDS_design_c* design_p = design_actor->designs;
        int i;

        for (i = 0; i < UT_TOTAL_NUM; i++, fg_p++, design_p++) {
            const mActor_name_t item = *fg_p;

            if (item >= DESIGN00_PLR0 && item <= DESIGN07_PLR3) {
                const int bx = design_actor->player_block.x;
                const int bz = design_actor->player_block.z;
                const int ut_x = i & 15;
                const int ut_z = i >> 4;

                /* Item is custom design on the ground */
                design_p->player_no = (item - DESIGN00_PLR0) >> 3;
                design_p->design_no = item & 7;

                /* Get the center position for the first unit in the block */
                // TODO: this is kinda bad, would prefer to just increment X & Z
                mFI_BkandUtNum2CenterWpos(&center_pos, bx, bz, ut_x, ut_z);
                design_p->bg_y = (u16)mCoBG_GetBgY_OnlyCenter_FromWpos(center_pos, 0.0f);

                OSReport("Pattern: X: %d, Z: %d, - pl_no: %d, design_no: %d, bg_y: %.02f\n", ut_x, ut_z,
                         design_p->player_no, design_p->design_no, design_p->bg_y);

            } else {
                design_p->player_no = -1; // no design here
            }
        }
    }
}

static void Design_Actor_move(ACTOR* actorx, GAME* game) {
    DESIGN_ACTOR* design = (DESIGN_ACTOR*)actorx;
    int player_bx;
    int player_bz;

    if (mFI_GetNextBlockNum(&player_bx, &player_bz) == TRUE &&
        (design->player_block.x != player_bx || design->player_block.z != player_bz)) {
        design->player_block.x = player_bx;
        design->player_block.z = player_bz;

        OSReport("Updating patterns due to new acre\n");

        /* Update all patterns */
        aDS_UpdatePatternInfo(design);
    }
}

static Vtx aDS_design_v[] = {
    { -2000, 0, 2000, 0, 0, 0, 0, 0, 120, 255 },       // bl
    { -2000, 0, -2000, 0, 0, 1000, 0, 0, 120, 255 },   // tl
    { 2000, 0, -2000, 0, 1000, 1000, 0, 0, 120, 255 }, // tr
    { 2000, 0, 2000, 0, 1000, 0, 0, 0, 120, 255 },     // br
};

static Gfx aDS_design_model[] = {
    gsSPTexture(65535, 65535, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPSetTextureLUT(G_TT_RGBA16),
    /* Segment 8 holds the RGB5A3 palette */
    gsDPLoadTLUT_Dolphin(15, 16, 1, 0x08000000),
    /* Segment 9 holds the CI4 image */
    gsDPLoadTextureBlock_4b_Dolphin(0x09000000, G_IM_FMT_CI, 32, 32, 15, GX_MIRROR, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&aDS_design_v[0], 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 0, 3, 2, 0),
    gsSPEndDisplayList(),
};

static void Design_Actor_draw(ACTOR* actorx, GAME* game) {
    DESIGN_ACTOR* design_actor = (DESIGN_ACTOR*)actorx;
    f32 block_pos_x;
    f32 block_pos_z;
    aDS_design_c* design = design_actor->designs;
    f32 ofs_z = mFI_UT_WORLDSIZE_HALF_Z_F;
    f32 ofs_x = mFI_UT_WORLDSIZE_HALF_X_F;
    int z;
    int x;

    mFI_BkNum2WposXZ(&block_pos_x, &block_pos_z, design_actor->player_block.x, design_actor->player_block.z);
    for (z = 0; z < UT_Z_NUM; z++) {
        ofs_x = mFI_UT_WORLDSIZE_HALF_X_F;

        for (x = 0; x < UT_X_NUM; x++) {
            if (design->player_no != -1) {
                const mNW_original_design_c* const org_design_p =
                    &Save_Get(private[design->player_no]).my_org[design->design_no];
                const u8* const tex_p = org_design_p->design.data;
                const u16* const pal_p = mNW_PaletteIdx2Palette(org_design_p->palette);
                const f32 bg_y = (f32)design->bg_y;

                OPEN_DISP(game->graph);

                Matrix_translate(block_pos_x + ofs_x, bg_y + 0.5f, block_pos_z + ofs_z, 0);
                Matrix_scale(0.01f, 0.01f, 0.01f, 1);

                /* Segment 8 holds the palette */
                gSPSegment(NEXT_POLY_OPA_DISP, G_MWO_SEGMENT_8, pal_p);

                /* Segment 9 holds the texture */
                gSPSegment(NEXT_POLY_OPA_DISP, G_MWO_SEGMENT_9, tex_p);

                gSPMatrix(NEXT_POLY_OPA_DISP, _Matrix_to_Mtx_new(game->graph),
                          G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPDisplayList(NEXT_POLY_OPA_DISP, aDS_design_model);

                CLOSE_DISP(game->graph);
            }

            ofs_x += mFI_UT_WORLDSIZE_X_F;
            design++;
        }

        ofs_z += mFI_UT_WORLDSIZE_Z_F;
    }
}
