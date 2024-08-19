#include "libforest/gbi_extensions.h"

extern u8 bush_pal_dummy[];
extern u8 cliff_pal_dummy[];
extern u8 earth_pal_dummy[];
extern u8 water_2_tex_dummy[];
extern u8 water_1_tex_dummy[];
extern u8 river_tex_dummy[];
extern u8 grass_tex_dummy[];
extern u8 bush_b_tex_dummy[];
extern u8 bush_a_tex_dummy[];
extern u8 cliff_tex_dummy[];
extern u8 earth_tex_dummy[];

static Vtx grd_s_c3_r2_2_v[] = {
#include "assets/field/bg/grd_s_c3_r2_2_v.inc"
};

extern Gfx grd_s_c3_r2_2_modelT[] = {
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetCombineLERP(PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, 1, 0, TEXEL0, TEXEL1, COMBINED, 0, SHADE, TEXEL0,
                       COMBINED, 0, PRIM_LOD_FRAC, PRIMITIVE),
    gsDPSetPrimColor(0, 50, 255, 255, 255, 50),
    gsDPSetEnvColor(0x00, 0x64, 0xFF, 0xFF),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_ZB_XLU_SURF2),
    gsDPLoadTextureBlock_4b_Dolphin(water_1_tex_dummy, G_IM_FMT_I, 32, 32, 15, GX_REPEAT, GX_REPEAT, 0, 0),
    gsDPLoadMultiBlock_4b_Dolphin(water_2_tex_dummy, 1, G_IM_FMT_I, 32, 32, 15, GX_REPEAT, GX_REPEAT, 0, 0),
    gsSPDisplayList(0x08000000),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&grd_s_c3_r2_2_v[280], 25, 0),
    gsSPNTrianglesInit_5b(21,      // tri count
                          0, 1, 2, // tri0
                          1, 3, 2, // tri1
                          4, 0, 5  // tri2
                          ),
    gsSPNTriangles_5b(0, 2, 5, // tri0
                      3, 6, 7, // tri1
                      3, 1, 6, // tri2
                      7, 8, 9  // tri3
                      ),
    gsSPNTriangles_5b(7, 6, 8,    // tri0
                      10, 11, 12, // tri1
                      10, 12, 13, // tri2
                      14, 15, 11  // tri3
                      ),
    gsSPNTriangles_5b(14, 11, 10, // tri0
                      14, 16, 17, // tri1
                      14, 17, 15, // tri2
                      16, 18, 17  // tri3
                      ),
    gsSPNTriangles_5b(16, 19, 18, // tri0
                      19, 20, 18, // tri1
                      19, 21, 20, // tri2
                      19, 22, 21  // tri3
                      ),
    gsSPNTriangles_5b(19, 23, 22, // tri0
                      19, 24, 23, // tri1
                      0, 0, 0,    // tri2
                      0, 0, 0     // tri3
                      ),
    gsSPEndDisplayList(),
};

