#include "libforest/gbi_extensions.h"

extern u8 cliff_pal_dummy[];
extern u8 earth_pal_dummy[];
extern u8 water_2_tex_dummy[];
extern u8 water_1_tex_dummy[];
extern u8 river_tex_dummy[];
extern u8 grass_tex_dummy[];
extern u8 cliff_tex_dummy[];
extern u8 earth_tex_dummy[];

static Vtx grd_s_c1_r1_2_v[] = {
#include "assets/field/bg/grd_s_c1_r1_2_v.inc"
};

extern Gfx grd_s_c1_r1_2_modelT[] = {
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetCombineLERP(PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, 1, 0, TEXEL0, TEXEL1, COMBINED, 0, SHADE, TEXEL0,
                       COMBINED, 0, PRIM_LOD_FRAC, PRIMITIVE),
    gsDPSetPrimColor(0, 50, 255, 255, 255, 50),
    gsDPSetEnvColor(0x00, 0x3C, 0xFF, 0xFF),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_ZB_XLU_SURF2),
    gsDPLoadTextureBlock_4b_Dolphin(water_1_tex_dummy, G_IM_FMT_I, 32, 32, 15, GX_REPEAT, GX_REPEAT, 0, 0),
    gsDPLoadMultiBlock_4b_Dolphin(water_2_tex_dummy, 1, G_IM_FMT_I, 32, 32, 15, GX_REPEAT, GX_REPEAT, 0, 0),
    gsSPDisplayList(0x08000000),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&grd_s_c1_r1_2_v[254], 20, 0),
    gsSPNTrianglesInit_5b(16,      // tri count
                          0, 1, 2, // tri0
                          0, 3, 1, // tri1
                          3, 4, 1  // tri2
                          ),
    gsSPNTriangles_5b(4, 5, 1, // tri0
                      6, 7, 8, // tri1
                      6, 9, 7, // tri2
                      7, 10, 8 // tri3
                      ),
    gsSPNTriangles_5b(7, 11, 10, // tri0
                      11, 2, 10, // tri1
                      11, 0, 2,  // tri2
                      12, 13, 14 // tri3
                      ),
    gsSPNTriangles_5b(13, 15, 14, // tri0
                      13, 16, 15, // tri1
                      16, 17, 15, // tri2
                      16, 18, 17  // tri3
                      ),
    gsSPNTriangles_5b(16, 19, 18, // tri0
                      0, 0, 0,    // tri1
                      0, 0, 0,    // tri2
                      0, 0, 0     // tri3
                      ),
    gsSPEndDisplayList(),
};

