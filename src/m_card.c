#include "m_card.h"

#include "dolphin/private/card.h"
#include "libultra/libultra.h"
#include "m_malloc.h"
#include "libc64/sleep.h"
#include "zurumode.h"
#include "m_font.h"
#include "m_diary.h"
#include "m_house.h"
#include "m_common_data.h"
#include "jsyswrap.h"
#include "m_cockroach.h"

typedef struct card_bg_info {
    CARDFileInfo fileInfo;
    s32 fileNo;
    int space_proc;
    int tries;
    void* data;
    s32 offset;
    s32 length;
} mCD_bg_info_c;

enum {
    mCD_WBC_CHECK_SLOT,
    mCD_WBC_INIT,
    mCD_WBC_CHECK_FILESYSTEM,
    mCD_WBC_OPEN_FILE,
    mCD_WBC_READ,
    mCD_WBC_WRITE,

    mCD_WBC_FINISHED,
    mCD_WBC_NUM = mCD_WBC_FINISHED
};

enum {
    mCD_SAVEHOME_BG_PROC_GET_AREA,
    mCD_SAVEHOME_BG_PROC_ERASE_DUMMY,
    mCD_SAVEHOME_BG_PROC_CHECK_SLOT,
    mCD_SAVEHOME_BG_PROC_READ_SEND_PRESENT,
    mCD_SAVEHOME_BG_PROC_WRITE_PRESENT,
    mCD_SAVEHOME_BG_PROC_GET_SLOT,
    mCD_SAVEHOME_BG_PROC_CREATE_FILE,
    mCD_SAVEHOME_BG_PROC_CHECK_REPAIR_LAND,
    mCD_SAVEHOME_BG_PROC_REPAIR_LAND,
    mCD_SAVEHOME_BG_PROC_SET_FILE_PERMISSION,
    mCD_SAVEHOME_BG_PROC_SET_DATA,
    mCD_SAVEHOME_BG_PROC_WRITE_MAIN_2,
    mCD_SAVEHOME_BG_PROC_WRITE_BK,
    mCD_SAVEHOME_BG_PROC_SET_OTHERS,
    mCD_SAVEHOME_BG_PROC_WRITE_OTHERS,
    mCD_SAVEHOME_BG_PROC_GET_STATUS_2,
    mCD_SAVEHOME_BG_PROC_SET_STATUS_2,
    mCD_SAVEHOME_BG_PROC_RENAME,

    mCD_SAVEHOME_BG_PROC_NUM
};

static char mCD_file_name[] = "DobutsunomoriP_MURA";
static int l_mcd_err_debug[mCD_ERROR_NUM] = { FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE };
static s32 l_mcd_err_result = CARD_RESULT_READY;
static mCD_bg_info_c l_mcd_bg_info;

static void mCD_ClearErrInfo(void) {
    bzero(l_mcd_err_debug, sizeof(l_mcd_err_debug));
    l_mcd_err_result = 0;
}

static void mCD_OnErrInfo(int err) {
    if (err >= 0 && err < mCD_ERROR_NUM) {
        l_mcd_err_debug[err] = TRUE;
    }
}

static void mCD_OffErrInfo(int err) {
    if (err >= 0 && err < mCD_ERROR_NUM) {
        l_mcd_err_debug[err] = FALSE;
    }
}

static void mCD_SetErrResult(s32 result) {
    l_mcd_err_result = result;
}

extern void mCD_PrintErrInfo(gfxprint_t* gfxprint) {
    gfxprint_color(gfxprint, 250, 100, 250, 255);
    gfxprint_locate8x8(gfxprint, 22, 3);

    if (l_mcd_err_debug[mCD_ERROR_NOT_ENABLED]) {
        gfxprint_printf(gfxprint, "N");
    }

    if (l_mcd_err_debug[mCD_ERROR_AREA]) {
        gfxprint_printf(gfxprint, "A");
    }

    if (l_mcd_err_debug[mCD_ERROR_WRITE]) {
        gfxprint_printf(gfxprint, "W");
    }

    if (l_mcd_err_debug[mCD_ERROR_READ]) {
        gfxprint_printf(gfxprint, "R");
    }

    if (l_mcd_err_debug[mCD_ERROR_CHECKSUM]) {
        gfxprint_printf(gfxprint, "C");
    }

    if (l_mcd_err_debug[mCD_ERROR_OUTDATED]) {
        gfxprint_printf(gfxprint, "O");
    }

    if (l_mcd_err_debug[mCD_ERROR_CREATE]) {
        gfxprint_printf(gfxprint, "c");
    }

    if (l_mcd_err_result != CARD_RESULT_READY) {
        gfxprint_printf(gfxprint, "%d", l_mcd_err_result);
    }
}

static void* mCD_malloc_32(u32 size) {
    return zelda_malloc_align(size, 32);
}

static int mCD_check_card_common(s32* result, s32 req_sector_size, s32 chan) {
    s32 mem_size = 0;
    s32 sector_size = 0;
    int res = mCD_RESULT_BUSY;

    *result = CARDProbeEx(chan, &mem_size, &sector_size);

    if (*result == CARD_RESULT_READY && sector_size == req_sector_size) {
        res = mCD_RESULT_SUCCESS;
    } else if (*result != CARD_RESULT_BUSY) {
        res = mCD_RESULT_ERROR;
    }

    return res;
}

static int mCD_check_card(s32* result, s32 req_sector_size, s32 chan) {
    int ms = 0;
    int check_common_res = mCD_RESULT_BUSY;
    int res = mCD_RESULT_ERROR;

    while (check_common_res == mCD_RESULT_BUSY && ms < 500) {
        check_common_res = mCD_check_card_common(result, req_sector_size, chan);

        if (check_common_res == mCD_RESULT_BUSY) {
            msleep(1);
            ms++;
        }
    }

    if (check_common_res == mCD_RESULT_SUCCESS) {
        res = mCD_RESULT_SUCCESS;
    }

    return res;
}

static int mCD_check_sector_size(s32 req_sector_size, s32 chan) {
    s32 mem_size = 0;
    s32 sector_size = 0;
    s32 result = CARD_RESULT_BUSY;
    int ms = 0;
    int res = FALSE;

    while (result == CARD_RESULT_BUSY && ms < 500) {
        result = CARDProbeEx(chan, &mem_size, &sector_size);

        if (result == CARD_RESULT_BUSY) {
            msleep(1);
            ms++;
        }
    }

    if (result == CARD_RESULT_READY) {
        if (req_sector_size == sector_size) {
            res = TRUE;
        } else {
            res = FALSE;
        }
    }

    return res;
}

static int mCD_get_file_num_common(s32 chan) {
    CARDFileInfo fileInfo;
    s32 num = 0;
    s32 file_no;

    for (file_no = 0; file_no < CARD_MAX_FILE; file_no++) {
        s32 result = CARDFastOpen(chan, file_no, &fileInfo);

        if (result == CARD_RESULT_READY) {
            num++;
            CARDClose(&fileInfo);
        }
    }

    return num;
}

static int mCD_get_file_num(void* workArea, s32 chan) {
    int num = 0;
    s32 result;

    if (workArea != NULL && mCD_check_card(&result, mCD_MEMCARD_SECTORSIZE, chan) == mCD_RESULT_SUCCESS) {
        result = CARDMount(chan, workArea, NULL);

        if (result == CARD_RESULT_READY || result == CARD_RESULT_BROKEN) {
            result = CARDCheck(chan);
            num = mCD_get_file_num_common(chan);
            CARDUnmount(chan);
        } else if (result == CARD_RESULT_ENCODING) {
            CARDUnmount(chan);
        }
    }

    return num;
}

extern void mCD_init_card(void) {
    CARDInit();
}

static void mCD_ClearCardBgInfo(mCD_bg_info_c* bg_info) {
    bzero(bg_info, sizeof(mCD_bg_info_c));
}

static void mCD_StartSetCardBgInfo(void) {
    mCD_ClearCardBgInfo(&l_mcd_bg_info);
}

static int mCD_get_space_bg_get_slot(s32* freeBlocks, mCD_bg_info_c* bg_info, s32 chan, s32* result, void* workArea) {
    int res;

    bg_info->tries++;
    res = mCD_check_card_common(result, mCD_MEMCARD_SECTORSIZE, chan);
    if (res == mCD_RESULT_SUCCESS && workArea != NULL) {
        *freeBlocks = 0;
        *result = CARDMountAsync(chan, workArea, NULL, NULL);

        if (*result == CARD_RESULT_ENCODING) {
            CARDUnmount(chan);
            res = mCD_RESULT_ERROR;
        } else if (*result != CARD_RESULT_READY && *result != CARD_RESULT_BROKEN) {
            res = mCD_RESULT_ERROR;
        }

        bg_info->tries = 0;
    } else if (res != mCD_RESULT_BUSY) {
        *freeBlocks = 0;
        res = mCD_RESULT_ERROR;
        bg_info->tries = 0;
    } else if (bg_info->tries >= 100) {
        *freeBlocks = 0;
        res = mCD_RESULT_ERROR;
        bg_info->tries = 0;
    }

    return res;
}

static int mCD_get_space_bg_main(s32* freeBlocks, mCD_bg_info_c* bg_info, s32 chan, s32* result, void* workArea) {
    int res = mCD_RESULT_BUSY;

    *result = CARDGetResultCode(chan);
    if (*result == CARD_RESULT_READY || *result == CARD_RESULT_BROKEN) {
        *result = CARDCheck(chan);

        if (*result == CARD_RESULT_READY) {
            s32 freeFiles;

            *result = CARDFreeBlocks(chan, freeBlocks, &freeFiles);
            if (*result == CARD_RESULT_READY) {
                res = mCD_RESULT_SUCCESS;
            } else {
                res = mCD_RESULT_ERROR;
            }
        } else {
            res = mCD_RESULT_ERROR;
        }

        CARDUnmount(chan);
    } else if (*result != CARD_RESULT_BUSY) {
        if (*result == CARD_RESULT_ENCODING) {
            CARDUnmount(chan);
        }

        res = mCD_RESULT_ERROR;
    }

    return res;
}

typedef int (*mCD_GET_SPACE_BG_PROC)(s32*, mCD_bg_info_c*, s32, s32*, void*);
static int mCD_get_space_bg(s32* freeBlocks, s32 chan, s32* result, void* workArea) {
    static mCD_GET_SPACE_BG_PROC get_proc[mCD_SPACE_BG_NUM] = { &mCD_get_space_bg_get_slot, &mCD_get_space_bg_main };
    mCD_bg_info_c* bg_info = &l_mcd_bg_info;
    u8 proc_type = bg_info->space_proc;
    int res = mCD_RESULT_BUSY;

    *freeBlocks = 0;
    if (proc_type < mCD_SPACE_BG_NUM) {
        int proc_res = (*get_proc[proc_type])(freeBlocks, bg_info, chan, result, workArea);

        if (proc_res == mCD_RESULT_SUCCESS) {
            bg_info->space_proc++;

            if (bg_info->space_proc >= mCD_SPACE_BG_NUM) {
                res = mCD_RESULT_SUCCESS;
                mCD_ClearCardBgInfo(bg_info);
            }
        } else if (proc_res != mCD_RESULT_BUSY) {
            res = mCD_RESULT_ERROR;
            mCD_ClearCardBgInfo(bg_info);
        }
    } else {
        res = mCD_RESULT_ERROR;
        mCD_ClearCardBgInfo(bg_info);
    }

    return res;
}

static void mCD_close_and_unmount(CARDFileInfo* fileInfo, s32 chan) {
    CARDClose(fileInfo);
    CARDUnmount(chan);
}

static int mCD_bg_check_slot(mCD_bg_info_c* bg_info, s32 chan, s32* result) {
    int res = mCD_RESULT_BUSY;
    int common_res = mCD_check_card_common(result, mCD_MEMCARD_SECTORSIZE, chan);

    bg_info->tries++;
    if (common_res == mCD_RESULT_SUCCESS) {
        bg_info->space_proc++;
        bg_info->tries = 0;
        res = mCD_RESULT_SUCCESS;
    } else if (common_res != mCD_RESULT_BUSY || bg_info->tries >= 100) {
        bg_info->tries = 0;
        res = mCD_RESULT_ERROR;
    }

    return res;
}

static int mCD_bg_init_com(mCD_bg_info_c* bg_info, s32 chan, s32* result, void** workArea_p, void** read_p, int read) {
    int res;

    *workArea_p = mCD_malloc_32(CARD_WORKAREA_SIZE);
    if (read == TRUE) {
        *read_p = mCD_malloc_32(mCD_MEMCARD_SECTORSIZE);
    }

    if (*workArea_p != NULL && (read == FALSE || (read == TRUE && *read_p != NULL))) {
        *result = CARDMountAsync(chan, *workArea_p, NULL, NULL);

        if (*result == CARD_RESULT_READY) {
            bg_info->space_proc++;
            res = mCD_RESULT_SUCCESS;
        } else if (*result == CARD_RESULT_BROKEN) {
            *result = CARDCheckAsync(chan, NULL);

            if (*result == CARD_RESULT_READY) {
                bg_info->space_proc++;
                res = mCD_RESULT_SUCCESS;
            } else {
                if (*result == CARD_RESULT_BROKEN || *result == CARD_RESULT_ENCODING) {
                    CARDUnmount(chan);
                }

                res = mCD_RESULT_ERROR;
            }
        } else {
            if (*result == CARD_RESULT_ENCODING) {
                CARDUnmount(chan);
            }

            res = mCD_RESULT_ERROR;
        }
    } else {
        res = mCD_RESULT_ERROR;
    }

    return res;
}

static int mCD_bg_init(mCD_bg_info_c* bg_info, s32 chan, s32* result, void* data, const char* filename, s32 data_len,
                       u32 length, s32 offset, void** workArea_p, void** read_p, CARDStat* stat) {
    return mCD_bg_init_com(bg_info, chan, result, workArea_p, read_p, FALSE);
}

static int mCD_bg_check_filesystem(mCD_bg_info_c* bg_info, s32 chan, s32* result, void* data, const char* filename,
                                   s32 data_len, u32 length, s32 offset, void** workArea_p, void** read_p,
                                   CARDStat* stat) {
    int res = mCD_RESULT_BUSY;

    *result = CARDGetResultCode(chan);
    if (*result == CARD_RESULT_READY) {
        *result = CARDCheckAsync(chan, NULL);
        if (*result == CARD_RESULT_READY) {
            bg_info->space_proc++;
            res = mCD_RESULT_SUCCESS;
        } else {
            CARDUnmount(chan);
            res = mCD_RESULT_ERROR;
        }
    } else if (*result != CARD_RESULT_BUSY) {
        res = mCD_RESULT_ERROR;
    }

    return res;
}

