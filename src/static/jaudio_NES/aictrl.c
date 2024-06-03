#include "aictrl.h"

#include "jaudio_NES/memory.h"

static u16* dac[3];
static ALHeap audio_hp;

extern void Jac_HeapSetup(void* pHeap, size_t size) {
}
