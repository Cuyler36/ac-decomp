#include "jaudio_NES/system.h"

#include "jaudio_NES/sub_sys.h"

BOOL AUDIO_SYSTEM_READY = FALSE;

extern void Nas_WaveDmaFrameWork(void) {
    u32 i;

    for (i = 0; i < AG.waveload_count; i++) {
        WaveLoad* waveload = &AG.waveload_list[i];

        if (waveload->time_to_live != 0) {
            waveload->time_to_live--;
            if (waveload->time_to_live == 0) {
                waveload->reuse_idx = AG.waveload_dma_queue0_wpos;
                AG.waveload_dma_queue0[AG.waveload_dma_queue0_wpos] = i;
                AG.waveload_dma_queue0_wpos++;
            }
        }
    }

    for (i = AG.waveload_count; i < AG.num_waveloads; i++) {
        WaveLoad* waveload = &AG.waveload_list[i];

        if (waveload->time_to_live != 0) {
            waveload->time_to_live--;
            if (waveload->time_to_live == 0) {
                waveload->reuse_idx = AG.waveload_dma_queue1_wpos;
                AG.waveload_dma_queue1[AG.waveload_dma_queue1_wpos] = i;
                AG.waveload_dma_queue1_wpos++;
            }
        }
    }

    AG._2648 = 0;
}

extern u8* Nas_WaveDmaCallBack(u32 device_addr, size_t size, s32 flag, u8* waveload_idx_ref, s32 medium) {
    s32 pad1;
    WaveLoad* waveload;
    s32 hasDma = FALSE;
    u32 dmaDevAddr;
    u32 pad2;
    u32 dmaIndex;
    u32 transfer;
    s32 bufferPos;
    u32 i;
}