static int mCD_write_bg_open_file(mCD_bg_info_c* bg_info, s32 chan, s32* result, void* data, const char* filename,
                                  s32 data_len, u32 length, s32 offset, void** workArea_p, void** read_p,
                                  CARDStat* stat) {
    int res = mCD_RESULT_BUSY;

    *result = CARDGetResultCode(chan);
    if (*result == CARD_RESULT_READY) {
        *result = CARDOpen(chan, filename, &bg_info->fileInfo);
        if (*result == CARD_RESULT_READY) {
            CARDStat stat;

            bg_info->fileNo = bg_info->fileInfo.fileNo;
            *result = CARDGetStatus(chan, bg_info->fileNo, &stat);
            if (*result == CARD_RESULT_READY && stat.length == length) {
                *result = CARDWriteAsync(&bg_info->fileInfo, data, data_len, offset, NULL);
                if (*result == CARD_RESULT_READY) {
                    bg_info->space_proc++;
                    res = mCD_RESULT_SUCCESS;
                } else {
                    mCD_close_and_unmount(&bg_info->fileInfo, chan);
                    res = mCD_RESULT_ERROR;
                }
            } else {
                mCD_close_and_unmount(&bg_info->fileInfo, chan);
                res = mCD_RESULT_ERROR;
            }
        } else {
            CARDUnmount(chan);
            res = mCD_RESULT_ERROR;
        }
    } else if (*result != CARD_RESULT_BUSY) {
        res = mCD_RESULT_ERROR;
    }

    return res;
}

static int mCD_write_bg_write(mCD_bg_info_c* bg_info, s32 chan, s32* result, void* data, const char* filename,
                              s32 data_len, u32 length, s32 offset, void** workArea_p, void** read_p, CARDStat* stat) {
    int res = mCD_RESULT_BUSY;

    *result = CARDGetResultCode(chan);
    if (*result == CARD_RESULT_READY) {
        *result = CARDWriteAsync(&bg_info->fileInfo, data, data_len, offset, NULL);
        if (*result == CARD_RESULT_READY) {
            bg_info->space_proc++;
            res = mCD_RESULT_SUCCESS;
        } else {
            res = mCD_RESULT_ERROR;
        }
    } else if (*result != CARD_RESULT_BUSY) {
        mCD_close_and_unmount(&bg_info->fileInfo, chan);
        res = mCD_RESULT_ERROR;
    }

    return res;
}

static int mCD_write_bg_cleanup(mCD_bg_info_c* bg_info, s32 chan, s32* result, void* data, const char* filename,
                                s32 data_len, u32 length, s32 offset, void** workArea_p, void** read_p,
                                CARDStat* stat) {
    int res = mCD_RESULT_BUSY;

    *result = CARDGetResultCode(chan);
    if (*result != CARD_RESULT_BUSY) {
        mCD_close_and_unmount(&bg_info->fileInfo, chan);
        bg_info->space_proc++;
        res = mCD_RESULT_SUCCESS;
    }

    return res;
}

typedef int (*mCD_BG_PROC)(mCD_bg_info_c*, s32, s32*, void*, const char*, s32, u32, s32, void**, void**, CARDStat*);

/* @unused static int mCD_write_bg(void* data, const char* filename, s32 data_len, u32 length, s32 offset, s32 chan,
 * s32* result) */
MATCH_FORCESTRIP static int mCD_write_bg(void* data, const char* filename, s32 data_len, u32 length, s32 offset,
                                         s32 chan, s32* result) {
    static void* work_p;
    // clang-format off
    static mCD_BG_PROC wbg_proc[] = {
        &mCD_bg_check_slot,
        &mCD_bg_init,
        &mCD_bg_check_filesystem,
        &mCD_write_bg_open_file,
        &mCD_write_bg_write,
        &mCD_write_bg_cleanup,
    };
    // clang-format on

    return 0;
}

static void mCD_bg_init_write_comp(mCD_bg_info_c* bg_info, s32 chan, s32* result, void* data, const char* filename,
                                   s32 data_len, u32 length, s32 offset, void** workArea_p, void** read_p,
                                   CARDStat* stat) {
    int res = mCD_bg_init_com(bg_info, chan, result, workArea_p, read_p, TRUE);

    if (res == mCD_RESULT_SUCCESS) {
        bg_info->data = data;
        bg_info->offset = offset;
        if (data_len < mCD_MEMCARD_SECTORSIZE) {
            bg_info->length = data_len;
        } else {
            bg_info->length = mCD_MEMCARD_SECTORSIZE;
        }
    }

    return res;
}

static int mCD_write_comp_bg_open_file(mCD_bg_info_c* bg_info, s32 chan, s32* result, void* data, const char* filename,
                                       s32 data_len, u32 length, s32 offset, void** workArea_p, void** read_p,
                                       CARDStat* stat) {
    int res = mCD_RESULT_BUSY;

    *result = CARDGetResultCode(chan);
    if (*result == CARD_RESULT_READY) {
        *result = CARDOpen(chan, filename, &bg_info->fileInfo);
        if (*result == CARD_RESULT_READY) {
            CARDStat stat;

            bg_info->fileNo = bg_info->fileInfo.fileNo;
            *result = CARDGetStatus(chan, bg_info->fileNo, &stat);
            if (*result == CARD_RESULT_READY && stat.length == length) {
                bzero(*read_p, mCD_MEMCARD_SECTORSIZE);
                *result = CARDReadAsync(&bg_info->fileInfo, *read_p, bg_info->length, bg_info->offset, NULL);
                if (*result == CARD_RESULT_READY) {
                    bg_info->space_proc++;
                    res = mCD_RESULT_SUCCESS;
                } else {
                    mCD_close_and_unmount(&bg_info->fileInfo, chan);
                    res = mCD_RESULT_ERROR;
                }
            } else {
                mCD_close_and_unmount(&bg_info->fileInfo, chan);
                res = mCD_RESULT_ERROR;
            }
        } else {
            CARDUnmount(chan);
            res = mCD_RESULT_ERROR;
        }
    } else if (*result != CARD_RESULT_BUSY) {
        res = mCD_RESULT_ERROR;
    }

    return res;
}

static int mCD_write_comp_bg_read(mCD_bg_info_c* bg_info, s32 chan, s32* result, void* data, const char* filename,
                                  s32 data_len, u32 length, s32 offset, void** workArea_p, void** read_p,
                                  CARDStat* stat) {
    int res = mCD_RESULT_BUSY;

    *result = CARDGetResultCode(chan);
    if (*result == CARD_RESULT_READY) {
        if (mem_cmp((u8*)*read_p, (u8*)bg_info->data, bg_info->length) == 0) {
            *result = CARDWriteAsync(&bg_info->fileInfo, bg_info->data, bg_info->length, bg_info->offset, NULL);
            if (*result == CARD_RESULT_READY) {
                bg_info->space_proc++;
                res = mCD_RESULT_SUCCESS;
            } else {
                mCD_close_and_unmount(&bg_info->fileInfo, chan);
                res = mCD_RESULT_ERROR;
            }
        } else {
            int ofs;

            bg_info->offset += mCD_MEMCARD_SECTORSIZE;
            ofs = bg_info->offset - offset;

            if (ofs >= data_len) {
                mCD_close_and_unmount(&bg_info->fileInfo, chan);
                bg_info->space_proc = mCD_WBC_FINISHED;
                res = mCD_RESULT_SUCCESS;
            } else {
                if (data_len - ofs < mCD_MEMCARD_SECTORSIZE) {
                    bg_info->length = data_len - ofs; /* Remaining size less than memcard sector size */
                } else {
                    bg_info->length = mCD_MEMCARD_SECTORSIZE;
                }

                bg_info->data = (void*)((u32)data + ofs);
                bzero(*read_p, mCD_MEMCARD_SECTORSIZE);
                *result = CARDReadAsync(&bg_info->fileInfo, *read_p, bg_info->length, bg_info->offset, NULL);
                if (*result == CARD_RESULT_READY) {
                    bg_info->space_proc = mCD_WBC_READ;
                    result = mCD_RESULT_SUCCESS;
                } else {
                    mCD_close_and_unmount(&bg_info->fileInfo, chan);
                    result = mCD_RESULT_ERROR;
                }
            }
        }
    } else if (*result != CARD_RESULT_BUSY) {
        mCD_close_and_unmount(&bg_info->fileInfo, chan);
        res = mCD_RESULT_ERROR;
    }

    return res;
}

static int mCD_write_comp_bg_write(mCD_bg_info_c* bg_info, s32 chan, s32* result, void* data, const char* filename,
                                   s32 data_len, u32 length, s32 offset, void** workArea_p, void** read_p,
                                   CARDStat* stat) {
    int res = mCD_RESULT_BUSY;

    *result = CARDGetResultCode(chan);
    if (*result == CARD_RESULT_READY) {
        int ofs;

        bg_info->offset += mCD_MEMCARD_SECTORSIZE;
        ofs = bg_info->offset - offset;

        if (ofs >= data_len) {
            mCD_close_and_unmount(&bg_info->fileInfo, chan);
            bg_info->space_proc = mCD_WBC_FINISHED;
            res = mCD_RESULT_SUCCESS;
        } else {
            if (data_len - ofs < mCD_MEMCARD_SECTORSIZE) {
                bg_info->length = data_len - ofs; /* Remaining size less than memcard sector size */
            } else {
                bg_info->length = mCD_MEMCARD_SECTORSIZE;
            }
            bg_info->data = (void*)((u32)data + ofs);
            bzero(*read_p, mCD_MEMCARD_SECTORSIZE);

            *result = CARDReadAsync(&bg_info->fileInfo, *read_p, bg_info->length, bg_info->offset, NULL);
            if (*result == CARD_RESULT_READY) {
                bg_info->space_proc = mCD_WBC_READ;
                result = mCD_RESULT_SUCCESS;
            } else {
                mCD_close_and_unmount(&bg_info->fileInfo, chan);
                result = mCD_RESULT_ERROR;
            }
        }
    } else if (*result != CARD_RESULT_BUSY) {
        mCD_close_and_unmount(&bg_info->fileInfo, chan);
        res = mCD_RESULT_ERROR;
    }

    return res;
}

static int mCD_write_comp_bg(void* data, const char* filename, s32 data_len, u32 length, s32 offset, s32 chan,
                             s32* result) {
    static void* work_p = NULL;
    static void* read_p = NULL;
    // clang-format off
    static mCD_BG_PROC wcbg_proc[] = {
        &mCD_bg_check_slot,
        &mCD_bg_init_write_comp,
        &mCD_bg_check_filesystem,
        &mCD_write_comp_bg_open_file,
        &mCD_write_comp_bg_read,
        &mCD_write_comp_bg_write,
    };
    // clang-format on

    mCD_bg_info_c* bg_info = &l_mcd_bg_info;
    int res = mCD_RESULT_BUSY;

    if (bg_info->space_proc >= 0 && bg_info->space_proc < mCD_WBC_NUM) {
        int success = (*wcbg_proc[bg_info->space_proc])(bg_info, chan, result, data, filename, data_len, length, offset,
                                                        &work_p, &read_p, NULL);

        if (success == mCD_RESULT_SUCCESS && bg_info->space_proc == mCD_WBC_FINISHED) {
            mCD_ClearCardBgInfo(bg_info);
            res = mCD_RESULT_SUCCESS;
        } else if (success == mCD_RESULT_ERROR) {
            mCD_ClearCardBgInfo(bg_info);
            res = mCD_RESULT_ERROR;
        }

        if (res != mCD_RESULT_BUSY) {
            if (work_p != NULL) {
                zelda_free(work_p);
                work_p = NULL;
            }

            if (read_p != NULL) {
                zelda_free(read_p);
                read_p = NULL;
            }
        }
    } else {
        if (work_p != NULL) {
            zelda_free(work_p);
            work_p = NULL;
        }

        if (read_p != NULL) {
            zelda_free(read_p);
            read_p = NULL;
        }

        mCD_ClearCardBgInfo(bg_info);
        work_p = NULL; // extra set
        res = mCD_RESULT_ERROR;
    }

    /* Debug display for error state */
    if (ZURUMODE2_ENABLED()) {
        if (res == mCD_RESULT_ERROR) {
            mCD_OnErrInfo(mCD_ERROR_WRITE);
            mCD_SetErrResult(*result);
        } else if (res == mCD_RESULT_SUCCESS) {
            mCD_OffErrInfo(mCD_ERROR_WRITE);
            mCD_SetErrResult(*result);
        }
    }

    return res;
}

static int mCD_read_bg_open_file(mCD_bg_info_c* bg_info, s32 chan, s32* result, void* data, const char* filename,
                                 s32 data_len, u32 length, s32 offset, void** workArea_p, void** read_p,
                                 CARDStat* stat) {
    int res = mCD_RESULT_BUSY;

    *result = CARDGetResultCode(chan);
    if (*result == CARD_RESULT_READY) {
        *result = CARDOpen(chan, filename, &bg_info->fileInfo);
        if (*result == CARD_RESULT_READY) {
            *result = CARDReadAsync(&bg_info->fileInfo, data, data_len, offset, NULL);

            if (result == CARD_RESULT_READY) {
                bg_info->space_proc = 4;
                res = mCD_RESULT_SUCCESS;
            } else {
                mCD_close_and_unmount(&bg_info->fileInfo, chan);
                res = mCD_RESULT_ERROR;
            }
        } else {
            CARDUnmount(chan);
            res = mCD_RESULT_ERROR;
        }
    } else if (*result != CARD_RESULT_BUSY) {
        if (*result != CARD_RESULT_BROKEN || *result != CARD_RESULT_ENCODING) {
            CARDUnmount(chan);
        }

        res = mCD_RESULT_ERROR;
    }

    return res;
}

static int mCD_read_bg_cleanup(mCD_bg_info_c* bg_info, s32 chan, s32* result, void* data, const char* filename,
                               s32 data_len, u32 length, s32 offset, void** workArea_p, void** read_p, CARDStat* stat) {
    int res = mCD_RESULT_BUSY;

    *result = CARDGetResultCode(chan);
    if (*result != CARD_RESULT_BUSY) {
        mCD_close_and_unmount(&bg_info->fileInfo, chan);
        bg_info->space_proc++;
        res = mCD_RESULT_SUCCESS;
    }

    return res;
}

/* @unused */
static int mCD_read_bg(void* data, const char* filename, s32 data_len, u32 length, s32 offset, s32 chan, s32* result) {
    static void* work_p;
    // clang-format off
    static mCD_BG_PROC rbg_proc[] = {
        &mCD_bg_check_slot,
        &mCD_bg_init,
        &mCD_bg_check_filesystem,
        &mCD_read_bg_open_file,
        &mCD_read_bg_cleanup,
    };
    // clang-format on

    return 0;
}

