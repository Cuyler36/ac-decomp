#ifndef BG_ITEM_H
#define BG_ITEM_H

#include "types.h"
#include "m_actor.h"
#include "m_field_info.h"
#include "libultra/ultratypes.h"
#include "bg_item_h.h"

#ifdef __cplusplus
extern "C" {
#endif

#define bIT_PIT_NUM 13
#define bIT_DROP_NUM 19
#define bIT_FRUIT_DROP_NUM 6
#define bIT_HOLE_NUM 1
#define bIT_SHIN_NUM 1
#define bIT_TEN_COIN_NUM 5
#define bIT_FADE_NUM 3

enum {
    bIT_TYPE_CAT_BG_ITEM,
    bIT_TYPE_CAT_BG_ITEM2,
    bIT_TYPE_CAT_FTR,
    bIT_TYPE_CAT_ITEM1,
    bIT_TYPE_CAT_CRACK,
    bIT_TYPE_CAT_DUMMY,

    bIT_TYPE_CAT_NUM
};

enum {
    bIT_DRAW_TYPE_STUMP1,
    bIT_DRAW_TYPE_STUMP2,
    bIT_DRAW_TYPE_STUMP3,
    bIT_DRAW_TYPE_STUMP4,
    bIT_DRAW_TYPE_FENCE,
    bIT_DRAW_TYPE_NOTICE,
    bIT_DRAW_TYPE_GRASS,
    bIT_DRAW_TYPE_MAPBOARD,
    bIT_DRAW_TYPE_MELODY,
    bIT_DRAW_TYPE_FENCES,
    bIT_DRAW_TYPE_HONYCOMB,
    bIT_DRAW_TYPE_HOLE00_G,
    bIT_DRAW_TYPE_HOLE00_S,
    bIT_DRAW_TYPE_CRACK00_G,
    bIT_DRAW_TYPE_CRACK00_S,
    bIT_DRAW_TYPE_HOLE00_SHIN_G,
    bIT_DRAW_TYPE_STONE_A,
    bIT_DRAW_TYPE_STONE_B,
    bIT_DRAW_TYPE_STONE_C,
    bIT_DRAW_TYPE_STONE_D,
    bIT_DRAW_TYPE_STONE_E,
    bIT_DRAW_TYPE_TANE,
    bIT_DRAW_TYPE_TREE0,
    bIT_DRAW_TYPE_TREE1,
    bIT_DRAW_TYPE_TREE2,
    bIT_DRAW_TYPE_TREE3,
    bIT_DRAW_TYPE_TREE4,
    bIT_DRAW_TYPE_TREE4_AP,
    bIT_DRAW_TYPE_TREE4_OR,
    bIT_DRAW_TYPE_TREE4_PH,
    bIT_DRAW_TYPE_TREE4_PR,
    bIT_DRAW_TYPE_TREE4_NT,
    bIT_DRAW_TYPE_TREE4_BG,
    bIT_DRAW_TYPE_FLOWER_LEAF,
    bIT_DRAW_TYPE_FLOWER00,
    bIT_DRAW_TYPE_FLOWER01,
    bIT_DRAW_TYPE_FLOWER02,
    bIT_DRAW_TYPE_TREE0_DEAD,
    bIT_DRAW_TYPE_PALM000,
    bIT_DRAW_TYPE_PALM001,
    bIT_DRAW_TYPE_PALM002,
    bIT_DRAW_TYPE_PALM003,
    bIT_DRAW_TYPE_PALM004,
    bIT_DRAW_TYPE_PALM004_CC,
    bIT_DRAW_TYPE_PALM_STUMP001,
    bIT_DRAW_TYPE_PALM_STUMP002,
    bIT_DRAW_TYPE_PALM_STUMP003,
    bIT_DRAW_TYPE_PALM_STUMP004,
    bIT_DRAW_TYPE_PALM000_DEAD,
    bIT_DRAW_TYPE_CEDAR000,
    bIT_DRAW_TYPE_CEDAR001,
    bIT_DRAW_TYPE_CEDAR002,
    bIT_DRAW_TYPE_CEDAR003,
    bIT_DRAW_TYPE_CEDAR004,
    bIT_DRAW_TYPE_CEDAR_STUMP001,
    bIT_DRAW_TYPE_CEDAR_STUMP002,
    bIT_DRAW_TYPE_CEDAR_STUMP003,
    bIT_DRAW_TYPE_CEDAR_STUMP004,
    bIT_DRAW_TYPE_CEDAR000_DEAD,
    bIT_DRAW_TYPE_GOLD_TREE000,
    bIT_DRAW_TYPE_GOLD_TREE001,
    bIT_DRAW_TYPE_GOLD_TREE002,
    bIT_DRAW_TYPE_GOLD_TREE003,
    bIT_DRAW_TYPE_GOLD_TREE004,
    bIT_DRAW_TYPE_GOLD_TREE_STUMP001,
    bIT_DRAW_TYPE_GOLD_TREE_STUMP002,
    bIT_DRAW_TYPE_GOLD_TREE_STUMP003,
    bIT_DRAW_TYPE_GOLD_TREE_STUMP004,
    bIT_DRAW_TYPE_GOLD_TREE000_DEAD,
    bIT_DRAW_TYPE_APPLE,
    bIT_DRAW_TYPE_ORANGE,
    bIT_DRAW_TYPE_PEACH,
    bIT_DRAW_TYPE_PEAR,
    bIT_DRAW_TYPE_NUTS,
    bIT_DRAW_TYPE_MATUTAKE,
    bIT_DRAW_TYPE_KABU,
    bIT_DRAW_TYPE_FISH,
    bIT_DRAW_TYPE_BAG,
    bIT_DRAW_TYPE_LEAF,
    bIT_DRAW_TYPE_ROLL,
    bIT_DRAW_TYPE_BOX,
    bIT_DRAW_TYPE_PACK,
    bIT_DRAW_TYPE_PRESENT,
    bIT_DRAW_TYPE_SEED,
    bIT_DRAW_TYPE_HANIWA,
    bIT_DRAW_TYPE_OTHER,
    bIT_DRAW_TYPE_86,
    bIT_DRAW_TYPE_TOOL,
    bIT_DRAW_TYPE_FOSSIL,
    bIT_DRAW_TYPE_TRASH,
    bIT_DRAW_TYPE_90,
    bIT_DRAW_TYPE_OTOSI,
    bIT_DRAW_TYPE_SHELL_A,
    bIT_DRAW_TYPE_SHELL_B,
    bIT_DRAW_TYPE_SHELL_C,
    bIT_DRAW_TYPE_CANDY,
    bIT_DRAW_TYPE_COCONUT,
    bIT_DRAW_TYPE_97,
    bIT_DRAW_TYPE_CLOTH,
    bIT_DRAW_TYPE_CARPET,
    bIT_DRAW_TYPE_WALL,
    bIT_DRAW_TYPE_AXE,
    bIT_DRAW_TYPE_NET,
    bIT_DRAW_TYPE_ROD,
    bIT_DRAW_TYPE_SCOOP,
    bIT_DRAW_TYPE_GOLD_AXE,
    bIT_DRAW_TYPE_GOLD_NET,
    bIT_DRAW_TYPE_GOLD_ROD,
    bIT_DRAW_TYPE_GOLD_SCOOP,
    bIT_DRAW_TYPE_UMBRELLA,
    bIT_DRAW_TYPE_PINWHEEL,
    bIT_DRAW_TYPE_FAN,
    bIT_DRAW_TYPE_PAPER,
    bIT_DRAW_TYPE_SEEDBAG,
    bIT_DRAW_TYPE_LUCKBAG,
    bIT_DRAW_TYPE_GYMCARD,
    bIT_DRAW_TYPE_MUSIC,
    bIT_DRAW_TYPE_LOTTERY,
    bIT_DRAW_TYPE_BONE,
    bIT_DRAW_TYPE_DIARY,
    bIT_DRAW_TYPE_FORK,
    bIT_DRAW_TYPE_ROSE_RED,
    bIT_DRAW_TYPE_ROSE_WHITE,
    bIT_DRAW_TYPE_ROSE_YELLOW,
    bIT_DRAW_TYPE_ROSE_BLUE,
    bIT_DRAW_TYPE_FISH2,
    bIT_DRAW_TYPE_NONE,

    bIT_DRAW_TYPE_MAX
};

enum {
    bIT_PAL_FLOWER_A,
    bIT_PAL_FLOWER_B,
    bIT_PAL_FLOWER_C,
    bIT_PAL_GRASS,
    bIT_PAL_HOLE_G,
    bIT_PAL_HOLE_S,
    bIT_PAL_TREE,
    bIT_PAL_PALM_TREE,
    bIT_PAL_GOLD_TREE,

    bIT_PAL_NUM
};

/* sizeof(bg_item_tbl_c) == 0x100 */
typedef struct bg_item_tbl_s {
    u8 info[UT_TOTAL_NUM];
} bg_item_tbl_c;

/* sizeof(bg_item_block_info_tbl_c) == 0x404 */
typedef struct block_info_tbl_s {
    /* 0x000 */ int _000;
    /* 0x004 */ bg_item_tbl_c info_tbl[mFM_VISIBLE_BLOCK_NUM];
} bg_item_block_info_tbl_c;

/* sizeof(bg_item_type_data_c) == 0xC */
typedef struct type_data_s {
    /* 0x00 */ s8 _00;
    /* 0x02 */ s16 type;
    /* 0x04 */ f32* pos_x_tbl_p;
    /* 0x08 */ f32* pos_z_tbl_p;
} bg_item_type_data_c;

typedef struct draw_pos_s {
    MtxF mtxf;
    int _04;
    s16 next_add_cnt; // this isn't right
    u8 cull_flag;
    s8 sub_idx;
} bg_item_draw_pos_c;

typedef struct draw_list_s {
    void* draw_proc;
    u8 gfx_idx;
    u8 mat_idx;
} bg_item_draw_list_c;

typedef struct draw_part_s {
    Gfx** display_list_table_p;
    int draw_list_count;
    bg_item_draw_list_c** draw_list_table_p;
    int shadow_vtx_count;
    Vtx* shadow_vtx_p;
    f32 shadow_len;
    u8* shadow_vtx_fix_table_p;
    bg_item_draw_list_c* shadow_draw_list;
} bg_item_draw_part_c;

typedef struct draw_part_table_s {
    bg_item_draw_part_c* draw_part_p;
    u16 flags;
} bg_item_draw_part_table_c;

typedef struct draw_data_s {
    bg_item_draw_pos_c draw_pos[UT_TOTAL_NUM + 1];
    u16* idx_p;
    u16 val;
} bg_item_draw_data_c;

typedef struct draw_table_s {
    bg_item_draw_data_c draw_data;
    s16 draw_flag;
    u8 bx;
    u8 bz;
} bg_item_draw_table_c;

typedef struct common_info_s {
    bg_item_draw_part_table_c* draw_part_table_p;
    bg_item_type_data_c** type_data_table_p;
    f32* pos_table_p;
    u16* pal_p[bIT_PAL_NUM];
    u16 _30;
} bg_item_common_info_c;

typedef void (*bIT_ACTOR_DROP_MOVE_PROC)(bg_item_drop_c*);
typedef void (*bIT_ACTOR_DROP_DRAW_PROC)(GAME*, bg_item_common_info_c*, bg_item_drop_c*);
typedef void (*bIT_ACTOR_DROP_DESTRUCT_PROC)(bg_item_drop_c*);

struct drop_s {
    bIT_ACTOR_DROP_MOVE_PROC move_proc;
    bIT_ACTOR_DROP_DRAW_PROC draw_proc;
    bIT_ACTOR_DROP_DESTRUCT_PROC dt_proc;
    s16 mode;
    mActor_name_t fg_item;
    mActor_name_t display_fg_item;
    xyz_t position;
    xyz_t target_position;
    xyz_t scale;
    xyz_t offset;
    s_xyz angle;
    s16 _4A;
    f32 _4C;
    s16 drop_speed;
    f32 _54;
    s16 _58;
    s16 _5A;
    s16 _5C;
    f32 velocity_xz;
    f32 velocity_y;
    f32 acceleration_y;
    u16 flags;
    s16 _6E;
    s16 _70;
    u8 _72;
    mActor_name_t target_pos_fg_item;
    u16 _76;
    u16 _78;
    f32 _7C;
    s16 wait_counter;
    f32 total_distance_y;
    u16 _88;
    s16 _8A;
    ACTOR* actorx_p;
    s16 _90;
    s16 layer;
    xyz_t last_position;
    f32 last_velocity_xz;
    f32 last_velocity_y;
};

typedef struct drop_table_s {
    bg_item_drop_c* drop_p;
    s16 count;
} bg_item_drop_table_c;

typedef struct pit_s {
    xyz_t position;
    f32 scale;
    s16 mode;
    mActor_name_t hole_fg_item;
    s16 hole_timer;
    s16 wait_counter;
    s16 wait_type;
    mActor_name_t fg_item_in_pit;
} bg_item_pit_c;

struct hole_s {
    xyz_t position;
    f32 scale;
    s_xyz angle;
    s16 mode;
    mActor_name_t hole_fg_item;
    mActor_name_t fg_item;
    u16 counter;
    u16 max_counter;
    u16 wait_counter;
    s16 wait_type;
};

typedef struct shin_s {
    xyz_t position;
    mActor_name_t fg_item;
    u16 mode;
} bg_item_shin_c;

typedef struct ten_coin_s {
    u8 _00[0x30];
    xyz_t position;
    f32 scale;
    s16 angle;
    mActor_name_t fg_item;
    u16 _44;
    s16 _46;
    s16 _48;
    s16 _4A;
    s16 _4C;
    s16 _4E;
    s16 _50;
    s16 left_frames;
    s16 total_frames;
    s16 hit_count;
} bg_item_ten_coin_c;

typedef struct fade_s {
    xyz_t position;
    mActor_name_t fg_item;
    u8 alpha;
    u8 mode;
    u8 _10;
} bg_item_fade_c;

typedef int (*bIT_TALK_DISPLAY_LIMIT_CHECK_PROC)(int);

struct bg_common_s {
    bg_item_draw_table_c draw_table[mFM_VISIBLE_BLOCK_NUM];
    bg_item_block_info_tbl_c block_info_table;
    mFI_item_table_c item_table;
    bg_item_common_info_c common_info;
    bg_item_drop_table_c drop_info;
    bg_item_drop_table_c fruit_drop_info;
    bg_item_pit_c pit[bIT_PIT_NUM];
    bg_item_drop_c drop[bIT_DROP_NUM];
    bg_item_drop_c fruit_drop[bIT_FRUIT_DROP_NUM];
    bg_item_hole_c hole[bIT_HOLE_NUM];
    bg_item_shin_c shin[bIT_SHIN_NUM];
    bg_item_ten_coin_c ten_coin[bIT_TEN_COIN_NUM];
    bg_item_fade_c fade[bIT_FADE_NUM];
    bIT_TALK_DISPLAY_LIMIT_CHECK_PROC talk_display_limit_check_proc;
    ACTOR* bg_item_actorx_p;
    u16 flags;
    int _139E0[mFM_VISIBLE_BLOCK_NUM];
    int _139F4[mFM_VISIBLE_BLOCK_NUM];
};

typedef struct blk_idx_info_s {
    u16 idx[bIT_DRAW_TYPE_MAX];
} bg_item_idx_info_c;

typedef struct bg_item_actor_s BG_ITEM_ACTOR;

struct bg_item_actor_s {
    ACTOR actor_class;
    bg_item_common_c common;
    bg_item_idx_info_c item_idx_info[mFM_VISIBLE_BLOCK_NUM];
};

extern ACTOR_PROFILE BgItem_Profile;

#ifdef __cplusplus
}
#endif

#endif
