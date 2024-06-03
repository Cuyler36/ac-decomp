#include "aictrl.h"

#include "jaudio_NES/memory.h"
#include "os/OSAlloc.h"

static u16* dac[3];
static ALHeap audio_hp;
static BOOL audio_hp_exist = FALSE;

extern void Jac_HeapSetup(void* pHeap, s32 size) {
    if (pHeap != nullptr) {
        Nas_HeapInit(&audio_hp, (u8*)pHeap, size);
        audio_hp_exist = TRUE;
    } else {
        audio_hp_exist = FALSE;
    }
}

extern void* OSAlloc2