static int mCD_read_fg(void* buf, const char* filename, s32 length, s32 offset, s32 chan, s32* result) {
    int res = mCD_RESULT_ERROR;

    if (buf != NULL && IS_ALIGNED((u32)buf, 32)) {
        int card_res = mCD_check_card(result, mCD_MEMCARD_SECTORSIZE, chan);

        if (card_res == mCD_RESULT_SUCCESS) {
            void* workArea = mCD_malloc_32(CARD_WORKAREA_SIZE);

            if (workArea != NULL) {
                *result = CARDMount(chan, workArea, NULL);
                if (*result == CARD_RESULT_READY || *result == CARD_RESULT_BROKEN) {
                    *result = CARDCheck(chan);
                    if (*result == CARD_RESULT_READY) {
                        CARDFileInfo fileInfo;

                        *result = CARDOpen(chan, filename, &fileInfo);
                        if (*result == CARD_RESULT_READY) {
                            *result = CARDRead(&fileInfo, buf, length, offset);
                            if (*result == CARD_RESULT_READY) {
                                res = mCD_RESULT_SUCCESS;
                            }

                            mCD_close_and_unmount(&fileInfo, chan);
                        } else {
                            CARDUnmount(chan);
                        }
                    } else {
                        CARDUnmount(chan);
                    }
                } else if (*result == CARD_RESULT_ENCODING) {
                    CARDUnmount(chan);
                }

                if (workArea != NULL) {
                    zelda_free(workArea);
                }
            }
        }
    }

    return res;
}

static int mCD_find_fg(const char* filename, void* workArea, s32 chan, s32* result) {
    int res = FALSE;

    if (mCD_check_card(result, mCD_MEMCARD_SECTORSIZE, chan) == mCD_RESULT_SUCCESS && workArea != NULL) {
        *result = CARDMount(chan, workArea, NULL);
        if (*result == CARD_RESULT_READY || *result == CARD_RESULT_BROKEN) {
            *result = CARDCheck(chan);
            if (*result == CARD_RESULT_READY) {
                CARDFileInfo fileInfo;

                *result = CARDOpen(chan, filename, &fileInfo);
                if (*result == CARD_RESULT_READY) {
                    mCD_close_and_unmount(&fileInfo, chan);
                    res = TRUE;
                } else {
                    CARDUnmount(chan);
                }
            } else {
                CARDUnmount(chan);
            }
        } else if (*result == CARD_RESULT_ENCODING) {
            CARDUnmount(chan);
        }
    }

    return res;
}

static int mCD_format_bg_mount(mCD_bg_info_c* bg_info, s32 chan, s32* result, void* data, const char* filename,
                               s32 data_len, u32 length, s32 offset, void** workArea_p, void** read_p, CARDStat* stat) {
    *workArea_p = mCD_malloc_32(CARD_WORKAREA_SIZE);
    if (*workArea_p != NULL) {
        *result = CARDMountAsync(chan, *workArea_p, NULL, NULL);
        if (*result == CARD_RESULT_READY || *result == CARD_RESULT_BROKEN || *result == CARD_RESULT_ENCODING) {
            bg_info->space_proc++;
            return mCD_RESULT_SUCCESS;
        } else {
            return mCD_RESULT_ERROR;
        }
    } else {
        return mCD_RESULT_ERROR;
    }
}

static int mCD_format_bg_open_file(mCD_bg_info_c* bg_info, s32 chan, s32* result, void* data, const char* filename,
                                   s32 data_len, u32 length, s32 offset, void** workArea_p, void** read_p,
                                   CARDStat* stat) {
    int res = mCD_RESULT_BUSY;

    *result = CARDGetResultCode(chan);
    if (*result == CARD_RESULT_READY || *result == CARD_RESULT_BROKEN || *result == CARD_RESULT_ENCODING) {
        *result = CARDFormatAsync(chan, NULL);
        if (*result == CARD_RESULT_READY) {
            bg_info->space_proc++;
            res = mCD_RESULT_SUCCESS;
        } else {
            CARDUnmount(chan);
            res = mCD_RESULT_ERROR;
        }
    } else if (*result != CARD_RESULT_BUSY) {
        res = mCD_RESULT_ERROR;
    }

    return res;
}

static int mCD_format_bg_cleanup(mCD_bg_info_c* bg_info, s32 chan, s32* result, void* data, const char* filename,
                                 s32 data_len, u32 length, s32 offset, void** workArea_p, void** read_p,
                                 CARDStat* stat) {
    int res = mCD_RESULT_BUSY;

    *result = CARDGetResultCode(chan);
    if (*result != CARD_RESULT_BUSY) {
        res = *result == CARD_RESULT_READY ? mCD_RESULT_SUCCESS : mCD_RESULT_ERROR;
        CARDUnmount(chan);
        bg_info->space_proc++;
    }

    return res;
}

static int mCD_format_bg(s32 chan, s32* result) {
    static void* work_p = NULL;
    // clang-format off
    static mCD_BG_PROC fbg_proc[] = {
        &mCD_bg_check_slot,
        &mCD_format_bg_mount,
        &mCD_format_bg_open_file,
        &mCD_format_bg_cleanup,
    };
    // clang-format on

    mCD_bg_info_c* bg_info = &l_mcd_bg_info;
    int res = mCD_RESULT_BUSY;

    if (bg_info->space_proc >= 0 && bg_info->space_proc < mCD_WBC_NUM) {
        int success = (*fbg_proc[bg_info->space_proc])(bg_info, chan, result, NULL, NULL, 0, 0, 0, &work_p, NULL, NULL);

        if (success == mCD_RESULT_SUCCESS && bg_info->space_proc == 4) {
            mCD_ClearCardBgInfo(bg_info);
            res = mCD_RESULT_SUCCESS;
        } else if (success == mCD_RESULT_ERROR) {
            mCD_ClearCardBgInfo(bg_info);
            res = mCD_RESULT_ERROR;
        }

        if (res != mCD_RESULT_BUSY) {
            if (work_p != NULL) {
                zelda_free(work_p);
                work_p = NULL;
            }
        }
    } else {
        if (work_p != NULL) {
            zelda_free(work_p);
            work_p = NULL;
        }

        res = mCD_RESULT_ERROR;
    }
}

static int mCD_set_file_status_bg_open_file(mCD_bg_info_c* bg_info, s32 chan, s32* result, void* data,
                                            const char* filename, s32 data_len, u32 length, s32 offset,
                                            void** workArea_p, void** read_p, CARDStat* stat) {
    int res = mCD_RESULT_BUSY;

    *result = CARDGetResultCode(chan);
    if (*result == CARD_RESULT_READY) {
        *result = CARDOpen(chan, filename, &bg_info->fileInfo);
        if (*result == CARD_RESULT_READY) {
            bg_info->fileNo = bg_info->fileInfo.fileNo;
            *result = CARDSetStatusAsync(chan, bg_info->fileNo, stat, NULL);
            if (*result == CARD_RESULT_READY) {
                bg_info->space_proc++;
                res = mCD_RESULT_SUCCESS;
            } else {
                mCD_close_and_unmount(&bg_info->fileInfo, chan);
                res = mCD_RESULT_ERROR;
            }
        } else {
            CARDUnmount(chan);
            res = mCD_RESULT_ERROR;
        }
    } else if (*result != CARD_RESULT_BUSY) {
        res = mCD_RESULT_ERROR;
    }

    return res;
}

static int mCD_set_file_status_bg_cleanup(mCD_bg_info_c* bg_info, s32 chan, s32* result, void* data,
                                          const char* filename, s32 data_len, u32 length, s32 offset, void** workArea_p,
                                          void** read_p, CARDStat* stat) {
    int res = mCD_RESULT_BUSY;

    *result = CARDGetResultCode(chan);
    if (*result != CARD_RESULT_BUSY) {
        res = *result == CARD_RESULT_READY ? mCD_RESULT_SUCCESS : mCD_RESULT_ERROR;
        CARDUnmount(chan);
        bg_info->space_proc++;
    }

    return res;
}

static int mCD_set_file_status_bg(CARDStat* stat, const char* filename, s32 chan, s32* result) {
    static void* work_p = NULL;
    // clang-format off
    static mCD_BG_PROC ssbg_proc[] = {
        &mCD_bg_check_slot,
        &mCD_format_bg_mount,
        &mCD_bg_check_filesystem,
        &mCD_set_file_status_bg_open_file,
        &mCD_set_file_status_bg_cleanup,
    };
    // clang-format on

    mCD_bg_info_c* bg_info = &l_mcd_bg_info;
    int res = mCD_RESULT_BUSY;

    if (bg_info->space_proc >= 0 && bg_info->space_proc < mCD_WBC_NUM) {
        int success =
            (*ssbg_proc[bg_info->space_proc])(bg_info, chan, result, NULL, filename, 0, 0, 0, &work_p, NULL, stat);

        if (success == mCD_RESULT_SUCCESS && bg_info->space_proc == 5) {
            mCD_ClearCardBgInfo(bg_info);
            res = mCD_RESULT_SUCCESS;
        } else if (success == mCD_RESULT_ERROR) {
            mCD_ClearCardBgInfo(bg_info);
            res = mCD_RESULT_ERROR;
        }

        if (res != mCD_RESULT_BUSY) {
            if (work_p != NULL) {
                zelda_free(work_p);
                work_p = NULL;
            }
        }
    } else {
        if (work_p != NULL) {
            zelda_free(work_p);
            work_p = NULL;
        }

        res = mCD_RESULT_ERROR;
    }

    return res;
}

static int mCD_get_file_status_bg_open_file(mCD_bg_info_c* bg_info, s32 chan, s32* result, void* data,
                                            const char* filename, s32 data_len, u32 length, s32 offset,
                                            void** workArea_p, void** read_p, CARDStat* stat) {
    int res = mCD_RESULT_BUSY;

    *result = CARDGetResultCode(chan);
    if (*result == CARD_RESULT_READY) {
        *result = CARDOpen(chan, filename, &bg_info->fileInfo);
        if (*result == CARD_RESULT_READY) {
            bg_info->fileNo = bg_info->fileInfo.fileNo;
            *result = CARDGetStatus(chan, bg_info->fileNo, stat);
            if (*result == CARD_RESULT_READY) {
                bg_info->space_proc++;
                res = mCD_RESULT_SUCCESS;
            } else {
                res = mCD_RESULT_ERROR;
            }

            mCD_close_and_unmount(&bg_info->fileInfo, chan);
        } else {
            CARDUnmount(chan);
            res = mCD_RESULT_ERROR;
        }
    } else if (*result != CARD_RESULT_BUSY) {
        res = mCD_RESULT_ERROR;
    }

    return res;
}

static int mCD_get_file_status_bg(CARDStat* stat, const char* filename, s32 chan, s32* result) {
    static void* work_p = NULL;
    // clang-format off
    static mCD_BG_PROC gsbg_proc[] = {
        &mCD_bg_check_slot,
        &mCD_format_bg_mount,
        &mCD_bg_check_filesystem,
        &mCD_get_file_status_bg_open_file,
    };
    // clang-format on

    mCD_bg_info_c* bg_info = &l_mcd_bg_info;
    int res = mCD_RESULT_BUSY;

    if (bg_info->space_proc >= 0 && bg_info->space_proc < mCD_WBC_NUM) {
        int success =
            (*gsbg_proc[bg_info->space_proc])(bg_info, chan, result, NULL, filename, 0, 0, 0, &work_p, NULL, stat);

        if (success == mCD_RESULT_SUCCESS && bg_info->space_proc == 5) {
            mCD_ClearCardBgInfo(bg_info);
            res = mCD_RESULT_SUCCESS;
        } else if (success == mCD_RESULT_ERROR) {
            mCD_ClearCardBgInfo(bg_info);
            res = mCD_RESULT_ERROR;
        }

        if (res != mCD_RESULT_BUSY) {
            if (work_p != NULL) {
                zelda_free(work_p);
                work_p = NULL;
            }
        }
    } else {
        if (work_p != NULL) {
            zelda_free(work_p);
            work_p = NULL;
        }

        res = mCD_RESULT_ERROR;
    }

    return res;
}

static int mCD_create_file_bg_create(mCD_bg_info_c* bg_info, s32 chan, s32* result, void* data, const char* filename,
                                     s32 data_len, u32 length, s32 offset, void** workArea_p, void** read_p,
                                     CARDStat* stat) {
    int res = mCD_RESULT_BUSY;

    *result = CARDGetResultCode(chan);
    if (*result == CARD_RESULT_READY) {
        *result = CARDCreateAsync(chan, filename, length, &bg_info->fileInfo, NULL);
        if (*result == CARD_RESULT_READY) {
            bg_info->space_proc++;
            res = mCD_RESULT_SUCCESS;
        } else {
            CARDUnmount(chan);
            res = mCD_RESULT_ERROR;
        }
    } else if (*result != CARD_RESULT_BUSY) {
        res = mCD_RESULT_ERROR;
    }

    return res;
}

static int mCD_create_file_bg_set_not_copy(mCD_bg_info_c* bg_info, s32 chan, s32* result, void* data,
                                           const char* filename, s32 data_len, u32 length, s32 offset,
                                           void** workArea_p, void** read_p, CARDStat* stat) {
    int res = mCD_RESULT_BUSY;

    *result = CARDGetResultCode(chan);
    if (*result == CARD_RESULT_READY) {
        CARDDir dir;

        bg_info->fileNo = bg_info->fileInfo.fileNo;
        *result = __CARDGetStatusEx(chan, &bg_info->fileInfo, &dir);
        if (*result == CARD_RESULT_READY) {
            dir.permission |= data_len;

            /* @bug - missing *result = ? */
            __CARDSetStatusEx(chan, bg_info->fileNo, &dir);
            if (*result == CARD_RESULT_READY) {
                bg_info->space_proc++;
                res = mCD_RESULT_SUCCESS;
            } else {
                res = mCD_RESULT_ERROR;
            }
        } else {
            res = mCD_RESULT_ERROR;
        }

        mCD_close_and_unmount(&bg_info->fileInfo, chan);
    } else if (*result != CARD_RESULT_BUSY) {
        CARDUnmount(chan);
        res = mCD_RESULT_ERROR;
    }

    return res;
}

static int mCD_create_file_bg(const char* filename, s32 perms, u32 length, s32 chan, s32* result, s32* fileNo) {
    static void* work_p = NULL;
    // clang-format off
    static mCD_BG_PROC cbg_proc[] = {
        &mCD_bg_check_slot,
        &mCD_format_bg_mount,
        &mCD_bg_check_filesystem,
        &mCD_create_file_bg_create,
        &mCD_create_file_bg_set_not_copy,
    };
    // clang-format on

    mCD_bg_info_c* bg_info = &l_mcd_bg_info;
    int res = mCD_RESULT_BUSY;

    *fileNo = 0;
    if (bg_info->space_proc >= 0 && bg_info->space_proc < 5) {
        int success = (*cbg_proc[bg_info->space_proc])(bg_info, chan, result, NULL, filename, perms, length, 0, &work_p,
                                                       NULL, NULL);

        if (success == mCD_RESULT_SUCCESS && bg_info->space_proc == 5) {
            *fileNo = bg_info->fileNo;
            mCD_ClearCardBgInfo(bg_info);
            res = mCD_RESULT_SUCCESS;
        } else if (success == mCD_RESULT_ERROR) {
            mCD_ClearCardBgInfo(bg_info);
            res = mCD_RESULT_ERROR;
        }

        if (res != mCD_RESULT_BUSY) {
            if (work_p != NULL) {
                zelda_free(work_p);
                work_p = NULL;
            }
        }
    } else {
        if (work_p != NULL) {
            zelda_free(work_p);
            work_p = NULL;
        }

        res = mCD_RESULT_ERROR;
    }

    /* Debug display for error state */
    if (ZURUMODE2_ENABLED()) {
        if (res == mCD_RESULT_ERROR) {
            mCD_OnErrInfo(mCD_ERROR_CREATE);
            mCD_SetErrResult(*result);
        } else if (res == mCD_RESULT_SUCCESS) {
            mCD_OffErrInfo(mCD_ERROR_CREATE);
            mCD_SetErrResult(*result);
        }
    }

    return res;
}