extern Gfx grd_s_c1_r1_2_model[] = {
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTLUT_Dolphin(15, 16, 1, earth_pal_dummy),
    gsDPLoadTextureBlock_4b_Dolphin(grass_tex_dummy, G_IM_FMT_CI, 32, 32, 15, GX_REPEAT, GX_REPEAT, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&grd_s_c1_r1_2_v[0], 32, 0),
    gsSPNTrianglesInit_5b(24,      // tri count
                          0, 1, 2, // tri0
                          2, 3, 4, // tri1
                          0, 2, 4  // tri2
                          ),
    gsSPNTriangles_5b(4, 5, 0, // tri0
                      5, 6, 0, // tri1
                      6, 7, 0, // tri2
                      7, 8, 0  // tri3
                      ),
    gsSPNTriangles_5b(9, 10, 11,  // tri0
                      10, 12, 11, // tri1
                      13, 14, 15, // tri2
                      15, 14, 8   // tri3
                      ),
    gsSPNTriangles_5b(16, 17, 9, // tri0
                      17, 18, 9, // tri1
                      9, 11, 16, // tri2
                      19, 20, 21 // tri3
                      ),
    gsSPNTriangles_5b(22, 19, 23, // tri0
                      19, 21, 23, // tri1
                      21, 24, 23, // tri2
                      21, 25, 24  // tri3
                      ),
    gsSPNTriangles_5b(26, 27, 28, // tri0
                      26, 28, 25, // tri1
                      29, 27, 26, // tri2
                      29, 30, 27  // tri3
                      ),
    gsSPNTriangles_5b(30, 29, 31, // tri0
                      0, 0, 0,    // tri1
                      0, 0, 0,    // tri2
                      0, 0, 0     // tri3
                      ),
    gsSPVertex(&grd_s_c1_r1_2_v[32], 32, 0),
    gsSPNTrianglesInit_5b(18,      // tri count
                          0, 1, 2, // tri0
                          3, 4, 5, // tri1
                          6, 5, 4  // tri2
                          ),
    gsSPNTriangles_5b(4, 7, 6,  // tri0
                      8, 6, 7,  // tri1
                      8, 7, 9,  // tri2
                      10, 11, 9 // tri3
                      ),
    gsSPNTriangles_5b(11, 12, 9,  // tri0
                      13, 14, 15, // tri1
                      13, 16, 14, // tri2
                      17, 18, 19  // tri3
                      ),
    gsSPNTriangles_5b(20, 13, 21, // tri0
                      15, 22, 13, // tri1
                      23, 2, 24,  // tri2
                      1, 25, 2    // tri3
                      ),
    gsSPNTriangles_5b(1, 26, 25,  // tri0
                      4, 3, 26,   // tri1
                      27, 28, 29, // tri2
                      0, 0, 0     // tri3
                      ),
    gsSPVertex(&grd_s_c1_r1_2_v[62], 17, 0),
    gsSPNTrianglesInit_5b(12,      // tri count
                          0, 1, 2, // tri0
                          1, 3, 4, // tri1
                          3, 5, 4  // tri2
                          ),
    gsSPNTriangles_5b(3, 6, 5, // tri0
                      5, 7, 4, // tri1
                      1, 4, 2, // tri2
                      4, 8, 2  // tri3
                      ),
    gsSPNTriangles_5b(9, 10, 11,  // tri0
                      9, 8, 10,   // tri1
                      12, 13, 14, // tri2
                      13, 15, 14  // tri3
                      ),
    gsSPNTriangles_5b(15, 13, 16, // tri0
                      0, 0, 0,    // tri1
                      0, 0, 0,    // tri2
                      0, 0, 0     // tri3
                      ),
    gsDPLoadTextureBlock_4b_Dolphin(earth_tex_dummy, G_IM_FMT_CI, 64, 64, 15, GX_REPEAT, GX_CLAMP, 0, 0),
    gsSPVertex(&grd_s_c1_r1_2_v[79], 32, 0),
    gsSPNTrianglesInit_5b(17,      // tri count
                          0, 1, 2, // tri0
                          1, 3, 2, // tri1
                          4, 5, 6  // tri2
                          ),
    gsSPNTriangles_5b(5, 7, 8,   // tri0
                      7, 9, 10,  // tri1
                      9, 11, 12, // tri2
                      11, 13, 12 // tri3
                      ),
    gsSPNTriangles_5b(13, 14, 15, // tri0
                      14, 16, 15, // tri1
                      17, 18, 19, // tri2
                      20, 21, 18  // tri3
                      ),
    gsSPNTriangles_5b(22, 23, 21, // tri0
                      24, 25, 23, // tri1
                      24, 26, 25, // tri2
                      26, 27, 28  // tri3
                      ),
    gsSPNTriangles_5b(27, 2, 28, // tri0
                      3, 29, 30, // tri1
                      0, 0, 0,   // tri2
                      0, 0, 0    // tri3
                      ),
    gsSPVertex(&grd_s_c1_r1_2_v[110], 12, 0),
    gsSPNTrianglesInit_5b(7,       // tri count
                          0, 1, 2, // tri0
                          1, 3, 4, // tri1
                          3, 5, 4  // tri2
                          ),
    gsSPNTriangles_5b(5, 6, 7,  // tri0
                      6, 8, 7,  // tri1
                      8, 9, 10, // tri2
                      9, 11, 10 // tri3
                      ),
    gsDPLoadTLUT_Dolphin(15, 16, 1, cliff_pal_dummy),
    gsDPLoadTextureBlock_4b_Dolphin(river_tex_dummy, G_IM_FMT_CI, 64, 32, 15, GX_REPEAT, GX_CLAMP, 0, 0),
    gsSPVertex(&grd_s_c1_r1_2_v[122], 32, 0),
    gsSPNTrianglesInit_5b(26,      // tri count
                          0, 1, 2, // tri0
                          1, 3, 2, // tri1
                          1, 4, 3  // tri2
                          ),
    gsSPNTriangles_5b(1, 5, 4, // tri0
                      5, 6, 4, // tri1
                      5, 7, 6, // tri2
                      7, 8, 6  // tri3
                      ),
    gsSPNTriangles_5b(7, 9, 8,   // tri0
                      9, 10, 8,  // tri1
                      9, 11, 10, // tri2
                      9, 12, 11  // tri3
                      ),
    gsSPNTriangles_5b(12, 13, 11, // tri0
                      12, 14, 13, // tri1
                      14, 15, 13, // tri2
                      16, 17, 15  // tri3
                      ),
    gsSPNTriangles_5b(16, 18, 17, // tri0
                      19, 20, 18, // tri1
                      21, 22, 20, // tri2
                      21, 23, 22  // tri3
                      ),
    gsSPNTriangles_5b(21, 24, 23, // tri0
                      25, 26, 24, // tri1
                      27, 28, 26, // tri2
                      26, 25, 27  // tri3
                      ),
    gsSPNTriangles_5b(27, 29, 28, // tri0
                      30, 31, 29, // tri1
                      27, 30, 29, // tri2
                      0, 0, 0     // tri3
                      ),
    gsSPVertex(&grd_s_c1_r1_2_v[154], 22, 0),
    gsSPNTrianglesInit_5b(16,      // tri count
                          0, 1, 2, // tri0
                          1, 3, 2, // tri1
                          1, 4, 3  // tri2
                          ),
    gsSPNTriangles_5b(4, 5, 3, // tri0
                      4, 6, 5, // tri1
                      6, 7, 5, // tri2
                      6, 8, 7  // tri3
                      ),
    gsSPNTriangles_5b(6, 9, 8,    // tri0
                      10, 11, 12, // tri1
                      12, 13, 10, // tri2
                      13, 14, 10  // tri3
                      ),
    gsSPNTriangles_5b(13, 15, 14, // tri0
                      16, 17, 18, // tri1
                      19, 20, 16, // tri2
                      16, 20, 17  // tri3
                      ),
    gsSPNTriangles_5b(19, 21, 20, // tri0
                      0, 0, 0,    // tri1
                      0, 0, 0,    // tri2
                      0, 0, 0     // tri3
                      ),
    gsDPLoadTextureBlock_4b_Dolphin(cliff_tex_dummy, G_IM_FMT_CI, 64, 64, 15, GX_REPEAT, GX_REPEAT, 0, 0),
    gsSPVertex(&grd_s_c1_r1_2_v[176], 32, 0),
    gsSPNTrianglesInit_5b(25,      // tri count
                          0, 1, 2, // tri0
                          1, 3, 2, // tri1
                          4, 5, 6  // tri2
                          ),
    gsSPNTriangles_5b(4, 6, 7,  // tri0
                      6, 8, 9,  // tri1
                      6, 9, 7,  // tri2
                      8, 10, 11 // tri3
                      ),
    gsSPNTriangles_5b(8, 11, 9,  // tri0
                      4, 12, 5,  // tri1
                      4, 13, 14, // tri2
                      4, 14, 12  // tri3
                      ),
    gsSPNTriangles_5b(13, 15, 14, // tri0
                      13, 16, 17, // tri1
                      13, 17, 15, // tri2
                      18, 19, 20  // tri3
                      ),
    gsSPNTriangles_5b(19, 21, 20, // tri0
                      20, 22, 18, // tri1
                      22, 23, 18, // tri2
                      0, 24, 1    // tri3
                      ),
    gsSPNTriangles_5b(0, 25, 24,  // tri0
                      25, 26, 24, // tri1
                      27, 28, 29, // tri2
                      28, 30, 29  // tri3
                      ),
    gsSPNTriangles_5b(25, 21, 19, // tri0
                      25, 19, 26, // tri1
                      0, 0, 0,    // tri2
                      0, 0, 0     // tri3
                      ),
    gsSPVertex(&grd_s_c1_r1_2_v[207], 32, 0),
    gsSPNTrianglesInit_5b(25,      // tri count
                          0, 1, 2, // tri0
                          0, 2, 3, // tri1
                          2, 4, 5  // tri2
                          ),
    gsSPNTriangles_5b(2, 5, 3,   // tri0
                      6, 7, 8,   // tri1
                      9, 10, 11, // tri2
                      9, 12, 10  // tri3
                      ),
    gsSPNTriangles_5b(6, 13, 7,   // tri0
                      14, 15, 16, // tri1
                      14, 17, 15, // tri2
                      16, 18, 19  // tri3
                      ),
    gsSPNTriangles_5b(16, 15, 18, // tri0
                      19, 18, 20, // tri1
                      18, 21, 20, // tri2
                      20, 22, 23  // tri3
                      ),
    gsSPNTriangles_5b(20, 21, 22, // tri0
                      23, 22, 6,  // tri1
                      22, 24, 6,  // tri2
                      6, 25, 26   // tri3
                      ),
    gsSPNTriangles_5b(6, 24, 25,  // tri0
                      25, 27, 28, // tri1
                      25, 29, 27, // tri2
                      29, 12, 27  // tri3
                      ),
    gsSPNTriangles_5b(12, 29, 10, // tri0
                      29, 30, 10, // tri1
                      0, 0, 0,    // tri2
                      0, 0, 0     // tri3
                      ),
    gsSPVertex(&grd_s_c1_r1_2_v[238], 16, 0),
    gsSPNTrianglesInit_5b(12,      // tri count
                          0, 1, 2, // tri0
                          0, 3, 1, // tri1
                          4, 5, 6  // tri2
                          ),
    gsSPNTriangles_5b(4, 2, 5, // tri0
                      2, 7, 5, // tri1
                      2, 1, 7, // tri2
                      5, 7, 8  // tri3
                      ),
    gsSPNTriangles_5b(7, 9, 8,   // tri0
                      8, 9, 10,  // tri1
                      9, 11, 10, // tri2
                      12, 13, 14 // tri3
                      ),
    gsSPNTriangles_5b(12, 15, 13, // tri0
                      0, 0, 0,    // tri1
                      0, 0, 0,    // tri2
                      0, 0, 0     // tri3
                      ),
    gsSPEndDisplayList(),
};
