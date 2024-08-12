#include "libforest/gbi_extensions.h"

extern u8 wave3_tex_dummy[];
extern u8 wave1_tex_dummy[];
extern u8 beach2_tex_dummy2[];

static Vtx grd_s_o_5_v[] = {
#include "assets/field/bg/grd_s_o_5_v.inc"
};

extern Gfx grd_s_o_5_model[] = {
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetCombineLERP(PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, 0, 0, 0, 0, COMBINED, 0, SHADE, 0, 0, 0, 0, COMBINED),
    gsDPSetPrimColor(0, 255, 32, 48, 144, 255),
    gsSPDisplayList(0x0C000000),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPLoadTextureBlock_4b_Dolphin(beach2_tex_dummy2, G_IM_FMT_I, 32, 16, 15, GX_REPEAT, GX_CLAMP, 0, 0),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&grd_s_o_5_v[0], 15, 0),
    gsSPNTrianglesInit_5b(
        16, // tri count
        0, 1, 2, // tri0
        1, 3, 2, // tri1
        0, 4, 5 // tri2
    ),
    gsSPNTriangles_5b(
        4, 6, 5, // tri0
        7, 4, 8, // tri1
        4, 9, 8, // tri2
        7, 10, 11 // tri3
    ),
    gsSPNTriangles_5b(
        10, 12, 11, // tri0
        1, 5, 13, // tri1
        1, 0, 5, // tri2
        4, 2, 9 // tri3
    ),
    gsSPNTriangles_5b(
        4, 0, 2, // tri0
        4, 11, 6, // tri1
        4, 7, 11, // tri2
        10, 8, 14 // tri3
    ),
    gsSPNTriangles_5b(
        10, 7, 8, // tri0
        0, 0, 0, // tri1
        0, 0, 0, // tri2
        0, 0, 0 // tri3
    ),
    gsSPEndDisplayList(),
};

extern Gfx grd_s_o_5_modelT[] = {
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetCombineLERP(TEXEL1, 0, TEXEL0, TEXEL0, TEXEL1, 0, TEXEL0, 0, PRIMITIVE, 0, SHADE, COMBINED, 0, 0, 0, COMBINED),
    gsDPSetPrimColor(0, 255, 60, 120, 255, 255),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_XLU_SURF2),
    gsDPSetTextureImage_Dolphin(G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, wave1_tex_dummy),
    gsDPSetTile_Dolphin(G_DOLPHIN_TLUT_DEFAULT_MODE, 0, 0, GX_REPEAT, GX_REPEAT, 0, 0),
    gsDPSetTextureImage_Dolphin(G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, wave3_tex_dummy),
    gsDPSetTile_Dolphin(G_DOLPHIN_TLUT_DEFAULT_MODE, 1, 0, GX_REPEAT, GX_REPEAT, 0, 0),
    gsSPDisplayList(0x0D000000),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&grd_s_o_5_v[15], 32, 0),
    gsSPNTrianglesInit_5b(
        40, // tri count
        0, 1, 2, // tri0
        1, 3, 2, // tri1
        1, 4, 3 // tri2
    ),
    gsSPNTriangles_5b(
        4, 5, 3, // tri0
        4, 6, 5, // tri1
        5, 2, 3, // tri2
        5, 7, 2 // tri3
    ),
    gsSPNTriangles_5b(
        8, 9, 10, // tri0
        8, 11, 9, // tri1
        11, 6, 9, // tri2
        6, 12, 9 // tri3
    ),
    gsSPNTriangles_5b(
        12, 13, 9, // tri0
        13, 10, 9, // tri1
        13, 14, 10, // tri2
        12, 15, 13 // tri3
    ),
    gsSPNTriangles_5b(
        15, 16, 13, // tri0
        16, 17, 13, // tri1
        18, 11, 8, // tri2
        18, 19, 11 // tri3
    ),
    gsSPNTriangles_5b(
        19, 6, 11, // tri0
        12, 20, 15, // tri1
        20, 16, 15, // tri2
        20, 21, 16 // tri3
    ),
    gsSPNTriangles_5b(
        12, 22, 20, // tri0
        12, 23, 22, // tri1
        22, 21, 20, // tri2
        22, 24, 21 // tri3
    ),
    gsSPNTriangles_5b(
        23, 25, 22, // tri0
        25, 26, 22, // tri1
        26, 24, 22, // tri2
        7, 5, 18 // tri3
    ),
    gsSPNTriangles_5b(
        5, 19, 18, // tri0
        5, 6, 19, // tri1
        27, 1, 0, // tri2
        27, 28, 1 // tri3
    ),
    gsSPNTriangles_5b(
        28, 4, 1, // tri0
        29, 30, 27, // tri1
        30, 28, 27, // tri2
        30, 4, 28 // tri3
    ),
    gsSPNTriangles_5b(
        30, 31, 4, // tri0
        0, 0, 0, // tri1
        0, 0, 0, // tri2
        0, 0, 0 // tri3
    ),
    gsSPVertex(&grd_s_o_5_v[47], 5, 0),
    gsSPNTrianglesInit_5b(
        3, // tri count
        0, 1, 2, // tri0
        0, 3, 1, // tri1
        3, 4, 1 // tri2
    ),
    gsSPEndDisplayList(),
};