static int mCD_set_file_permission_bg_set(mCD_bg_info_c* bg_info, s32 chan, s32* result, void* data,
                                          const char* filename, s32 data_len, u32 length, s32 offset, void** workArea_p,
                                          void** read_p, CARDStat* stat) {
    int res = mCD_RESULT_BUSY;

    *result = CARDGetResultCode(chan);
    if (*result == CARD_RESULT_READY) {
        *result = CARDOpen(chan, filename, &bg_info->fileInfo);
        if (*result == CARD_RESULT_READY) {
            CARDDir dir;

            bg_info->fileNo = bg_info->fileInfo.fileNo;
            *result = __CARDGetStatusEx(chan, &bg_info->fileInfo, &dir);
            if (*result == CARD_RESULT_READY) {
                dir.permission |= data_len;

                /* @bug - missing *result = ? */
                __CARDSetStatusEx(chan, bg_info->fileNo, &dir);
                if (*result == CARD_RESULT_READY) {
                    bg_info->space_proc++;
                    res = mCD_RESULT_SUCCESS;
                } else {
                    res = mCD_RESULT_ERROR;
                }
            } else {
                res = mCD_RESULT_ERROR;
            }

            mCD_close_and_unmount(&bg_info->fileInfo, chan);
        } else {
            CARDUnmount(chan);
            res = mCD_RESULT_ERROR;
        }
    } else if (*result != CARD_RESULT_BUSY) {
        CARDUnmount(chan);
        res = mCD_RESULT_ERROR;
    }

    return res;
}

static int mCD_set_file_permission_bg(const char* filename, s32 perms, s32 chan, s32* result, s32* fileNo) {
    static void* work_p = NULL;
    // clang-format off
    static mCD_BG_PROC sp_proc[] = {
        &mCD_bg_check_slot,
        &mCD_format_bg_mount,
        &mCD_bg_check_filesystem,
        &mCD_set_file_permission_bg_set,
    };
    // clang-format on

    mCD_bg_info_c* bg_info = &l_mcd_bg_info;
    int res = mCD_RESULT_BUSY;

    *fileNo = 0;
    if (bg_info->space_proc >= 0 && bg_info->space_proc < 4) {
        int success =
            (*sp_proc[bg_info->space_proc])(bg_info, chan, result, NULL, filename, perms, 0, 0, &work_p, NULL, NULL);

        if (success == mCD_RESULT_SUCCESS && bg_info->space_proc == 4) {
            *fileNo = bg_info->fileNo;
            mCD_ClearCardBgInfo(bg_info);
            res = mCD_RESULT_SUCCESS;
        } else if (success == mCD_RESULT_ERROR) {
            mCD_ClearCardBgInfo(bg_info);
            res = mCD_RESULT_ERROR;
        }

        if (res != mCD_RESULT_BUSY) {
            if (work_p != NULL) {
                zelda_free(work_p);
                work_p = NULL;
            }
        }
    } else {
        if (work_p != NULL) {
            zelda_free(work_p);
            work_p = NULL;
        }

        res = mCD_RESULT_ERROR;
    }

    /* Debug display for error state */
    if (ZURUMODE2_ENABLED()) {
        if (res == mCD_RESULT_ERROR) {
            mCD_OnErrInfo(mCD_ERROR_CREATE);
            mCD_SetErrResult(*result);
        } else if (res == mCD_RESULT_SUCCESS) {
            mCD_OffErrInfo(mCD_ERROR_CREATE);
            mCD_SetErrResult(*result);
        }
    }

    return res;
}

static int mCD_erase_file_bg_erase(mCD_bg_info_c* bg_info, s32 chan, s32* result, void* data, const char* filename,
                                   s32 data_len, u32 length, s32 offset, void** workArea_p, void** read_p,
                                   CARDStat* stat) {
    int res = mCD_RESULT_BUSY;

    *result = CARDGetResultCode(chan);
    if (*result == CARD_RESULT_READY) {
        *result = CARDDeleteAsync(chan, filename, NULL);
        if (*result == CARD_RESULT_READY) {
            bg_info->space_proc++;
            res = mCD_RESULT_SUCCESS;
        } else {
            CARDUnmount(chan);
            res = mCD_RESULT_ERROR;
        }
    } else if (*result != CARD_RESULT_BUSY) {
        res = mCD_RESULT_BUSY;
    }

    return res;
}

static int mCD_set_file_permission_bg(const char* filename, s32 chan, s32* result) {
    static void* work_p = NULL;
    // clang-format off
    static mCD_BG_PROC ebg_proc[] = {
        &mCD_bg_check_slot,
        &mCD_format_bg_mount,
        &mCD_bg_check_filesystem,
        &mCD_erase_file_bg_erase,
        &mCD_format_bg_cleanup,
    };
    // clang-format on

    mCD_bg_info_c* bg_info = &l_mcd_bg_info;
    int res = mCD_RESULT_BUSY;

    if (bg_info->space_proc >= 0 && bg_info->space_proc < 5) {
        int success =
            (*ebg_proc[bg_info->space_proc])(bg_info, chan, result, NULL, filename, 0, 0, 0, &work_p, NULL, NULL);

        if (success == mCD_RESULT_SUCCESS && bg_info->space_proc == 5) {
            mCD_ClearCardBgInfo(bg_info);
            res = mCD_RESULT_SUCCESS;
        } else if (success == mCD_RESULT_ERROR) {
            mCD_ClearCardBgInfo(bg_info);
            res = mCD_RESULT_ERROR;
        }

        if (res != mCD_RESULT_BUSY) {
            if (work_p != NULL) {
                zelda_free(work_p);
                work_p = NULL;
            }
        }
    } else {
        if (work_p != NULL) {
            zelda_free(work_p);
            work_p = NULL;
        }

        res = mCD_RESULT_ERROR;
    }

    return res;
}

static int mCD_erase_file_fg(const char* filename, s32 chan, s32* result, void* workArea) {
    int res = FALSE;

    if (chan != -1 && workArea != NULL && mCD_check_card(result, mCD_MEMCARD_SECTORSIZE, chan) == mCD_RESULT_SUCCESS) {
        *result = CARDMount(chan, workArea, NULL);
        if (*result == CARD_RESULT_READY || *result == CARD_RESULT_BUSY) {
            *result = CARDCheck(chan);
            if (*result == CARD_RESULT_READY) {
                *result = CARDDelete(chan, filename);
                if (*result == CARD_RESULT_READY) {
                    res = TRUE;
                }
            }

            CARDUnmount(chan);
        } else if (*result == CARD_RESULT_ENCODING) {
            CARDUnmount(chan);
        }
    }

    return res;
}

static int mCD_rename_file_fg(const char* new_filename, const char* filename, s32 chan, s32* result, void* workArea) {
    int res = FALSE;

    if (chan != -1 && workArea != NULL && mCD_check_card(result, mCD_MEMCARD_SECTORSIZE, chan) == mCD_RESULT_SUCCESS) {
        *result = CARDMount(chan, workArea, NULL);
        if (*result == CARD_RESULT_READY || *result == CARD_RESULT_BUSY) {
            *result = CARDCheck(chan);
            if (*result == CARD_RESULT_READY) {
                *result = CARDRename(chan, filename, new_filename);
                if (*result == CARD_RESULT_READY) {
                    res = TRUE;
                }
            }

            CARDUnmount(chan);
        } else if (*result == CARD_RESULT_ENCODING) {
            CARDUnmount(chan);
        }
    }

    return res;
}

static u16 l_mcd_copy_protect = 0xFFFF;

// clang-format off
static u8 l_mcd_font_1[256] ATTRIBUTE_ALIGN(4) = {
    0xa1, 0xbf, 0xc4, 0xc0, 0xc1, 0xc2, 0xc3, 0xc5, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce,
    0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdf, 0xde, 0xe0,
    0xa0, 0x21, 0x94, 0xe1, 0xe2, 0x25, 0x26, 0xb4, 0x28, 0x29, 0x7e, 0x97, 0x82, 0x2d, 0x2e, 0x97,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x97, 0x3c, 0x3d, 0x3e, 0x3f,
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0xe3, 0x97, 0xe4, 0xe5, 0x5f,
    0xe7, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0xe8, 0xe9, 0xea, 0xeb, 0x97,
    0x97, 0xec, 0xed, 0xee, 0xef, 0x95, 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf8, 0xf9, 0xfa,
    0x96, 0xfb, 0xfc, 0xfd, 0xff, 0xfe, 0xdd, 0x7c, 0xa7, 0x97, 0x97, 0xb6, 0xb5, 0xb3, 0xb2, 0xb9,
    0xaf, 0xac, 0xc6, 0xe6, 0x84, 0xbb, 0xab, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x2f, 0x97,
    0x97, 0x97, 0x97, 0x97, 0x2b, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97,
    0x97, 0xf7, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97,
    0x3b, 0x23, 0xa0, 0xa0, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97,
    0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97,
    0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97, 0x97
};
// clang-format on

static char l_comment_0_str[32] ATTRIBUTE_ALIGN(4) = "Animal Crossing";
static char l_comment_1_str[32] ATTRIBUTE_ALIGN(4) = " Town Data";
static char l_comment_erase_land[32] ATTRIBUTE_ALIGN(4) = "Letters and Designs Data";
static char l_comment_player_0_str[32] ATTRIBUTE_ALIGN(4) = "Animal Crossing";
static char l_comment_player_1_str[32] ATTRIBUTE_ALIGN(4) = " is traveling";
static char l_comment_present_0_str[32] ATTRIBUTE_ALIGN(4) = "Animal Crossing";
static char l_comment_present_1_str[32] ATTRIBUTE_ALIGN(4) = "Bonus Letters:";
static char l_comment_gift_1_str[32] ATTRIBUTE_ALIGN(4) = "Gift Letters:";

static char l_mCD_land_file_name[32] ATTRIBUTE_ALIGN(4) = "DobutsunomoriP_MURA";
static char l_mCD_land_file_name_dummy[32] ATTRIBUTE_ALIGN(4) = "DobutsunomoriP_MURA_d";
static char l_mCD_player_file_name[32] ATTRIBUTE_ALIGN(4) = "DobutsunomoriP_PL_";
static char l_mCD_present_file_name[32] ATTRIBUTE_ALIGN(4) = "DobutsunomoriP_Omake_";

typedef struct {
    const char* filename; /* filename */
    int filesize;         /* size of the entire file */
    int entrysize;        /* size of the sub-entry */
} mCD_file_entry_c;

static mCD_file_entry_c l_mcd_file_table[] = {
    { l_mCD_land_file_name, mCD_LAND_SAVE_SIZE, sizeof(Save) },
    { l_mCD_land_file_name, mCD_LAND_SAVE_SIZE, sizeof(Save) },
    { l_mCD_land_file_name, mCD_LAND_SAVE_SIZE, sizeof(Save) },
    { l_mCD_land_file_name, mCD_LAND_SAVE_SIZE, mCD_MAIL_SAVE_SIZE },
    { l_mCD_land_file_name, mCD_LAND_SAVE_SIZE, mCD_ORIGINAL_SAVE_SIZE },
    { l_mCD_land_file_name, mCD_LAND_SAVE_SIZE, mCD_DIARY_SAVE_SIZE },
    { l_mCD_present_file_name, mCD_PRESENT_SAVE_SIZE, mCD_PRESENT_SAVE_SIZE },
    { l_mCD_player_file_name, mCD_PLAYER_SAVE_SIZE, mCD_PLAYER_SAVE_SIZE },
};

static int l_keepSave_set;
static int l_mcd_keep_startCond = 0;
static int l_aram_access_bit = 0;

static void mCD_clear_aram_access_bit(void) {
    l_aram_access_bit = 0;
}

static u32 l_aram_alloc_size_table[mCD_ARAM_DATA_NUM] = {
    sizeof(mCD_keep_original_c),
    sizeof(mCD_keep_mail_c),
    sizeof(mCD_keep_diary_c),
};
static u32 l_aram_real_size_32_table[mCD_ARAM_DATA_NUM] = {
    sizeof(mCD_keep_original_c),
    sizeof(mCD_keep_mail_c),
    sizeof(mCD_keep_diary_c),
};
static void* l_aram_block_p_table[mCD_ARAM_DATA_NUM];

extern void mCD_save_data_aram_malloc(void) {
    int i;

    for (i = 0; i < mCD_ARAM_DATA_NUM; i++) {
        l_aram_block_p_table[i] = JC__JKRAllocFromAram(l_aram_alloc_size_table[i]);
    }
}

extern int mCD_save_data_aram_to_main(void* dst, u32 size, int idx) {
    int res = FALSE;
    void* aram_block;

    if (idx >= mCD_ARAM_DATA_NUM) {
        idx = 0;
    }

    aram_block = l_aram_block_p_table[idx];
    if (aram_block != NULL) {
        JC__JKRAramToMainRam_block(aram_block, dst, size);
        DCFlushRange(dst, size);
        res = TRUE;
    }

    return res;
}

extern int mCD_save_data_main_to_aram(void* src, u32 size, int idx) {
    int res = FALSE;
    void* aram_block;

    if (idx >= mCD_ARAM_DATA_NUM) {
        idx = 0;
    }

    aram_block = l_aram_block_p_table[idx];
    if (aram_block != NULL) {
        DCFlushRange(src, size);
        JC__JKRMainRamToAram_block(src, aram_block, size);
        res = TRUE;
    }

    return res;
}

static u32 mCD_get_aram_save_data_max_size(void) {
    u32 size = 0;
    int i;

    for (i = 0; i < mCD_ARAM_DATA_NUM; i++) {
        if (size < l_aram_alloc_size_table[i]) {
            size = l_aram_alloc_size_table[i];
        }
    }

    return size;
}

static void mCD_set_init_mail_data(u8* buf) {
    mCD_keep_mail_c* keep_mail = (mCD_keep_mail_c*)buf;
    int i;
    int j;

    for (i = 0; i < mCD_KEEP_MAIL_PAGE_COUNT; i++) {
        mem_clear(keep_mail->_0004[i], sizeof(keep_mail->_0004), CHAR_SPACE);
        for (j = 0; j < mCD_KEEP_MAIL_COUNT; j++) {
            mMl_clear_mail(&keep_mail->mail[i][j]);
        }
    }
}

