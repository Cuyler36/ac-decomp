#include "ac_npc.h"

// Temporary file to allow for adding new villagers

// aNPC_get_draw_data_idx
extern int func_80544a1c(mActor_name_t npc_id) {
    mNpc_EventNpc_c* event_npc;
    mNpc_MaskNpc_c* mask_npc;
    int res = -1;

    switch (ITEM_NAME_GET_TYPE(npc_id)) {
        case NAME_TYPE_SPNPC:
            event_npc = mNpc_GetSameEventNpc(npc_id);
            if (event_npc != NULL) {
                if (ITEM_NAME_GET_TYPE(event_npc->texture_id) == NAME_TYPE_NPC) {
                    res = event_npc->texture_id - NPC_START;
                } else {
                    res = (event_npc->texture_id - SP_NPC_START) + ALL_NPC_NUM;
                }
            } else {
                mask_npc = mNpc_GetSameMaskNpc(npc_id);
                if (mask_npc != NULL) {
                    if (ITEM_NAME_GET_TYPE(mask_npc->npc_id) == NAME_TYPE_NPC) {
                        res = mask_npc->npc_id - NPC_START;
                    } else {
                        res = (mask_npc->npc_id - SP_NPC_START) + ALL_NPC_NUM;
                    }
                } else {
                    res = (npc_id - SP_NPC_START) + ALL_NPC_NUM;
                }
            }
            break;
        case NAME_TYPE_NPC:
            res = npc_id - NPC_START;
            break;
    }

    return res;
}
