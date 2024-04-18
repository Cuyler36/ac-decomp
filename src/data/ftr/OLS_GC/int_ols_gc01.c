#include "libforest/gbi_extensions.h"

#include "../src/data/ftr/OLS_GC/int_ols_gc01_tex.c"

// clang-format off
static Vtx int_ols_gc01_v[] = {
    {  -1490,      0,  -1600, 0,  1024,   512,  -38,  -38, -115,  50 }, // cubeback
    {  -1490,   2240,  -1600, 0,  1024,     0,  -38,  -38, -115, 255 }, // cubeback
    {   1490,      0,  -1600, 0,     0,   512,   38,  -38, -115,  50 }, // cubeback
    {   1490,   2240,  -1600, 0,     0,     0,   38,  -38, -115, 255 }, // cubeback

    {  -1490,      0,   1600, 0,     0,   512,  -73,   73,   73,  50 }, // cubefront
    {  -1490,   2240,   1600, 0,     0,     0,  -73,   73,   73, 255 }, // cubefront
    {   1490,      0,   1600, 0,  1024,   512,   73,   73,   73,  50 }, // cubefront
    {   1490,   2240,   1600, 0,  1024,     0,   73,   73,   73, 255 }, // cubefront

    {      0,   1800,  -2320, 0,   256,    64,   47,   14, -117, 200 }, // cubehandle
    {   -792,   1800,  -2092, 0,   384,    64,  -82,   12,  -96, 200 }, // cubehandle
    {  -1120,   1800,  -1600, 0,   512,    64,  -99,    3,  -80, 200 }, // cubehandle
    {  -1120,   1400,  -1600, 0,   512,   128,  -95,  -36,  -76, 160 }, // cubehandle
    {   -792,   1400,  -2092, 0,   384,   128,  -78,  -36,  -93, 160 }, // cubehandle
    {      0,   1400,  -2320, 0,   256,   128,   38,  -38, -115, 160 }, // cubehandle
    {   1120,   1800,  -1600, 0,     0,    64,   26,    4, -124, 200 }, // cubehandle
    {    792,   1800,  -2092, 0,   128,    64,    5,   16, -126, 200 }, // cubehandle
    {   1120,   1400,  -1600, 0,     0,   128,   34,  -52, -110, 160 }, // cubehandle
    {    792,   1400,  -2092, 0,   128,   128,    9,  -45, -118, 160 }, // cubehandle
    {      0,   1800,  -2141, 0,   256,     0,   73,   73,  -73, 200 }, // cubehandle
    {   -665,   1800,  -1965, 0,   384,     0,  -73,   73,  -73, 200 }, // cubehandle
    {   -941,   1800,  -1600, 0,   512,     0,  -73,   73,  -73, 200 }, // cubehandle
    {    941,   1800,  -1600, 0,     0,     0,   73,   73,  -73, 200 }, // cubehandle
    {    665,   1800,  -1965, 0,   128,     0,   73,   73,  -73, 200 }, // cubehandle

    {  -1490,      0,   1600, 0,  1024,   512, -115,  -38,   38,  50 }, // cubeside
    {  -1490,   2240,   1600, 0,  1024,     0, -115,  -38,   38, 255 }, // cubeside
    {  -1490,      0,  -1600, 0,     0,   512, -115,  -38,  -38,  50 }, // cubeside
    {  -1490,   2240,  -1600, 0,     0,     0, -115,  -38,  -38, 255 }, // cubeside
    {   1490,      0,   1600, 0,     0,   512,   73,  -73,   73,  50 }, // cubeside
    {   1490,   2240,   1600, 0,     0,     0,   73,  -73,   73, 255 }, // cubeside
    {   1490,      0,  -1600, 0,  1024,   512,   73,  -73,  -73,  50 }, // cubeside
    {   1490,   2240,  -1600, 0,  1024,     0,   73,  -73,  -73, 255 }, // cubeside

    {  -1490,   2240,   1600, 0,     0,  1024,  -73,   73,   73, 255 }, // cubetop
    {  -1490,   2240,  -1600, 0,     0,     0,  -73,   73,  -73, 255 }, // cubetop
    {   1490,   2240,   1600, 0,  1024,  1024,   73,   73,   73, 255 }, // cubetop
    {   1490,   2240,  -1600, 0,  1024,     0,   73,   73,  -73, 255 }, // cubetop
};
// clang-format on