static void mCD_set_init_original_data(u8* buf) {
    mCD_keep_original_c* keep_original = (mCD_keep_original_c*)buf;
    int i;
    int j;

    for (i = 0; i < mCD_KEEP_ORIGINAL_PAGE_COUNT; i++) {
        mem_clear(keep_original->_0004[i], sizeof(keep_original->_0004), CHAR_SPACE);
        for (j = 0; j < mCD_KEEP_ORIGINAL_COUNT; j++) {
            mNW_InitOriginalData(&keep_original->original[i][j]);
        }
    }
}

static void mCD_set_init_diary_data(u8* buf) {
    mCD_keep_diary_c* keep_diary = (mCD_keep_diary_c*)buf;
    int i;
    int j;

    for (i = 0; i < mCD_KEEP_ORIGINAL_PAGE_COUNT; i++) {
        for (j = 0; j < mCD_KEEP_ORIGINAL_COUNT; j++) {
            mDi_init_diary(&keep_diary->entries[i][j]);
        }
    }
}

typedef void (*mCD_DATA_INIT_PROC)(u8*);

extern void mCD_set_aram_save_data(void) {
    static mCD_DATA_INIT_PROC init_proc[] = {
        &mCD_set_init_original_data,
        &mCD_set_init_mail_data,
        &mCD_set_init_diary_data,
    };
    u32 max_size = mCD_get_aram_save_data_max_size();
    u8* buf = (u8*)mCD_malloc_32(max_size);

    if (buf != NULL) {
        int i;

        for (i = 0; i < mCD_ARAM_DATA_NUM; i++) {
            if (l_aram_block_p_table[i] != NULL) {
                bzero(buf, max_size);
                (*init_proc[i])(buf);
                mCD_save_data_main_to_aram(buf, l_aram_real_size_32_table[i], i);
            }
        }

        zelda_free(buf);
        mCD_clear_aram_access_bit();
    }
}

static int mCD_TransErrorCode(s32 result_code) {
    switch (result_code) {
        case CARD_RESULT_READY:
            return mCD_TRANS_ERR_NONE;
        case CARD_RESULT_BROKEN:
            return mCD_TRANS_ERR_NOCARD;
        case CARD_RESULT_IOERROR:
            return mCD_TRANS_ERR_IOERROR;
        case CARD_RESULT_ENCODING:
        case CARD_RESULT_NOCARD:
            return mCD_TRANS_ERR_BROKEN_WRONGENCODING;
        case CARD_RESULT_NOENT:
        case CARD_RESULT_INSSPACE:
            return mCD_TRANS_ERR_NO_SPACE;
        /* This might be default case */
        case CARD_RESULT_EXIST:
        case CARD_RESULT_CANCELED:
        case CARD_RESULT_FATAL_ERROR:
        case CARD_RESULT_NAMETOOLONG:
        case CARD_RESULT_NOFILE:
            return mCD_TRANS_ERR_GENERIC;
        case CARD_RESULT_BUSY:
            return mCD_TRANS_ERR_BUSY;
        default:
            return mCD_TRANS_ERR_GENERIC;
    }
}

static int mCD_TransErrorCode_nes(s32 result_code) {
    switch (result_code) {
        case CARD_RESULT_READY:
            return mCD_TRANS_ERR_NONE;
        case CARD_RESULT_WRONGDEVICE:
            return mCD_TRANS_ERR_WRONGDEVICE;
        case CARD_RESULT_NOCARD:
            return mCD_TRANS_ERR_NOCARD;
        case CARD_RESULT_IOERROR:
            return mCD_TRANS_ERR_IOERROR;
        case CARD_RESULT_BROKEN:
        case CARD_RESULT_ENCODING:
            return mCD_TRANS_ERR_BROKEN_WRONGENCODING;
        case CARD_RESULT_NOENT:
        case CARD_RESULT_INSSPACE:
            return mCD_TRANS_ERR_NO_SPACE;
        /* This might be default case */
        case CARD_RESULT_EXIST:
        case CARD_RESULT_CANCELED:
        case CARD_RESULT_FATAL_ERROR:
        case CARD_RESULT_NAMETOOLONG:
        case CARD_RESULT_NOFILE:
            return mCD_TRANS_ERR_GENERIC;
        case CARD_RESULT_BUSY:
            return mCD_TRANS_ERR_BUSY;
        default:
            return mCD_TRANS_ERR_GENERIC;
    }
}

static int mCD_get_offset(int idx) {
    mCD_file_entry_c* entry = l_mcd_file_table;
    int ofs = 0;
    int i;

    if (idx > mCD_FILE_SAVE_MISC && idx < mCD_FILE_PRESENT) {
        for (i = idx - 1; i >= 0; i--) {
            ofs += entry->entrysize;
            entry++;
        }
    }

    return ofs;
}

static int mCD_get_size(int idx) {
    int size = 0;

    if (idx >= 0 && idx < mCD_FILE_NUM) {
        size = l_mcd_file_table[idx].entrysize;
    }

    return size;
}

static Save l_keepSave;
static mCD_memMgr_c l_memMgr;

static void mCD_ClearMemMgr_com(mCD_memMgr_c* mgr) {
    bzero(mgr, sizeof(mCD_memMgr_c));
    mgr->chan = -1;
    mgr->land_saved = -1;
    mgr->copy_protect = -1;
    mgr->broken_file_idx = -1;
}

static void mCD_ClearMemMgr_com2(mCD_memMgr_c* mgr) {
    if (mgr->workArea != NULL) {
        zelda_free(mgr->workArea);
    }

    if (mgr->cards[0].workArea != NULL) {
        zelda_free(mgr->cards[0].workArea);
    }

    if (mgr->cards[1].workArea != NULL) {
        zelda_free(mgr->cards[1].workArea);
    }

    mCD_ClearMemMgr_com(mgr);
}

static void mCD_ClearKeepLand(void) {
    bzero(&l_keepSave, sizeof(l_keepSave));
    l_keepSave_set = FALSE;
}

static void mCD_ClearMemMgr(void) {
    mCD_ClearMemMgr_com(&l_memMgr);
}

typedef struct {
    PersonalID_c pid;
    char filename[32];
} mCD_cardPrivate_c;

static mCD_cardPrivate_c l_mcd_card_private_table[19];
static mCD_cardPrivate_c l_mcd_card_private;

static void mCD_ClearCardPrivateTable_com(mCD_cardPrivate_c* priv, int count) {
    for (count; count != 0; count--) {
        mPr_ClearPersonalID(&priv->pid);
        mem_clear((u8*)priv->filename, sizeof(priv->filename), 0);
        priv++;
    }
}

static void mCD_ClearCardPrivateTable(void) {
    mCD_ClearCardPrivateTable_com(l_mcd_card_private_table, ARRAY_COUNT(l_mcd_card_private_table));
}

static void mCD_SetCardPrivateTable(mCD_cardPrivate_c* priv, PersonalID_c* pid, char* filename) {
    mPr_CopyPersonalID(&priv->pid, pid);
    bcopy(filename, priv->filename, sizeof(priv->filename));
}

typedef struct {
    u16 checksum;
    Private_c priv;
    Animal_c remove_animal;
    u16 copy_protect;
    u8 _2DEA[54];
} mCD_foreigner_c;

static union {
    mCD_foreigner_c file;
    u8 sector_align[mCD_ALIGN_SECTORSIZE(sizeof(mCD_foreigner_c))];
} l_mcd_foreigner_file;

static void mCD_ClearForeignerFile(mCD_foreigner_c* foreigner) {
    bzero(foreigner, sizeof(mCD_foreigner_c));
    foreigner->checksum = 0;
    mPr_ClearPrivateInfo(&foreigner->priv);
    mNpc_ClearAnimalInfo(&foreigner->remove_animal);
    foreigner->copy_protect = 0xFFFF;
}

typedef struct {
    u16 code[4];
} mCD_LandProtectCode_c;

static mCD_LandProtectCode_c l_keep_noLandCode;

static void mCD_ClearNoLandProtectCode(mCD_LandProtectCode_c* protect_code) {
    bzero(protect_code, sizeof(mCD_LandProtectCode_c));
}

static int mCD_CheckInitProtectCode(mCD_LandProtectCode_c* protect_code) {
    int res = FALSE;
    int i;

    for (i = 0; i < 4; i++) {
        if (protect_code->code[i] != 0) {
            break;
        }
    }

    if (i == 4) {
        res = TRUE;
    }

    return res;
}

static int mCD_CheckProtectCode(mCD_LandProtectCode_c* protect_code) {
    int res = FALSE;

    if (protect_code->code[0] == 0x3C1C) {
        int i;

        for (i = 0; i < 3; i++) {
            if (protect_code->code[1 + i] == 0) {
                break;
            }
        }

        if (i == 3) {
            res = TRUE;
        }
    }

    return res;
}

static void mCD_MakeProtectCode(mCD_LandProtectCode_c* protect_code) {
    int i;

    protect_code->code[0] = mCD_LAND_PROTECT_CODE_MAGIC0;
    for (i = 0; i < 3; i++) {
        protect_code->code[1 + i] = RANDOM(0xFFFE);
    }
}

static int mCD_CompNoLandCode(mCD_LandProtectCode_c* code0, mCD_LandProtectCode_c* code1) {
    int res = FALSE;

    if (mCD_CheckProtectCode(code0) == TRUE) {
        int i;

        for (i = 0; i < 4; i++) {
            if (code0->code[i] != code1->code[i]) {
                break;
            }
        }

        if (i == 4) {
            res = TRUE;
        }
    }

    return res;
}

static void mCD_SetForeignerFile(mCD_foreigner_c* foreigner, Private_c* priv, Animal_c* animal) {
    mPr_CopyPrivateInfo(&foreigner->priv, priv);
    bcopy(animal, &foreigner->remove_animal, sizeof(Animal_c));
    foreigner->checksum = mFRm_GetFlatCheckSum((u16*)foreigner, sizeof(mCD_foreigner_c), foreigner->checksum);
}

static void mCD_clear_all_control(void);

extern void mCD_InitAll(void) {
    mCD_SetStartCardBgInfo();
    mCD_ClearMemMgr();
    mCD_ClearKeepLand();
    mCD_ClearCardPrivateTable();
    mCD_ClearCardPrivateTable_com(&l_mcd_card_private, 1);
    mCD_clear_all_control();
    l_mcd_copy_protect = 0xFFFF;
    mCD_ClearForeignerFile(&l_mcd_foreigner_file);
    mCD_ClearNoLandProtectCode(&l_keep_noLandCode);
    mCD_ClearErrInfo();
}

static int mCD_save_file(void* data, int file_idx, s32 chan, s32* result) {
    int res = mCD_RESULT_ERROR;

    if (data != NULL && file_idx >= 0 && file_idx < mCD_FILE_NUM) {
        mCD_file_entry_c* file = &l_mcd_file_table[file_idx];
        int ofs = mCD_get_offset(file_idx);

        res = mCD_write_comp_bg(data, file->filename, file->entrysize, file->filesize, ofs, chan, result);
    }

    return res;
}

static int mCD_load_file(void* buf, int file_idx, s32 chan, s32* result) {
    int res = mCD_RESULT_ERROR;

    if (buf != NULL && file_idx >= 0 && file_idx < mCD_FILE_NUM) {
        mCD_file_entry_c* file = &l_mcd_file_table[file_idx];
        int ofs = mCD_get_offset(file_idx);

        res = mCD_read_fg(buf, file->filename, file->entrysize, ofs, chan, result);
    }

    return res;
}

/* This struct seems to be stubbed */
typedef struct {
    int _00;
    int _04;
    int _08;
    int _0C;
    int _10;
    mCD_LandProtectCode_c protect_code; // assumed?
    int _1C;
    int _20;
    int _24;
    int _28;
    int _2C;
    int _30;
} mCD_wctrl_c;

static mCD_wctrl_c l_mCD_wctrl;

static void mCD_clear_write_control_common(mCD_wctrl_c* write_control) {
    write_control->_00 = 0;
    write_control->_04 = 0;
    write_control->_08 = 0;
    write_control->_0C = 0;
    bzero(&write_control->protect_code, sizeof(mCD_LandProtectCode_c));
    write_control->_1C = 0;
    write_control->_20 = 0;
    write_control->_24 = 0;
    write_control->_28 = 0;
    write_control->_30 = 0;
}

static void mCD_clear_all_control(void) {
    mCD_clear_write_control_common(&l_mCD_wctrl);
}

static int mCD_GetHighPriority_common(mCD_memMgr_c* mgr, int prio0, int prio1) {
    mgr->chan = 0;
    if (prio1 < prio0) {
        mgr->chan = 1;
        prio0 = prio1;
    }

    return prio0;
}

static int mCD_check_Land_exist_com(s32 chan) {
    int res = FALSE;

    if (chan == 0 || chan == 1) {
        CARDFileInfo fileInfo;

        if (CARDOpen(chan, l_mCD_land_file_name, &fileInfo) == CARD_RESULT_READY) {
            res = TRUE;
        }
    }

    return res;
}

static int mCD_check_Land_exist(s32 chan, void* workArea) {
    int res = FALSE;

    if ((chan == 0 || chan == 1) && workArea != NULL) {
        s32 result;

        if (mCD_check_card(&result, mCD_MEMCARD_SECTORSIZE, chan) == mCD_RESULT_SUCCESS) {
            result = CARDMount(chan, workArea, NULL);
            if (result == CARD_RESULT_READY || result == CARD_RESULT_BROKEN) {
                result = CARDCheck(chan);
                if (result == CARD_RESULT_READY) {
                    res = mCD_check_Land_exist_com(chan);
                }

                CARDUnmount(chan);
            } else if (result == CARD_RESULT_ENCODING) {
                CARDUnmount(chan);
            }
        }
    }

    return res;
}

static int mCD_CheckFilename(const char* name0, const char* name1, int len) {
    int res = FALSE;
    int i = 0;

    while (name0 != NULL && i < len && *name0 != '\0') {
        if (*name0 != *name1) {
            break;
        }

        i++;
        name0++;
        name1++;
    }

    if (i == len) {
        res = TRUE;
    }

    return res;
}

static int mCD_CheckPresentFilename(CARDStat* stat) {
    return mCD_CheckFilename(stat->fileName, l_mCD_present_file_name, 21);
}

static int mCD_CheckPresentFileStatus(CARDStat* stat) {
    int res = FALSE;

    if (stat->company[0] == '0' && stat->company[1] == '1') {
        res = mCD_CheckPresentFilename(stat);
    }

    return res;
}

static int mCD_CheckPassportFilename(CARDStat* stat) {
    return mCD_CheckFilename(stat->fileName, l_mCD_player_file_name, 18);
}

static int mCD_CheckPassportFileStatus(CARDStat* stat) {
    int res = FALSE;

    if (stat->company[0] == '0' && stat->company[1] == '1') {
        res = mCD_CheckPassportFilename(stat);
    }

    return res;
}

