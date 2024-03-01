#include "ac_design.h"

#include "m_name_table.h"
#include "m_common_data.h"

static void Design_Actor_ct(ACTOR* actorx, GAME* game);
static void Design_Actor_dt(ACTOR* actorx, GAME* game);
static void Design_Actor_move(ACTOR* actorx, GAME* game);
static void Design_Actor_draw(ACTOR* actorx, GAME* game);

ACTOR_PROFILE Design_Profile = {
    mAc_PROFILE_DESIGN,
    ACTOR_PART_BG,
    ACTOR_STATE_CAN_MOVE_IN_DEMO_SCENES | ACTOR_STATE_NO_DRAW_WHILE_CULLED | ACTOR_STATE_NO_MOVE_WHILE_CULLED,
    DESIGN00_PLR0,
    ACTOR_OBJ_BANK_KEEP,
    sizeof(DESIGN_ACTOR),
    &Design_Actor_ct,
    &Design_Actor_dt,
    &Design_Actor_move,
    &Design_Actor_draw,
    NULL,
};

static void Design_Actor_ct(ACTOR* actorx, GAME* game) {
    DESIGN_ACTOR* design = (DESIGN_ACTOR*)actorx;

    actorx->gravity = 0.0f;
    design->pl_no = ((actorx->npc_id - DESIGN00_PLR0) >> 3) & 3;
    design->design_no = actorx->npc_id & 7;
    actorx->world.position.y = mCoBG_GetBgY_OnlyCenter_FromWpos(actorx->world.position, 0.0f) + 1.0f;
}

static void Design_Actor_dt(ACTOR* actorx, GAME* game) {
    // nothing
}

static void Design_Actor_move(ACTOR* actorx, GAME* game) {
    DESIGN_ACTOR* design = (DESIGN_ACTOR*)actorx;
    // nothing for now
}

const static Vtx aDS_design_v[] = {
    { -1200, 0, 1200, 0, 0, 0, 0, 0, 120, 255 },     // bl
    { -1200, 0, -1200, 0, 0, 992, 0, 0, 120, 255 },  // tl
    { 1200, 0, -1200, 0, 992, 992, 0, 0, 120, 255 }, // tr
    { 1200, 0, 1200, 0, 992, 0, 0, 0, 120, 255 },    // br
};

const static Gfx aDS_design_model[] = {
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
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSPEndDisplayList(),
};

static void Design_Actor_draw(ACTOR* actorx, GAME* game) {
    DESIGN_ACTOR* design = (DESIGN_ACTOR*)actorx;
    const mNW_original_design_c* const org_design_p = &Save_Get(private[design->pl_no]).my_org[design->design_no];
    const u8* tex_p = org_design_p->design.data;
    const u16* pal_p = mNW_PaletteIdx2Palette(org_design_p->palette);

    OPEN_DISP(game->graph);

    Matrix_translate(actorx->world.position.x, actorx->world.position.y, actorx->world.position.z, 0);
    Matrix_scale(0.01f, 0.01f, 0.01f, 1);

    /* Segment 8 holds the palette */
    gSPSegment(NEXT_POLY_OPA_DISP, G_MWO_SEGMENT_8, pal_p);

    /* Segment 9 holds the texture */
    gSPSegment(NEXT_POLY_OPA_DISP, G_MWO_SEGMENT_9, tex_p);

    gSPMatrix(NEXT_POLY_OPA_DISP, _Matrix_to_Mtx_new(game->graph), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(NEXT_POLY_OPA_DISP, aDS_design_model);

    CLOSE_DISP(game->graph);
}