// clang-format off
extern Gfx int_ols_gc01_model[] = {
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTLUT_Dolphin(15, 16, 1, int_ols_gc01_pal_other),
    gsDPLoadTextureBlock_4b_Dolphin(int_ols_gc01_back_tex, G_IM_FMT_CI, 32, 16, 15, GX_MIRROR, GX_MIRROR, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&int_ols_gc01_v[0], 8, 0),
    gsSPNTrianglesInit_5b(
    2, // tri count
    1, 2, 0, // tri0
    1, 3, 2, // tri1
    0, 0, 0 // tri2
    ),
    gsDPLoadTLUT_Dolphin(15, 16, 1, int_ols_gc01_pal_front_top),
    gsDPLoadTextureBlock_4b_Dolphin(int_ols_gc01_front_tex, G_IM_FMT_CI, 32, 16, 15, GX_MIRROR, GX_MIRROR, 0, 0),
    gsSPNTrianglesInit_5b(
    2, // tri count
    7, 4, 6, // tri0
    7, 5, 4, // tri1
    0, 0, 0 // tri2
    ),
    gsDPLoadTLUT_Dolphin(15, 16, 1, int_ols_gc01_pal_other),
    gsDPLoadTextureBlock_4b_Dolphin(int_ols_gc01_handle_tex, G_IM_FMT_CI, 16, 4, 15, GX_MIRROR, GX_MIRROR, 0, 0),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&int_ols_gc01_v[8], 15, 0),
    gsSPNTrianglesInit_5b(
    16, // tri count
    6, 9, 7, // tri0
    0, 4, 1, // tri1
    7, 5, 0 // tri2
    ),
    gsSPNTriangles_5b(
    1, 3, 2, // tri0
    7, 13, 6, // tri1
    0, 11, 10, // tri2
    0, 14, 7 // tri3
    ),
    gsSPNTriangles_5b(
    1, 12, 11, // tri0
    6, 8, 9, // tri1
    0, 5, 4, // tri2
    7, 9, 5 // tri3
    ),
    gsSPNTriangles_5b(
    1, 4, 3, // tri0
    7, 14, 13, // tri1
    0, 1, 11, // tri2
    0, 10, 14 // tri3
    ),
    gsSPNTriangles_5b(
    1, 2, 12, // tri0
    0, 0, 0, // tri1
    0, 0, 0, // tri2
    0, 0, 0 // tri3
    ),
    gsDPLoadTextureBlock_4b_Dolphin(int_ols_gc01_side_tex, G_IM_FMT_CI, 32, 16, 15, GX_MIRROR, GX_MIRROR, 0, 0),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&int_ols_gc01_v[23], 8, 0),
    gsSPNTrianglesInit_5b(
    4, // tri count
    1, 2, 0, // tri0
    7, 4, 6, // tri1
    1, 3, 2 // tri2
    ),
    gsSPNTriangles_5b(
    7, 5, 4, // tri0
    0, 0, 0, // tri1
    0, 0, 0, // tri2
    0, 0, 0 // tri3
    ),
    gsDPLoadTLUT_Dolphin(15, 16, 1, int_ols_gc01_pal_front_top),
    gsDPLoadTextureBlock_4b_Dolphin(int_ols_gc01_top_tex, G_IM_FMT_CI, 32, 32, 15, GX_MIRROR, GX_MIRROR, 0, 0),
    gsSPVertex(&int_ols_gc01_v[31], 4, 0),
    gsSPNTrianglesInit_5b(
    2, // tri count
    1, 2, 3, // tri0
    1, 0, 2, // tri1
    0, 0, 0 // tri2
    ),
    gsSPEndDisplayList(),
};
// clang-format on