static int mCD_set_to_num(char* c) {
    int n = 0;
    int fig = 0;

    if (c != NULL) {
        int num = 0;
        int i;

        for (i = 0; i < 3; i++) {
            if (c[i] == '\0') {
                break;
            }

            num = i != 0 ? i * 10 : 1;
            fig++;
        }

        for (fig; fig != 0; fig--) {
            n += (*c - '0') * num;
            num /= 10;
            c++;
        }
    }

    return n;
}

static int mCD_GetPassportFileIdx(char* filename) {
    return mCD_set_to_num(&filename[18]);
}

static int mCD_CheckPassportFile_slot(s32 chan, void* workArea) {
    int res = FALSE;

    if ((chan == 0 || chan == 1) && workArea != NULL) {
        s32 result;

        if (mCD_check_card(&result, mCD_MEMCARD_SECTORSIZE, chan) == mCD_RESULT_SUCCESS) {
            result = CARDMount(chan, workArea, NULL);
            if (result == CARD_RESULT_READY || result == CARD_RESULT_BROKEN) {
                result = CARDCheck(chan);
                if (result == CARD_RESULT_READY) {
                    /* Check all files on the memcard */
                    CARDStat stat;
                    int i;

                    for (i = 0; i < CARD_MAX_FILE; i++) {
                        result = CARDGetStatus(chan, i, &stat);
                        if (result == CARD_RESULT_READY) {
                            if (mCD_CheckPassportFileStatus(&stat) == TRUE) {
                                res = TRUE;
                                break;
                            }
                        }
                    }
                }

                CARDUnmount(chan);
            } else if (result == CARD_RESULT_ENCODING) {
                CARDUnmount(chan);
            }
        }
    }

    return res;
}

static int mCD_GetSpaceSlot_bg2(mCD_memMgr_c* mgr, int size) {
    int chan = mgr->_0010 & 1 ? 1 : 0;
    mCD_memMgr_card_info_c* card_info = &mgr->cards[chan];
    int res = mCD_RESULT_BUSY;

    if (card_info->workArea != NULL) {
        int space_res = mCD_get_space_bg(&card_info->freeBlocks, chan, &card_info->result, card_info->workArea);
        if (space_res == mCD_RESULT_SUCCESS) {
            if (card_info->freeBlocks >= size) {
                if (mCD_check_Land_exist(chan, card_info->workArea) == TRUE) {
                    card_info->game_result = mCD_TRANS_ERR_LAND_EXIST;
                } else if (mCD_CheckPassportFile_slot(chan, card_info->workArea) == TRUE) {
                    card_info->game_result = mCD_TRANS_ERR_PASSPORT_EXIST;
                } else if (mCD_get_file_num(card_info->workArea, chan) >= CARD_MAX_FILE) {
                    card_info->game_result = mCD_TRANS_ERR_NO_FILES;
                } else {
                    mgr->chan = chan;
                    res = mCD_RESULT_SUCCESS;
                }
            } else {
                card_info->result = CARD_RESULT_INSSPACE;
            }

            mgr->_0010 |= 1 << chan;
        } else if (space_res != mCD_RESULT_BUSY) {
            if (mCD_check_sector_size(mCD_MEMCARD_SECTORSIZE, chan) == FALSE) {
                card_info->game_result = mCD_TRANS_ERR_NOT_MEMCARD;
            }

            mgr->_0010 |= 1 << chan;
        }
    } else {
        mgr->_0010 |= 1 << chan;
        res = mCD_RESULT_ERROR;
    }

    if (card_info->game_result != mCD_TRANS_ERR_LAND_EXIST && card_info->game_result != mCD_TRANS_ERR_PASSPORT_EXIST &&
        card_info->game_result != mCD_TRANS_ERR_NOT_MEMCARD && card_info->game_result != mCD_TRANS_ERR_NO_FILES) {
        card_info->game_result = mCD_TransErrorCode_nes(card_info->result);
    }

    /* If no memcard was chosen */
    if (mgr->_0010 == 0b11 && res == mCD_RESULT_BUSY) {
        mCD_GetHighPriority_common(mgr, mgr->cards[0].game_result, mgr->cards[1].game_result);

        if (mgr->chan == -1 || mgr->cards[mgr->chan].game_result != mCD_TRANS_ERR_NONE) {
            res = mCD_RESULT_ERROR;
        } else {
            res = mCD_RESULT_SUCCESS;
        }
    }

    return res;
}

static int mCD_check_noLand_file(mCD_LandProtectCode_c* protect_code, u8* data, s32 chan) {
    int res = FALSE;

    if (data != NULL) {
        s32 result;

        if (mCD_load_file(data, mCD_FILE_SAVE_MISC, chan, &result) == mCD_RESULT_SUCCESS) {
            if (mCD_CheckProtectCode((mCD_LandProtectCode_c*)(data + mCD_SAVE_DATA_OFS)) == TRUE) {
                if (protect_code != NULL) {
                    bcopy((mCD_LandProtectCode_c*)(data + mCD_SAVE_DATA_OFS), protect_code,
                          sizeof(mCD_LandProtectCode_c));
                }

                res = TRUE;
            }
        }
    }

    return TRUE;
}

static int mCD_GetNoLandSlot_bg(mCD_memMgr_c* mgr) {
    s32 chan = (mgr->_0010 & 1) ? 1 : 0;
    mCD_memMgr_card_info_c* card_info = &mgr->cards[chan];
    int res = mCD_RESULT_BUSY;

    if (card_info->workArea != NULL) {
        static mCD_LandProtectCode_c noLand_code;

        if (mCD_check_noLand_file(&noLand_code, mgr->workArea, chan) == TRUE &&
            mCD_CompNoLandCode(&noLand_code, &l_keep_noLandCode) == TRUE) {
            mgr->chan = chan;
            res = mCD_RESULT_SUCCESS;
        } else {
            mgr->_0010 |= 1 << chan;
            card_info->game_result = mCD_TRANS_ERR_INVALID_NOLAND_CODE;
        }
    } else {
        mgr->_0010 |= 1 << chan;
        res = mCD_RESULT_ERROR;
    }

    if (card_info->game_result != mCD_TRANS_ERR_INVALID_NOLAND_CODE) {
        card_info->game_result = mCD_TransErrorCode(card_info->result);
    }

    if (mgr->_0010 == 0b11 && res == mCD_RESULT_BUSY) {
        mCD_GetHighPriority_common(mgr, mgr->cards[0].game_result, mgr->cards[1].game_result);
        res = mCD_RESULT_ERROR;
    }

    return res;
}

static int mCD_CheckThisLandSlot(s32* result, s32* game_result, s32 chan, Save_t* buf_save) {
    int res = mCD_RESULT_ERROR;

    if (buf_save != NULL) {
        int i;

        for (i = 0; i < 2; i++) {
            int read_res;

            *result = CARD_RESULT_NOCARD;
            read_res = mCD_read_fg(buf_save, l_mCD_land_file_name, CARD_WORKAREA_SIZE,
                                   mCD_get_offset(mCD_FILE_SAVE_MAIN + i), chan, result);
            if (read_res == mCD_RESULT_SUCCESS) {
                if (mFRm_CheckSaveData_common(&buf_save->save_check, buf_save->land_info.id) == TRUE) {
                    if (mLd_CheckThisLand(buf_save->land_info.name, buf_save->land_info.id) == TRUE) {
                        *game_result = mCD_TRANS_ERR_NONE;
                        res = mCD_RESULT_SUCCESS;
                        break;
                    } else if (mLd_CheckId(buf_save->land_info.id) == TRUE) {
                        *game_result = mCD_TRANS_ERR_OTHER_TOWN;
                        res = mCD_RESULT_BUSY;
                        break;
                    } else {
                        *game_result = mCD_TRANS_ERR_TOWN_INVALID;
                        res = mCD_RESULT_BUSY;
                    }
                } else {
                    *game_result = mCD_TRANS_ERR_TOWN_INVALID;
                    res = mCD_RESULT_BUSY;
                }
            }
        }
    }

    return res;
}

static void mCD_set_1byte(char* c, int idx) {
    *c = l_mcd_font_1[idx];
}

static void mCD_set_number_str(char* str, u8 num) {
    int n = num;
    int f = FALSE;
    int fig_table[3];
    int i;

    bzero(fig_table, sizeof(fig_table));
    for (i = 0; i < 3; i++) {
        fig_table[i] = n % 10;
        n /= 10;
    }

    for (i = 1; i < 3; i++) {
        if (fig_table[i] > 0 || f == TRUE) {
            *str++ = fig_table[i] + '0';
            f = TRUE;
        }
    }

    *str = fig_table[0] + '0';
}

static void mCD_get_land_comment1(char* comment1, u8* town_name) {
    int spaces = 0;
    u8* town_name_p;
    char* comment_p;
    int i;

    mem_clear((u8*)comment1, 32, 0);
    town_name_p = town_name;
    for (i = LAND_NAME_SIZE; i != 0; i--) {
        spaces += *town_name_p++ == CHAR_SPACE ? 1 : 0;
    }

    for (i = 0; i < LAND_NAME_SIZE - spaces; i++) {
        mCD_set_1byte(comment1, *town_name);
        comment1++;
        town_name++;
    }

    comment_p = l_comment_1_str;
    for (i = 0; i < 32; i++) {
        *comment1 = *comment_p;
        if (*comment_p == '\0') {
            break;
        }

        comment1++;
        comment_p++;
    }
}

static void mCD_get_passport_comment1(char* comment1, u8* player_name) {
    int spaces = 0;
    u8* player_name_p;
    char* comment_p;
    int i;

    mem_clear((u8*)comment1, 32, 0);
    player_name_p = player_name;
    for (i = LAND_NAME_SIZE; i != 0; i--) {
        spaces += *player_name_p++ == CHAR_SPACE ? 1 : 0;
    }

    for (i = 0; i < LAND_NAME_SIZE - spaces; i++) {
        mCD_set_1byte(comment1, *player_name);
        comment1++;
        player_name++;
    }

    comment_p = l_comment_player_1_str;
    for (i = 0; i < 32; i++) {
        *comment1 = *comment_p;
        if (*comment_p == '\0') {
            break;
        }

        comment1++;
        comment_p++;
    }
}

static void mCD_get_present_comment1(char* comment1, int num, char* src_comment, int src_len) {
    int i;

    mem_clear((u8*)comment1, 32, 0);
    for (i = 0; i < src_len; i++) {
        *comment1++ = *src_comment++;
    }

    if (num >= 0) {
        mCD_set_1byte(comment1, '0' + (num % 10));
    }
}

extern int mCD_card_format_bg(s32 chan) {
    s32 result;

    return mCD_format_bg(chan, &result);
}

static int mCD_get_this_land_slot_no(mCD_memMgr_c* mgr) {
    Save_t* buf_save = (Save_t*)mgr->workArea;
    mCD_memMgr_card_info_c* card_info = mgr->cards;
    int i;
    int res = mCD_RESULT_ERROR;

    for (i = 0; i < CARD_NUM_CHANS; i++) {
        card_info[i].result = CARD_RESULT_NOCARD;
        card_info[i].game_result = mCD_TRANS_ERR_NOCARD;
    }

    if (buf_save != NULL) {
        for (i = 0; i < CARD_NUM_CHANS; i++) {
            int t_res = mCD_CheckThisLandSlot(&card_info->result, &card_info->game_result, i, buf_save);

            if (t_res == mCD_RESULT_SUCCESS) {
                mgr->chan = i;
                card_info->game_result = mCD_TRANS_ERR_NONE;
                res = mCD_RESULT_SUCCESS;
                break;
            }

            if (card_info->game_result == mCD_TRANS_ERR_NOCARD || t_res == mCD_RESULT_ERROR) {
                card_info->game_result = mCD_TransErrorCode(card_info->result);
            }

            card_info++;
        }
    }

    return res;
}

static int mCD_get_this_land_slot_no_game_start(mCD_memMgr_c* mgr) {
    Save_t* buf_save = (Save_t*)mgr->workArea;
    mCD_memMgr_card_info_c* card_info = mgr->cards;
    int i;
    int res = mCD_RESULT_ERROR;

    for (i = 0; i < CARD_NUM_CHANS; i++) {
        card_info[i].result = CARD_RESULT_NOCARD;
        card_info[i].game_result = mCD_TRANS_ERR_NOCARD;
    }

    if (buf_save != NULL) {
        for (i = 0; i < CARD_NUM_CHANS; i++) {
            int t_res = mCD_CheckThisLandSlot(&card_info->result, &card_info->game_result, i, buf_save);

            if (t_res == mCD_RESULT_SUCCESS) {
                mgr->chan = i;
                res = mCD_RESULT_SUCCESS;
                break;
            }

            if (card_info->game_result == mCD_TRANS_ERR_NOCARD || t_res == mCD_RESULT_ERROR) {
                card_info->game_result = mCD_TransErrorCode(card_info->result);
            } else {
                card_info->game_result = mCD_TRANS_ERR_7;
                mgr->chan = i;
            }

            card_info++;
        }
    }

    return res;
}

static int mCD_get_this_land_slot_no_nes(mCD_memMgr_c* mgr) {
    Save_t* buf_save = (Save_t*)mgr->workArea;
    mCD_memMgr_card_info_c* card_info = mgr->cards;
    int i;
    int res = mCD_RESULT_ERROR;

    for (i = 0; i < CARD_NUM_CHANS; i++) {
        card_info[i].result = CARD_RESULT_NOCARD;
        card_info[i].game_result = mCD_TRANS_ERR_NOCARD;
    }

    if (buf_save != NULL) {
        for (i = 0; i < CARD_NUM_CHANS; i++) {
            int t_res = mCD_CheckThisLandSlot(&card_info->result, &card_info->game_result, i, buf_save);

            if (t_res == mCD_RESULT_SUCCESS) {
                mgr->chan = i;
                res = mCD_RESULT_SUCCESS;
                break;
            }

            if (mCD_check_sector_size(mCD_MEMCARD_SECTORSIZE, i) == FALSE) {
                card_info->game_result = mCD_TRANS_ERR_NOT_MEMCARD;
                mgr->chan = i;
            } else if (card_info->game_result == mCD_TRANS_ERR_NOCARD || t_res == mCD_RESULT_ERROR) {
                card_info->game_result = mCD_TransErrorCode(card_info->result);
            }

            card_info++;
        }
    }

    if (res == mCD_RESULT_ERROR) {
        mCD_GetHighPriority_common(mgr, mgr->cards[0].game_result, mgr->cards[1].game_result);
    }

    return res;
}

static int mCD_check_copyProtect(Save_t* buf_save, s32 chan) {
    int res = FALSE;

    if (chan != -1 && buf_save != NULL) {
        int i;

        for (i = 0; i < 2; i++) {
            s32 result;
            int t_res = mCD_read_fg(buf_save, l_mCD_land_file_name, 0x200, mCD_get_offset(mCD_FILE_SAVE_MAIN + i), chan,
                                    &result);

            if (t_res == mCD_RESULT_SUCCESS && buf_save->copy_protect == Common_Get(copy_protect)) {
                res = TRUE;
                break;
            }
        }
    }

    return res;
}

