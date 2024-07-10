#ifndef GC_KEYBOARD_H
#define GC_KEYBOARD_H

#include "types.h"
#include "dolphin/os/OSSerial.h"
#include "dolphin/sipriv.h"

#ifdef __cplusplus
extern "C" {
#endif

extern s32 GCKB_Detect(void);
extern BOOL GCKB_Init(s32 chan);
extern BOOL GCKB_ReadKeys(s32 chan, u8* pressed_keys);

#ifdef __cplusplus
}
#endif

#endif
