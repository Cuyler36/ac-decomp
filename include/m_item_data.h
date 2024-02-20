#ifndef M_ITEM_DATA_H
#define M_ITEM_DATA_H

#include "types.h"
#include "m_item_name.h"
#include "m_name_table.h"

#ifdef __cplusplus
extern "C" {
#endif

#define mID_ITEM_NUM 2000 // Total number of items

enum {
    mID_COLOR_NONE,
    mID_COLOR_YELLOW,
    mID_COLOR_RED,
    mID_COLOR_ORANGE,
    mID_COLOR_GREEN,
    mID_COLOR_BLUE,
    mID_COLOR_PURPLE,
    mID_COLOR_BROWN,
    mID_COLOR_WHITE,
    mID_COLOR_GRAY,
    mID_COLOR_BLACK,

    mID_COLOR_NUM
};

enum {
    mID_FTR_SIZE_NONE,
    mID_FTR_SIZE_1x1,
    mID_FTR_SIZE_2x1,
    mID_FTR_SIZE_2x2,
    mID_FTR_SIZE_3x2,
    mID_FTR_SIZE_3x3,

    mID_FTR_SIZE_NUM
};

/* sizeof (mID_data_c) == 0x24 */
typedef struct item_data_s {
    /* 0x00 */ mActor_name_t id;           /* item id */
    /* 0x02 */ u8 primary_color : 4;       /* main color */
    /* 0x02 */ u8 secondary_color : 4;     /* 2nd color */
    /* 0x03 */ u8 has_face : 1;            /* item has "face" */
    /* 0x03 */ u8 is_lucky : 1;            /* item is lucky */
    /* 0x03 */ u8 article : 2;             /* article */
    /* 0x03 */ u8 ftr_size : 4;            /* furniture size */
    /* 0x04 */ u8 name[mIN_ITEM_NAME_LEN]; /* name */
    /* 0x14 */ u8 birth_group;             /* birth group */
    /* 0x15 */ u8 series;                  /* furniture series */
    /* 0x16 */ u8 necessity_type : 6;      /* furniture necessity type */
    /* 0x16 */ u8 surface_type : 2;        /* furniture surface type */
    /* 0x17 */ u8 can_sell : 1;            /* can sell? */
    /* 0x17 */ u8 can_donate : 1;          /* can donate? */
    /* 0x17 */ u8 can_throwaway : 1;       /* can throw away? */
    /* 0x17 */ u8 can_place_in_letter : 1; /* can put in letter? */
    /* 0x17 */ u8 can_plant : 1;           /* can plant in ground? */
    /* 0x17 */ u8 can_dig : 1;             /* can dig up out of ground? */
    /* 0x17 */ u8 can_bury : 1;            /* can bury in ground? */
    /* 0x17 */ u8 can_eat : 1;             /* can eat? */
    /* 0x18 */ u32 price;                  /* price Nook buys for, sell price is 4x */
    /* 0x1C */ u16 inventory_icon;         /* inventory icon */
    /* 0x1E */ u16 ground_icon;            /* ground icon */
    /* 0x20 */ u8 can_place_as_ftr : 1;    /* gives the option to 'place' instead of just 'drop' */
    /* 0x20 */ u8 can_equip : 1;           /* can equip to hold */
    /* 0x20 */ u8 can_wear : 1;            /* can wear (shirts, headgear, etc?) */
    /* 0x20 */ u8 seasonality : 3;         /* season item is available in */
    /* 0x20 */ u8 rsv : 2;                 /* reserved */
    /* 0x21 */ u8 item_type;               /* type of item */
    /* 0x22 */ u8 hra_point_group;         /* point group for HRA base points */
    /* 0x23 */ u8 _pad;                    /* reserved */
} mID_data_c;

extern const mID_data_c* const mID_GetItemDataFromIdx(int idx);
extern const mID_data_c* const mID_GetItemDataFromId(mActor_name_t id);
extern const u8* mID_GetName(mActor_name_t id);
extern BOOL mID_CopyName(mActor_name_t id, u8* buf);
extern BOOL mID_GetLucky(mActor_name_t id);
extern BOOL mID_GetHasFace(mActor_name_t id);
extern int mID_GetPrimaryColor(mActor_name_t id);
extern int mID_GetSecondaryColor(mActor_name_t id);
extern BOOL mID_HasColor(mActor_name_t id, int color);
extern u32 mID_GetPrice(mActor_name_t id);

#ifdef __cplusplus
}
#endif

#endif