static int mCD_bg_get_area_common(mCD_memMgr_c* mgr, mCD_memMgr_fileInfo_c* fileInfo, int file_idx, int sound_idx) {
    static u8 sound_table[mCD_HOME_SFX_NUM] = { NA_SE_ITEM_HORIDASHI, NA_SE_TOWN_DELETE };
    mCD_memMgr_card_info_c* card_info = mgr->cards;
    int stages_compl = 0;
    int res = mCD_RESULT_BUSY;

    /* Start playing sound effect if necessary */
    if (mgr->_017C == 0 && sound_idx >= 0 && sound_idx < mCD_HOME_SFX_NUM) {
        sAdo_SysLevStart(sound_table[sound_idx]);
    }

    if (mgr->_017C < 100) {
        int i;

        for (i = 0; i < CARD_NUM_CHANS; i++) {
            if (card_info->workArea == NULL) {
                card_info->workArea = mCD_malloc_32(CARD_WORKAREA_SIZE);
            }

            if (card_info->workArea != NULL) {
                stages_compl++;
            }

            card_info++;
        }

        if (mgr->workArea == NULL) {
            mgr->workArea_size = mCD_get_size(file_idx);
            mgr->workArea = mCD_malloc_32(mgr->workArea_size);
        }

        if (mgr->workArea != NULL) {
            stages_compl++;
        }

        if (stages_compl == 3) {
            fileInfo->proc++;
            res = mCD_RESULT_SUCCESS;
        } else {
            mgr->_017C++;
        }
    } else {
        /* Unable to allocate work area buffers */
        mCD_OnErrInfo(mCD_ERROR_AREA);
        res = mCD_RESULT_ERROR;
    }

    return res;
}

static int mCD_SaveHome_bg_get_area(mCD_memMgr_c* mgr, mCD_memMgr_fileInfo_c* fileInfo) {
    SoftResetEnable = FALSE;
    return mCD_bg_get_area_common(mgr, fileInfo, mCD_FILE_SAVE_MAIN, mCD_HOME_SFX_NORMAL);
}

static int mCD_SaveHome_bg_erase_dummy(mCD_memMgr_c* mgr, mCD_memMgr_fileInfo_c* fileInfo) {
    mCD_memMgr_card_info_c* card_info = mgr->cards;
    int i;

    for (i = 0; i < CARD_NUM_CHANS; i++) {
        s32 result;

        mCD_erase_file_fg(l_mCD_land_file_name_dummy, i, &result, card_info->workArea);
        card_info++;
    }

    return mCD_RESULT_SUCCESS;
}

static int mCD_send_present(PresentSaveFile_c* present_file, int length) {
    int res = FALSE;
    Mail_c* letter = present_file->save.letters;
    u32 player_no = Common_Get(player_no);
    u16 checksum = mFRm_ReturnCheckSum((u16*)present_file, length);

    if (checksum == 0 && present_file->save.present_count <= mCD_PRESENT_MAX) {
        int i;

        for (i = 0; i < mCD_PRESENT_MAX; i++) {
            if (mPr_NullCheckPersonalID(&letter->header.recipient.personalID) == TRUE) {
                Private_c* priv;
                mHm_hs_c* house;
                int free_idx;

                if (player_no >= PLAYER_NUM) {
                    break;
                }

                priv = Now_Private;
                if (priv == NULL) {
                    break;
                }

                house = &Save_Get(homes[mHS_get_arrange_idx(player_no)]);
                if (mPr_CheckCmpPersonalID(&priv->player_ID, &house->ownerID) != TRUE) {
                    break;
                }

                free_idx = mMl_chk_mail_free_space(house->mailbox, HOME_MAILBOX_SIZE);
                if (free_idx == -1) {
                    break;
                }

                letter->content.font = mMl_FONT_0;
                mPr_CopyPersonalID(&letter->header.recipient.personalID, &priv->player_ID);
                letter->header.recipient.type = mMl_NAME_TYPE_PLAYER;
                mMl_copy_mail(&house->mailbox[free_idx], letter);

                res = TRUE;
                present_file->save.present_count--;
                if (present_file->save.present_count <= 0) {
                    break;
                }

                letter++; /* send next letter */
            }
        }
    }

    return res;
}

static int mCD_SaveHome_bg_check_slot(mCD_memMgr_c* mgr, mCD_memMgr_fileInfo_c* fileInfo) {
    int res = mCD_RESULT_BUSY;

    if (mgr->cards[0].workArea != NULL && mgr->cards[1].workArea != NULL && mgr->workArea != NULL) {
        if (Save_Get(save_exist) == FALSE) {
            if (mCD_CheckInitProtectCode(&l_keep_noLandCode) == FALSE &&
                mCD_CheckProtectCode(&l_keep_noLandCode) == TRUE) {
                int slot_res = mCD_GetNoLandSlot_bg(mgr);

                if (slot_res == mCD_RESULT_SUCCESS) {
                    mgr->land_saved = Save_Get(save_exist);
                    mgr->copy_protect = Common_Get(copy_protect);
                    mgr->_0010 = 0b00;
                    fileInfo->proc++;
                    res = mCD_RESULT_SUCCESS;
                } else if (slot_res != mCD_RESULT_BUSY) {
                    res = mCD_RESULT_ERROR;
                }
            } else {
                int space_res = mCD_GetSpaceSlot_bg2(mgr, mCD_LAND_SAVE_SIZE);

                if (space_res == TRUE) {
                    if (mgr->chan != -1) {
                        fileInfo->proc++;
                        mgr->land_saved = Save_Get(save_exist);
                        mgr->copy_protect = Common_Get(copy_protect);
                        mgr->_0010 = 0b00;
                        res = mCD_RESULT_SUCCESS;
                    } else {
                        res = mCD_RESULT_ERROR;
                    }
                } else if (space_res != mCD_RESULT_BUSY) {
                    mgr->_0010 = 0b00;
                    res = mCD_RESULT_ERROR;
                }
            }
        } else {
            int land_slot_res = mCD_get_this_land_slot_no_nes(mgr);

            if (land_slot_res == mCD_RESULT_SUCCESS) {
                int chan = mgr->chan;

                if (chan != -1) {
                    if (mCD_check_copyProtect((Save_t*)mgr->workArea, chan) == TRUE) {
                        fileInfo->proc++;
                        mgr->land_saved = Save_Get(save_exist);
                        mgr->copy_protect = Common_Get(copy_protect);
                        res = mCD_RESULT_SUCCESS;
                    } else {
                        mgr->cards[chan].game_result = mCD_TRANS_ERR_7;
                        mgr->land_saved = Save_Get(save_exist);
                        mgr->copy_protect = Common_Get(copy_protect);
                        res = mCD_RESULT_ERROR;
                    }
                }
            } else if (land_slot_res != mCD_RESULT_BUSY) {
                res = mCD_RESULT_ERROR;
            }
        }
    } else {
        res = mCD_RESULT_ERROR;
    }

    return res;
}

static int mCD_CheckPresentFile(char* filename, s32* fileNo, s32 chan, s32* result, void* workArea) {
    int res = FALSE;

    if (mCD_check_card(result, mCD_MEMCARD_SECTORSIZE, chan) == mCD_RESULT_SUCCESS) {
        *result = CARDMount(chan, workArea, NULL);
        if (*result == CARD_RESULT_READY || *result == CARD_RESULT_BROKEN) {
            CARDStat stat;
            int file;

            *result = CARDCheck(chan);
            if (*result == CARD_RESULT_READY) {
                for (file = *fileNo; file < 32; file++) {
                    *result = CARDGetStatus(chan, file, &stat);
                    if (*result == CARD_RESULT_READY && mCD_CheckPresentFileStatus(&stat) == TRUE) {
                        bcopy(stat.fileName, filename, CARD_FILENAME_MAX);
                        res = TRUE;
                        break;
                    }
                }
            }

            /* @BUG - if CARDCheck fails, file will be undefined */
            *fileNo = file;
            CARDUnmount(chan);
        } else {
            if (*result == CARD_RESULT_ENCODING) {
                CARDUnmount(chan);
            }

            *fileNo = 32;
        }
    } else {
        *fileNo = 32;
    }

    return res;
}

static u8* mCD_set_bti_data(u8* data, int res_fileNo, int tlut_size, int count, int pal_size);

static int mCD_SaveHome_bg_read_send_present(mCD_memMgr_c* mgr, mCD_memMgr_fileInfo_c* fileInfo) {
    static int icon_fileNo[mCD_PRESENT_TYPE_NUM] = { RESOURCE_TEGAMI, RESOURCE_TEGAMI2 };
    static const char* comment_p_table[mCD_PRESENT_TYPE_NUM] = { l_comment_present_1_str, l_comment_gift_1_str };
    static int comment_len_table[mCD_PRESENT_TYPE_NUM] = { 14, 13 };
    PresentSaveFile_c* present_buf = (PresentSaveFile_c*)mgr->workArea;
    int type = mCD_PRESENT_TYPE_BONUS;

    if (mgr->cards[0].workArea != NULL && mgr->cards[1].workArea != NULL && present_buf != NULL) {
        mCD_memMgr_card_info_c* card_info;

        mgr->chan = -1;
        card_info = &mgr->cards[fileInfo->chan];

        /* Get info about the present save file */
        if (mCD_CheckPresentFile(mgr->filename, &fileInfo->fileNo, fileInfo->chan, &card_info->result,
                                 card_info->workArea) == TRUE) {
            if (mCD_read_fg(present_buf, mgr->filename, l_mcd_file_table[mCD_FILE_PRESENT].entrysize,
                            mCD_get_offset(mCD_FILE_PRESENT), fileInfo->chan,
                            &card_info->result) == mCD_RESULT_SUCCESS) {
                /* Determine if this is a bonus gift file or a 'gift' present file */
                if (mCD_CheckFilename(present_buf->comment + 32, l_comment_gift_1_str, 13) == TRUE) {
                    type = mCD_PRESENT_TYPE_GIFT;
                }

                /* Try sending the presents */
                if (mCD_send_present(present_buf, l_mcd_file_table[mCD_FILE_PRESENT].entrysize) == TRUE) {
                    mgr->loaded_file_type = mCD_FILE_PRESENT;
                    mgr->workArea_size = mCD_get_size(mgr->loaded_file_type);
                    mgr->chan = fileInfo->chan;
                    mCD_get_present_comment1(present_buf->comment + 32, present_buf->save.present_count,
                                             comment_p_table[type], comment_len_table[type]);
                    mCD_set_bti_data(present_buf->icon, icon_fileNo[type], 0x400, 1, 0x200);
                    present_buf->save.checksum =
                        mFRm_GetFlatCheckSum((u16*)present_buf, mgr->workArea_size, present_buf->save.checksum);
                    fileInfo->proc++;
                } else if (present_buf->save.present_count == 0) {
                    fileInfo->proc++;
                    mgr->chan = fileInfo->chan;
                } else {
                    mgr->chan = -1;
                    fileInfo->chan = CARD_NUM_CHANS;
                }
            } else if (card_info->result == CARD_RESULT_WRONGDEVICE || card_info->result == CARD_RESULT_NOCARD ||
                       card_info->result == CARD_RESULT_IOERROR || card_info->result == CARD_RESULT_BROKEN ||
                       card_info->result == CARD_RESULT_FATAL_ERROR || card_info->result == CARD_RESULT_LIMIT ||
                       card_info->result == card_info->result == CARD_RESULT_ENCODING) {
                fileInfo->fileNo = 32;
            }
        }

        if (mgr->chan == -1 || fileInfo->fileNo >= 32) {
            fileInfo->proc = mCD_SAVEHOME_BG_PROC_READ_SEND_PRESENT;
            fileInfo->fileNo = 0;
            fileInfo->chan++;
        }

        if (fileInfo->chan >= CARD_NUM_CHANS) {
            fileInfo->proc = mCD_SAVEHOME_BG_PROC_GET_SLOT;
            mgr->chan = -1;
        }

        return mCD_RESULT_SUCCESS;
    } else {
        return mCD_RESULT_ERROR;
    }
}

static int mCD_write_common(mCD_memMgr_c* mgr) {
    int res;

    if (mgr->workArea != NULL && mgr->chan != -1) {
        mCD_memMgr_card_info_c* card_info = &mgr->cards[mgr->chan];

        res = mCD_save_file(mgr->workArea, mgr->loaded_file_type, mgr->chan, &card_info->result);
        card_info->game_result = mCD_TransErrorCode(card_info->result);
    } else {
        res = mCD_RESULT_ERROR;
    }

    return res;
}

static int mCD_SaveHome_bg_write_present(mCD_memMgr_c* mgr, mCD_memMgr_fileInfo_c* fileInfo) {
    s32 chan = mgr->chan;

    if (chan != -1) {
        mCD_file_entry_c* file_entry = &l_mcd_file_table[mgr->loaded_file_type];
        int ofs = mCD_get_offset(mgr->loaded_file_type);
        mCD_memMgr_card_info_c* card_info = &mgr->cards[chan];
        int write_res = mCD_write_comp_bg(mgr->workArea, mgr->filename, mgr->workArea_size, file_entry->filesize, ofs,
                                          chan, &card_info->result);

        card_info->game_result = mCD_TransErrorCode(card_info->result);
        if (write_res != mCD_RESULT_BUSY) {
            if (card_info->result == CARD_RESULT_WRONGDEVICE || card_info->result == CARD_RESULT_NOCARD ||
                card_info->result == CARD_RESULT_IOERROR || card_info->result == CARD_RESULT_BROKEN ||
                card_info->result == CARD_RESULT_FATAL_ERROR || card_info->result == CARD_RESULT_LIMIT ||
                card_info->result == card_info->result == CARD_RESULT_ENCODING) {
                fileInfo->chan++;
                fileInfo->fileNo = 0;
            } else {
                fileInfo->fileNo++;
            }

            if (fileInfo->fileNo >= 32) {
                fileInfo->chan++;
            }

            if (fileInfo->chan >= CARD_NUM_CHANS) {
                fileInfo->proc++;
                mgr->loaded_file_type = mCD_FILE_SAVE_MAIN;
                mgr->chan = -1;
            } else {
                fileInfo->proc = mCD_SAVEHOME_BG_PROC_READ_SEND_PRESENT;
            }
        }

        card_info->game_result = mCD_TransErrorCode(card_info->result);
        return mCD_RESULT_SUCCESS;
    } else {
        return mCD_RESULT_ERROR;
    }
}

