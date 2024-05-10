#ifndef M_CARD_H
#define M_CARD_H

#include "dolphin/card.h"
#include "types.h"
#include "libu64/gfxprint.h"
#include "m_personal_id.h"
#include "m_land_h.h"
#include "m_private.h"
#include "m_diary.h"

#ifdef __cplusplus
extern "C" {
#endif

#define mCD_LAND_SAVE_SIZE 0x72000
#define mCD_ORIGINAL_SAVE_SIZE 0xE000
#define mCD_MAIL_SAVE_SIZE 0xC000
#define mCD_DIARY_SAVE_SIZE 0xC000
#define mCD_PRESENT_SAVE_SIZE 0x2000
#define mCD_PLAYER_SAVE_SIZE 0x6000

/* Offset to start of save data when loading from card.
   Skips the comment, banner, & icon data. */
#define mCD_SAVE_DATA_OFS 0x1440

#define mCD_LAND_PROTECT_CODE_MAGIC0 0x3C1C

enum {
    mCD_ERROR_NOT_ENABLED, // N [not enabled?]
    mCD_ERROR_AREA,        // A [area?]
    mCD_ERROR_WRITE,       // W [write]
    mCD_ERROR_READ,        // R [read?]
    mCD_ERROR_CHECKSUM,    // C [checksum]
    mCD_ERROR_OUTDATED,    // O [outdated]
    mCD_ERROR_CREATE,      // c [create]

    mCD_ERROR_NUM
};

enum {
    mCD_SPACE_BG_GET_SLOT,
    mCD_SPACE_BG_MAIN,

    mCD_SPACE_BG_NUM
};

enum {
    mCD_ARAM_DATA_ORIGINAL,
    mCD_ARAM_DATA_MAIL,
    mCD_ARAM_DATA_DIARY,

    mCD_ARAM_DATA_NUM
};

enum {
    mCD_FILE_SAVE_MISC,
    mCD_FILE_SAVE_MAIN,
    mCD_FILE_SAVE_MAIN_BAK,
    mCD_FILE_SAVE_MAIL,
    mCD_FILE_SAVE_ORIGINAL,
    mCD_FILE_SAVE_DIARY,
    mCD_FILE_PRESENT,
    mCD_FILE_PLAYER,

    mCD_FILE_NUM
};

/* This is also a priority table where lower = higher priority */
enum {
    mCD_TRANS_ERR_NONE,
    mCD_TRANS_ERR_1,
    mCD_TRANS_ERR_IOERROR,
    mCD_TRANS_ERR_3,
    mCD_TRANS_ERR_BROKEN_WRONGENCODING,
    mCD_TRANS_ERR_5,
    mCD_TRANS_ERR_NOT_MEMCARD,
    mCD_TRANS_ERR_7,
    mCD_TRANS_ERR_INVALID_NOLAND_CODE,
    mCD_TRANS_ERR_LAND_EXIST,
    mCD_TRANS_ERR_PASSPORT_EXIST,
    mCD_TRANS_ERR_NO_FILES,
    mCD_TRANS_ERR_NO_SPACE,
    mCD_TRANS_ERR_TOWN_INVALID,
    mCD_TRANS_ERR_OTHER_TOWN,
    mCD_TRANS_ERR_15,
    mCD_TRANS_ERR_16,
    mCD_TRANS_ERR_17,
    mCD_TRANS_ERR_18,
    mCD_TRANS_ERR_19,
    mCD_TRANS_ERR_20,
    mCD_TRANS_ERR_WRONGDEVICE,
    mCD_TRANS_ERR_NOCARD,
    mCD_TRANS_ERR_23,
    mCD_TRANS_ERR_GENERIC,
    mCD_TRANS_ERR_BUSY,

    mCD_TRANS_ERR_NUM
};

enum {
    mCD_HOME_SFX_NORMAL,
    mCD_HOME_SFX_DELETE,

    mCD_HOME_SFX_NUM
};

#define mCD_RESULT_ERROR -1
#define mCD_RESULT_BUSY 0
#define mCD_RESULT_SUCCESS 1

#define mCD_MEMCARD_SECTORSIZE 0x2000
#define mCD_ALIGN_SECTORSIZE(s) ALIGN_NEXT(s, mCD_MEMCARD_SECTORSIZE)

typedef struct {
    mLd_land_info_c land;
    PersonalID_c pid[PLAYER_NUM];
} mCD_persistent_data_c;

#define mCD_KEEP_ORIGINAL_PAGE_COUNT 8
#define mCD_KEEP_ORIGINAL_COUNT 12

typedef struct {
    int count;
    u8 _0004[mCD_KEEP_ORIGINAL_PAGE_COUNT][12];
    mNW_original_design_c original[mCD_KEEP_ORIGINAL_PAGE_COUNT][mCD_KEEP_ORIGINAL_COUNT];
    int _CC80; // force size to 0xCCA0
} mCD_keep_original_c ATTRIBUTE_ALIGN(32);

#define mCD_KEEP_MAIL_PAGE_COUNT 8
#define mCD_KEEP_MAIL_COUNT 20

typedef struct {
    int count;
    u8 _0004[mCD_KEEP_MAIL_PAGE_COUNT][12];
    Mail_c mail[mCD_KEEP_MAIL_PAGE_COUNT][mCD_KEEP_MAIL_COUNT];
} mCD_keep_mail_c ATTRIBUTE_ALIGN(32);

#define mCD_KEEP_DIARY_COUNT PLAYER_NUM
#define mCD_KEEP_DIARY_ENTRY_COUNT lbRTC_MONTHS_MAX

typedef struct {
    u16 checksum;
    mDi_entry_c entries[mCD_KEEP_DIARY_COUNT][mCD_KEEP_DIARY_ENTRY_COUNT];
} mCD_keep_diary_c ATTRIBUTE_ALIGN(32);

typedef struct {
    CARDStat stat;
    s32 fileNo;
    s32 freeBlocks;
    s32 result;
    s32 game_result;
    void* workArea;
    u8 _80[0x94 - 0x80];
} mCD_memMgr_card_info_c;

typedef struct {
    int proc;
    int _04;
    s32 fileNo;
    s32 chan;
    int _10;
    int _14;
    s32 game_result;
    int _1C;
} mCD_memMgr_fileInfo_c;

typedef struct {
    s32 chan;
    int loaded_file_type;
    u32 workArea_size;
    void* workArea;
    int _0010;
    int load_proc;
    u8 _0018[0x0054 - 0x0018];
    mCD_memMgr_card_info_c cards[CARD_NUM_CHANS];
    int _017C;
    int land_saved;
    int copy_protect;
    int _0188;
    int _018C;
    int _0190;
    int _0194;
    int _0198;
    int _019C;
    int _01A0;
    int broken_file_idx;
    u8 _01A8[0x11BC - 0x01A8];
    char filename[32];
} mCD_memMgr_c;

/* Bonus letter */
#define mCD_PRESENT_MAX 9

typedef struct present_save_s {
    u16 checksum;
    u16 present_count;
    Mail_c letters[mCD_PRESENT_MAX];
} PresentSave_c;

typedef union {
    struct {
        char comment[CARD_COMMENT_SIZE];
        u8 banner[0xE00];
        u8 icon[0x600];
        PresentSave_c save;
    };
    u8 __align_sector[mCD_MEMCARD_SECTORSIZE];
} PresentSaveFile_c ATTRIBUTE_ALIGN(32);

enum {
    mCD_PRESENT_TYPE_BONUS,
    mCD_PRESENT_TYPE_GIFT,

    mCD_PRESENT_TYPE_NUM
};

extern s32 mCD_GetThisLandSlotNo_code(int* player_no, s32* slot_card_results);
extern void mCD_save_data_aram_malloc();
extern void mCD_set_aram_save_data();
extern void mCD_init_card();

extern void mCD_PrintErrInfo(gfxprint_t* gfxprint);
extern void mCD_InitAll();
extern void mCD_LoadLand();
extern void mCD_toNextLand();

extern int mCD_EraseBrokenLand_bg(void*);
extern int mCD_card_format_bg(s32 chan);
extern void mCD_ReCheckLoadLand(GAME_PLAY* play);

#ifdef __cplusplus
}
#endif

#endif