extern Gfx grd_s_c3_r2_2_model[] = {
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTLUT_Dolphin(15, 16, 1, bush_pal_dummy),
    gsDPLoadTextureBlock_4b_Dolphin(bush_a_tex_dummy, G_IM_FMT_CI, 64, 64, 15, GX_REPEAT, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&grd_s_c3_r2_2_v[269], 11, 0),
    gsSPNTrianglesInit_5b(5,       // tri count
                          0, 1, 2, // tri0
                          3, 0, 4, // tri1
                          5, 3, 6  // tri2
                          ),
    gsSPNTriangles_5b(7, 5, 8,  // tri0
                      9, 7, 10, // tri1
                      0, 0, 0,  // tri2
                      0, 0, 0   // tri3
                      ),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPLoadTLUT_Dolphin(15, 16, 1, earth_pal_dummy),
    gsDPLoadTextureBlock_4b_Dolphin(grass_tex_dummy, G_IM_FMT_CI, 32, 32, 15, GX_REPEAT, GX_REPEAT, 0, 0),
    gsSPVertex(&grd_s_c3_r2_2_v[0], 32, 0),
    gsSPNTrianglesInit_5b(19,      // tri count
                          0, 1, 2, // tri0
                          1, 3, 2, // tri1
                          4, 5, 6  // tri2
                          ),
    gsSPNTriangles_5b(5, 7, 6, // tri0
                      5, 8, 7, // tri1
                      2, 9, 0, // tri2
                      10, 2, 3 // tri3
                      ),
    gsSPNTriangles_5b(9, 2, 11,   // tri0
                      11, 2, 4,   // tri1
                      11, 12, 13, // tri2
                      4, 12, 11   // tri3
                      ),
    gsSPNTriangles_5b(14, 15, 16, // tri0
                      15, 17, 16, // tri1
                      18, 7, 8,   // tri2
                      19, 20, 21  // tri3
                      ),
    gsSPNTriangles_5b(22, 23, 24, // tri0
                      23, 25, 24, // tri1
                      23, 26, 25, // tri2
                      27, 28, 29  // tri3
                      ),
    gsSPVertex(&grd_s_c3_r2_2_v[30], 32, 0),
    gsSPNTrianglesInit_5b(17,      // tri count
                          0, 1, 2, // tri0
                          2, 3, 0, // tri1
                          4, 5, 6  // tri2
                          ),
    gsSPNTriangles_5b(4, 7, 5, // tri0
                      7, 8, 5, // tri1
                      7, 9, 8, // tri2
                      9, 7, 10 // tri3
                      ),
    gsSPNTriangles_5b(11, 10, 7,  // tri0
                      7, 3, 11,   // tri1
                      12, 13, 14, // tri2
                      14, 15, 16  // tri3
                      ),
    gsSPNTriangles_5b(17, 18, 19, // tri0
                      18, 17, 20, // tri1
                      21, 22, 23, // tri2
                      24, 25, 26  // tri3
                      ),
    gsSPNTriangles_5b(27, 25, 24, // tri0
                      28, 29, 26, // tri1
                      0, 0, 0,    // tri2
                      0, 0, 0     // tri3
                      ),
    gsSPVertex(&grd_s_c3_r2_2_v[60], 29, 0),
    gsSPNTrianglesInit_5b(17,      // tri count
                          0, 1, 2, // tri0
                          3, 4, 5, // tri1
                          6, 7, 8  // tri2
                          ),
    gsSPNTriangles_5b(0, 2, 7,   // tri0
                      9, 10, 11, // tri1
                      9, 12, 10, // tri2
                      13, 10, 14 // tri3
                      ),
    gsSPNTriangles_5b(11, 15, 16, // tri0
                      17, 18, 19, // tri1
                      17, 20, 18, // tri2
                      20, 21, 20  // tri3
                      ),
    gsSPNTriangles_5b(21, 22, 20, // tri0
                      21, 23, 22, // tri1
                      23, 24, 25, // tri2
                      14, 10, 12  // tri3
                      ),
    gsSPNTriangles_5b(26, 27, 28, // tri0
                      27, 20, 28, // tri1
                      0, 0, 0,    // tri2
                      0, 0, 0     // tri3
                      ),
    gsDPLoadTLUT_Dolphin(15, 16, 1, cliff_pal_dummy),
    gsDPLoadTextureBlock_4b_Dolphin(cliff_tex_dummy, G_IM_FMT_CI, 64, 64, 15, GX_REPEAT, GX_REPEAT, 0, 0),
    gsSPVertex(&grd_s_c3_r2_2_v[89], 32, 0),
    gsSPNTrianglesInit_5b(23,      // tri count
                          0, 1, 2, // tri0
                          1, 3, 2, // tri1
                          4, 5, 6  // tri2
                          ),
    gsSPNTriangles_5b(4, 7, 5, // tri0
                      4, 1, 7, // tri1
                      1, 0, 7, // tri2
                      8, 9, 10 // tri3
                      ),
    gsSPNTriangles_5b(9, 11, 10,  // tri0
                      8, 12, 9,   // tri1
                      13, 14, 15, // tri2
                      13, 16, 14  // tri3
                      ),
    gsSPNTriangles_5b(15, 17, 13, // tri0
                      15, 18, 17, // tri1
                      19, 20, 21, // tri2
                      20, 22, 21  // tri3
                      ),
    gsSPNTriangles_5b(21, 13, 17, // tri0
                      21, 22, 13, // tri1
                      11, 23, 24, // tri2
                      11, 25, 23  // tri3
                      ),
    gsSPNTriangles_5b(26, 27, 28, // tri0
                      26, 29, 27, // tri1
                      27, 30, 28, // tri2
                      30, 31, 28  // tri3
                      ),
    gsSPVertex(&grd_s_c3_r2_2_v[121], 32, 0),
    gsSPNTrianglesInit_5b(25,      // tri count
                          0, 1, 2, // tri0
                          0, 3, 1, // tri1
                          1, 4, 2  // tri2
                          ),
    gsSPNTriangles_5b(4, 5, 2, // tri0
                      6, 7, 1, // tri1
                      7, 4, 1, // tri2
                      8, 9, 6  // tri3
                      ),
    gsSPNTriangles_5b(9, 7, 6,    // tri0
                      10, 11, 12, // tri1
                      11, 13, 12, // tri2
                      10, 12, 8   // tri3
                      ),
    gsSPNTriangles_5b(14, 15, 16, // tri0
                      15, 17, 16, // tri1
                      18, 19, 20, // tri2
                      19, 21, 20  // tri3
                      ),
    gsSPNTriangles_5b(21, 22, 20, // tri0
                      19, 23, 21, // tri1
                      23, 24, 21, // tri2
                      23, 25, 24  // tri3
                      ),
    gsSPNTriangles_5b(25, 26, 24, // tri0
                      25, 27, 26, // tri1
                      27, 28, 26, // tri2
                      28, 29, 26  // tri3
                      ),
    gsSPNTriangles_5b(29, 30, 26, // tri0
                      29, 31, 30, // tri1
                      0, 0, 0,    // tri2
                      0, 0, 0     // tri3
                      ),
    gsSPVertex(&grd_s_c3_r2_2_v[153], 6, 0),
    gsSPNTrianglesInit_5b(4,       // tri count
                          0, 1, 2, // tri0
                          1, 3, 2, // tri1
                          3, 4, 2  // tri2
                          ),
    gsSPNTriangles_5b(3, 5, 4, // tri0
                      0, 0, 0, // tri1
                      0, 0, 0, // tri2
                      0, 0, 0  // tri3
                      ),
    gsDPLoadTextureBlock_4b_Dolphin(river_tex_dummy, G_IM_FMT_CI, 64, 32, 15, GX_REPEAT, GX_CLAMP, 0, 0),
    gsSPVertex(&grd_s_c3_r2_2_v[159], 32, 0),
    gsSPNTrianglesInit_5b(24,      // tri count
                          0, 1, 2, // tri0
                          0, 3, 1, // tri1
                          3, 4, 1  // tri2
                          ),
    gsSPNTriangles_5b(5, 6, 7,   // tri0
                      6, 8, 7,   // tri1
                      9, 10, 11, // tri2
                      10, 12, 11 // tri3
                      ),
    gsSPNTriangles_5b(5, 13, 6,  // tri0
                      5, 9, 13,  // tri1
                      9, 14, 13, // tri2
                      9, 11, 14  // tri3
                      ),
    gsSPNTriangles_5b(15, 16, 17, // tri0
                      15, 18, 16, // tri1
                      18, 3, 16,  // tri2
                      18, 4, 3    // tri3
                      ),
    gsSPNTriangles_5b(19, 20, 21, // tri0
                      22, 23, 24, // tri1
                      23, 25, 24, // tri2
                      25, 26, 24  // tri3
                      ),
    gsSPNTriangles_5b(25, 27, 26, // tri0
                      27, 28, 26, // tri1
                      27, 29, 28, // tri2
                      27, 30, 29  // tri3
                      ),
    gsSPNTriangles_5b(30, 31, 29, // tri0
                      0, 0, 0,    // tri1
                      0, 0, 0,    // tri2
                      0, 0, 0     // tri3
                      ),
    gsSPVertex(&grd_s_c3_r2_2_v[191], 31, 0),
    gsSPNTrianglesInit_5b(24,      // tri count
                          0, 1, 2, // tri0
                          1, 3, 2, // tri1
                          1, 4, 3  // tri2
                          ),
    gsSPNTriangles_5b(4, 5, 3, // tri0
                      4, 6, 5, // tri1
                      4, 7, 6, // tri2
                      4, 8, 7  // tri3
                      ),
    gsSPNTriangles_5b(9, 10, 8,   // tri0
                      11, 12, 10, // tri1
                      13, 14, 12, // tri2
                      15, 16, 14  // tri3
                      ),
    gsSPNTriangles_5b(15, 17, 18, // tri0
                      17, 19, 18, // tri1
                      17, 20, 19, // tri2
                      17, 21, 20  // tri3
                      ),
    gsSPNTriangles_5b(21, 22, 20, // tri0
                      21, 23, 22, // tri1
                      21, 24, 23, // tri2
                      24, 25, 23  // tri3
                      ),
    gsSPNTriangles_5b(24, 26, 25, // tri0
                      24, 27, 26, // tri1
                      27, 28, 26, // tri2
                      27, 29, 28  // tri3
                      ),
    gsSPNTriangles_5b(29, 30, 28, // tri0
                      0, 0, 0,    // tri1
                      0, 0, 0,    // tri2
                      0, 0, 0     // tri3
                      ),
    gsDPLoadTLUT_Dolphin(15, 16, 1, earth_pal_dummy),
    gsDPLoadTextureBlock_4b_Dolphin(earth_tex_dummy, G_IM_FMT_CI, 64, 64, 15, GX_REPEAT, GX_CLAMP, 0, 0),
    gsSPVertex(&grd_s_c3_r2_2_v[222], 32, 0),
    gsSPNTrianglesInit_5b(17,      // tri count
                          0, 1, 2, // tri0
                          1, 3, 2, // tri1
                          4, 3, 5  // tri2
                          ),
    gsSPNTriangles_5b(4, 6, 3,    // tri0
                      7, 8, 9,    // tri1
                      10, 11, 12, // tri2
                      11, 13, 12  // tri3
                      ),
    gsSPNTriangles_5b(13, 14, 12, // tri0
                      2, 10, 15,  // tri1
                      16, 6, 17,  // tri2
                      18, 19, 20  // tri3
                      ),
    gsSPNTriangles_5b(21, 7, 22,  // tri0
                      21, 22, 19, // tri1
                      14, 18, 20, // tri2
                      23, 24, 25  // tri3
                      ),
    gsSPNTriangles_5b(26, 27, 28, // tri0
                      29, 26, 30, // tri1
                      0, 0, 0,    // tri2
                      0, 0, 0     // tri3
                      ),
    gsSPVertex(&grd_s_c3_r2_2_v[253], 5, 0),
    gsSPNTrianglesInit_5b(2,       // tri count
                          0, 1, 2, // tri0
                          1, 3, 4, // tri1
                          0, 0, 0  // tri2
                          ),
    gsDPLoadTLUT_Dolphin(15, 16, 1, bush_pal_dummy),
    gsDPLoadTextureBlock_4b_Dolphin(bush_b_tex_dummy, G_IM_FMT_CI, 64, 32, 15, GX_REPEAT, GX_CLAMP, 0, 0),
    gsSPVertex(&grd_s_c3_r2_2_v[258], 11, 0),
    gsSPNTrianglesInit_5b(5,       // tri count
                          0, 1, 2, // tri0
                          1, 3, 4, // tri1
                          5, 6, 7  // tri2
                          ),
    gsSPNTriangles_5b(6, 0, 8,  // tri0
                      9, 5, 10, // tri1
                      0, 0, 0,  // tri2
                      0, 0, 0   // tri3
                      ),
    gsSPEndDisplayList(),
};