static int mCD_SaveHome_bg_get_slot(mCD_memMgr_c* mgr, mCD_memMgr_fileInfo_c* fileInfo) {
    int res = mCD_RESULT_BUSY;

    if (mgr->cards[0].workArea != NULL && mgr->cards[1].workArea != NULL && mgr->workArea != NULL) {
        if (Save_Get(save_exist) == FALSE) {
            if (mCD_CheckInitProtectCode(&l_keep_noLandCode) == FALSE &&
                mCD_CheckProtectCode(&l_keep_noLandCode) == TRUE) {
                int slot_res = mCD_GetNoLandSlot_bg(mgr);

                if (slot_res == mCD_RESULT_SUCCESS) {
                    mgr->land_saved = Save_Get(save_exist);
                    mgr->copy_protect = Common_Get(copy_protect);
                    mgr->_0010 = 0b00;
                    fileInfo->proc = mCD_SAVEHOME_BG_PROC_SET_FILE_PERMISSION;
                    res = mCD_RESULT_SUCCESS;
                } else if (slot_res != mCD_RESULT_BUSY) {
                    res = mCD_RESULT_ERROR;
                }
            } else {
                int space_res = mCD_GetSpaceSlot_bg2(mgr, mCD_LAND_SAVE_SIZE);

                if (space_res == TRUE) {
                    if (mgr->chan != -1) {
                        fileInfo->proc++;
                        mgr->land_saved = Save_Get(save_exist);
                        mgr->copy_protect = Common_Get(copy_protect);
                        mgr->_0010 = 0b00;
                        res = mCD_RESULT_SUCCESS;
                    } else {
                        res = mCD_RESULT_ERROR;
                    }
                } else if (space_res != mCD_RESULT_BUSY) {
                    mgr->_0010 = 0b00;
                    res = mCD_RESULT_ERROR;
                }
            }
        } else {
            int land_slot_res = mCD_get_this_land_slot_no_nes(mgr);

            if (land_slot_res == mCD_RESULT_SUCCESS) {
                int chan = mgr->chan;

                if (chan != -1) {
                    if (mCD_check_copyProtect((Save_t*)mgr->workArea, chan) == TRUE) {
                        fileInfo->proc = mCD_SAVEHOME_BG_PROC_CHECK_REPAIR_LAND;
                        mgr->land_saved = Save_Get(save_exist);
                        mgr->copy_protect = Common_Get(copy_protect);
                        res = mCD_RESULT_SUCCESS;
                    } else {
                        mgr->cards[chan].game_result = mCD_TRANS_ERR_7;
                        mgr->land_saved = Save_Get(save_exist);
                        mgr->copy_protect = Common_Get(copy_protect);
                        res = mCD_RESULT_ERROR;
                    }
                }
            } else if (land_slot_res != mCD_RESULT_BUSY) {
                res = mCD_RESULT_ERROR;
            }
        }
    } else {
        res = mCD_RESULT_ERROR;
    }

    return res;
}

static int mCD_SaveHome_bg_create_file(mCD_memMgr_c* mgr, mCD_memMgr_fileInfo_c* fileInfo) {
    s32 chan = mgr->chan;
    int res;

    if (chan != -1) {
        mCD_memMgr_card_info_c* card_info = &mgr->cards[chan];

        mgr->_019C = 1;
        res = mCD_create_file_bg(l_mCD_land_file_name_dummy, CARD_ATTR_NO_MOVE | CARD_ATTR_NO_COPY,
                                 l_mcd_file_table[mCD_FILE_SAVE_MISC].filesize, chan, &card_info->result,
                                 &card_info->fileNo);
        card_info->game_result = mCD_TransErrorCode(card_info->result);
        if (res == mCD_RESULT_SUCCESS) {
            fileInfo->proc = mCD_SAVEHOME_BG_PROC_SET_FILE_PERMISSION;
        }
    } else {
        res = mCD_RESULT_ERROR;
    }

    return res;
}

static int mCD_check_broken_land(mCD_memMgr_c* mgr) {
    s32 chan = mgr->chan;
    Save_t* save = (Save_t*)mgr;
    int res = FALSE;

    if (chan != -1 && save != NULL) {
        int size = mCD_get_size(mCD_FILE_SAVE_MAIN);
        int ok_save_idx = -1;
        int i;

        for (i = 0; i < 2; i++) {
            s32 result;

            if (mCD_load_file(save, mCD_FILE_SAVE_MAIN + i, chan, &result) != mCD_RESULT_SUCCESS) {
                break;
            }

            if (mFRm_ReturnCheckSum((u16*)save, size) == 0 &&
                mFRm_CheckSaveData_common(&save->save_check, save->land_info.id)) {
                ok_save_idx = i;
            } else {
                mgr->broken_file_idx = i;
            }
        }

        if (ok_save_idx != -1 && mgr->broken_file_idx != -1 && ok_save_idx != mgr->broken_file_idx) {
            res = TRUE;
        }
    }

    return res;
}

static int mCD_repair_load_land(mCD_memMgr_c* mgr) {
    s32 chan = mgr->chan;
    Save_t* save = (Save_t*)mgr;
    int res = FALSE;

    if (chan != -1 && save != NULL && mgr->broken_file_idx != -1) {
        s32 result;

        if (mCD_load_file(save, mCD_FILE_SAVE_MAIN + ((~mgr->broken_file_idx) & 1), chan, &result) ==
            mCD_RESULT_SUCCESS) {
            int size = mCD_get_size(mCD_FILE_SAVE_MAIN);

            if (mFRm_ReturnCheckSum((u16*)save, size) == 0 &&
                mFRm_CheckSaveData_common(&save->save_check, save->land_info.id)) {
                res = TRUE;
            }
        }
    }

    return res;
}

static int mCD_repair_land(mCD_memMgr_c* mgr) {
    if (mgr->chan != -1 && mgr->workArea != NULL && mgr->broken_file_idx != -1) {
        s32 result;

        /* The loaded file is the 'good' version of the save so we just write it back to the broken save */
        return mCD_save_file(mgr->workArea, mCD_FILE_SAVE_MAIN + mgr->broken_file_idx, mgr->chan, &result);
    } else {
        return mCD_RESULT_ERROR;
    }
}

static int mCD_SaveHome_bg_check_repair_land(mCD_memMgr_c* mgr, mCD_memMgr_fileInfo_c* fileInfo) {
    if (mCD_check_broken_land(mgr) == TRUE) {
        if (mCD_repair_load_land(mgr) == TRUE) {
            fileInfo->proc++;
        } else {
            fileInfo->proc = mCD_SAVEHOME_BG_PROC_SET_FILE_PERMISSION;
        }
    } else {
        fileInfo->proc = mCD_SAVEHOME_BG_PROC_SET_FILE_PERMISSION;
    }

    return mCD_RESULT_SUCCESS;
}

static int mCD_SaveHome_bg_repair_land(mCD_memMgr_c* mgr, mCD_memMgr_fileInfo_c* fileInfo) {
    int res = mCD_RESULT_BUSY;

    if (mCD_repair_land(mgr)) {
        fileInfo->proc++;
        res = mCD_RESULT_SUCCESS;
    }

    return res;
}

static int mCD_get_status_common(mCD_memMgr_c* mgr, mCD_memMgr_fileInfo_c* fileInfo, const char* filename,
                                 u32 comment_addr, u32 icon_addr, int icon_fmt, int icon_speed, int icon_frames,
                                 int banner_fmt) {
    s32 chan = mgr->chan;
    int res;

    if (chan != -1) {
        mCD_memMgr_card_info_c* card_info = &mgr->cards[chan];

        res = mCD_get_file_status_bg(&card_info->stat, filename, chan, &card_info->result);
        card_info->game_result = mCD_TransErrorCode(card_info->result);

        if (res == mCD_RESULT_SUCCESS) {
            int i;

            card_info->stat.commentAddr = comment_addr;
            card_info->stat.iconAddr = icon_addr;
            for (i = 0; i < icon_frames; i++) {
                card_info->stat.iconFormat =
                    (card_info->stat.iconFormat & ~(CARD_STAT_ICON_MASK << (i * 2))) | (icon_fmt << (i * 2));
                card_info->stat.iconSpeed =
                    (card_info->stat.iconFormat & ~(CARD_STAT_SPEED_MASK << (i * 2))) | (icon_speed << (i * 2));
            }

            card_info->stat.bannerFormat = (card_info->stat.bannerFormat & ~CARD_STAT_BANNER_MASK) | banner_fmt;
            fileInfo->proc++;
        }
    } else {
        res = mCD_RESULT_ERROR;
    }

    return res;
}

static int mCD_SaveHome_bg_set_file_permission(mCD_memMgr_c* mgr, mCD_memMgr_fileInfo_c* fileInfo) {
    s32 chan = mgr->chan;
    int res = mCD_RESULT_BUSY;

    if (chan != -1) {
        mCD_memMgr_card_info_c* card_info = &mgr->cards[chan];
        const char* filename =
            mgr->_019C == 1 ? l_mCD_land_file_name_dummy : l_mcd_file_table[mCD_FILE_SAVE_MISC].filename;

        res = mCD_set_file_permission_bg(filename, CARD_ATTR_NO_MOVE | CARD_ATTR_NO_COPY, chan, &card_info->result,
                                         &card_info->fileNo);
        card_info->game_result = mCD_TransErrorCode(card_info->result);

        if (res == mCD_RESULT_SUCCESS) {
            fileInfo->proc++;
        }
    } else {
        res = mCD_RESULT_ERROR;
    }

    return res;
}

static void mCD_ClearResetCode(void) {
    bzero(&Now_Private->reset_code, sizeof(Now_Private->reset_code));
}

static int mCD_CheckResetCode(Private_c* priv) {
    int res = TRUE;

    if ((priv->state_flags & mPr_FLAG_BIRTHDAY_ACTIVE) != 0) {
        priv->reset_code = 0;
    }

    if (priv->reset_code != 0) {
        res = FALSE;
    }

    return res;
}

static void mCD_SetResetCode(Private_c* priv) {
    priv->reset_code = (u32)RANDOM(USHT_MAX_S);
    priv->reset_code++;
}

static void mCD_SetResetInfo(Private_c* priv) {
    Common_Set(reset_flag, FALSE);
    if (mCD_CheckResetCode(priv) == FALSE) {

        /* No reset penalty if zurumode 2 is enabled */
        if (!ZURUMODE2_ENABLED()) {
            Common_Set(reset_flag, TRUE);
        }

        priv->reset_count++;
    }
}

static u16 mCD_get_land_copyProtect(void) {
    return 1 + (u16)RANDOM(0xFFF0);
}

static int mCD_SaveHome_bg_set_data(mCD_memMgr_c* mgr, mCD_memMgr_fileInfo_c* fileInfo) {
    Save_t* save = (Save_t*)mgr->workArea;
    int _04 = fileInfo->_04;

    if (save != NULL) {
        Private_c* priv;

        bzero(save, mgr->workArea_size);
        priv = Now_Private;
        mCkRh_SavePlayTime(Common_Get(player_no));

        if (_04 == 0) {
            mCD_ClearResetCode();
            mAGrw_ClearMoneyStoneShineGround();
        } else if (mCD_CheckResetCode(priv) == TRUE) {
            mCD_SetResetCode(priv);
        }

        if (_04 == 0 && priv != NULL) {
            int i;

            /* Clear all Wisp spirits in the player's inventory */
            for (i = 0; i < mPr_POCKETS_SLOT_COUNT; i++) {
                if (ITEM_IS_WISP(priv->inventory.pockets[i])) {
                    mPr_SetPossessionItem(priv, i, EMPTY_NO, mPr_ITEM_COND_NORMAL);
                }
            }
        }

        Save_Set(save_exist, TRUE);
        Save_Set(copy_protect, mCD_get_land_copyProtect());
        Common_Set(copy_protect, Save_Get(copy_protect));
        Save_Set(travel_hard_time, lbRTC_HardTime());
        bcopy(&Common_Get(save).save, save, sizeof(Save_t));
        save->save_check.version = mFRm_VERSION;
        mFRm_SetSaveCheckData(&save->save_check);
        save->save_check.checksum = mFRm_GetFlatCheckSum((u16*)save, sizeof(Save), save->save_check.checksum);
        mgr->loaded_file_type = mCD_FILE_SAVE_MAIN;
        mgr->workArea_size = mCD_get_size(mgr->loaded_file_type);
        fileInfo->proc++;
        return mCD_RESULT_SUCCESS;
    } else {
        return mCD_RESULT_ERROR;
    }
}

static int mCD_SaveHome_bg_write_main_2(mCD_memMgr_c* mgr, mCD_memMgr_fileInfo_c* fileInfo) {
    void* workArea = mgr->workArea;
    s32 chan = mgr->chan;
    mCD_file_entry_c* file_entry = &l_mcd_file_table[mCD_FILE_SAVE_MAIN];
    const char* filename = mgr->_019C == 1 ? l_mCD_land_file_name_dummy : file_entry->filename;
    int res;

    if (workArea != NULL && chan != -1) {
        mCD_memMgr_card_info_c* card_info;
        int ofs;

        mgr->loaded_file_type = mCD_FILE_SAVE_MAIN;
        ofs = mCD_get_offset(mgr->loaded_file_type);
        card_info = &mgr->cards[chan];
        res = mCD_write_comp_bg(workArea, filename, mgr->workArea_size, file_entry->filesize, ofs, chan,
                                &card_info->result);
        card_info->game_result = mCD_TransErrorCode(card_info->result);

        if (res == mCD_RESULT_SUCCESS) {
            mgr->loaded_file_type = mCD_FILE_SAVE_MAIN_BAK;
            fileInfo->proc++;
        } else if (res != mCD_RESULT_BUSY) {
            res = mCD_RESULT_ERROR;
        }
    } else {
        res = mCD_RESULT_ERROR;
    }

    return res;
}

static int mCD_SaveHome_bg_write_bk(mCD_memMgr_c* mgr, mCD_memMgr_fileInfo_c* fileInfo) {
    void* workArea = mgr->workArea;
    s32 chan = mgr->chan;
    mCD_file_entry_c* file_entry = &l_mcd_file_table[mCD_FILE_SAVE_MAIN_BAK];
    const char* filename = mgr->_019C == 1 ? l_mCD_land_file_name_dummy : file_entry->filename;
    Private_c* priv = Now_Private;
    int res;

    if (workArea != NULL && chan != -1) {
        mCD_memMgr_card_info_c* card_info;
        int ofs;

        mgr->loaded_file_type = mCD_FILE_SAVE_MAIN_BAK;
        ofs = mCD_get_offset(mgr->loaded_file_type);
        card_info = &mgr->cards[chan];
        res = mCD_write_comp_bg(workArea, filename, mgr->workArea_size, file_entry->filesize, ofs, chan,
                                &card_info->result);
        card_info->game_result = mCD_TransErrorCode(card_info->result);

        if (res == mCD_RESULT_SUCCESS) {
            if (fileInfo->_04 == 1) {
                if (Common_Get(player_decoy_flag) == TRUE && priv != NULL) {
                    priv->exists = TRUE;
                }
            } else {
                Common_Set(player_decoy_flag, FALSE);
            }

            mgr->loaded_file_type = mCD_FILE_SAVE_MAIN_BAK;
            fileInfo->proc++;
        } else if (res != mCD_RESULT_BUSY) {
            res = mCD_RESULT_ERROR;
        }
    } else {
        res = mCD_RESULT_ERROR;
    }

    return res;
}
