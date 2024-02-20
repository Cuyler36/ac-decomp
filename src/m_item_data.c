#include "m_item_data.h"

#include "_mem.h"
#include "m_huusui_room.h"
#include "m_mark_room.h"
#include "m_font.h"

extern const mID_data_c item_data_table[];

extern const mID_data_c* const mID_GetItemDataFromIdx(int idx) {
    if (idx >= mID_ITEM_NUM) {
        return NULL;
    }

    return &item_data_table[idx];
}

extern const mID_data_c* const mID_GetItemDataFromId(mActor_name_t id) {
    int i;

    for (i = 0; i < mID_ITEM_NUM; i++) {
        if (item_data_table[i].id == id) {
            return &item_data_table[i];
        }
    }

    return NULL;
}

extern const u8* mID_GetName(mActor_name_t id) {
    const mID_data_c* const data = mID_GetItemDataFromId(id);

    if (data != NULL) {
        return data->name;
    }

    return NULL;
}

extern BOOL mID_CopyName(mActor_name_t id, u8* buf) {
    const u8* name = mID_GetName(id);

    if (name != NULL) {
        memcpy(buf, name, mIN_ITEM_NAME_LEN);
        return TRUE;
    } else {
        memset(buf, mIN_ITEM_NAME_LEN, CHAR_SPACE);
        return FALSE;
    }
}

extern BOOL mID_GetLucky(mActor_name_t id) {
    const mID_data_c* const data = mID_GetItemDataFromId(id);

    if (data != NULL) {
        return data->is_lucky;
    }

    return FALSE;
}
extern BOOL mID_GetHasFace(mActor_name_t id) {
    const mID_data_c* const data = mID_GetItemDataFromId(id);

    if (data != NULL) {
        return data->has_face;
    }

    return FALSE;
}

extern int mID_GetPrimaryColor(mActor_name_t id) {
    const mID_data_c* const data = mID_GetItemDataFromId(id);

    if (data != NULL) {
        return data->primary_color;
    }

    return mID_COLOR_NONE;
}

extern int mID_GetSecondaryColor(mActor_name_t id) {
    const mID_data_c* const data = mID_GetItemDataFromId(id);

    if (data != NULL) {
        return data->secondary_color;
    }

    return mID_COLOR_NONE;
}

extern BOOL mID_HasColor(mActor_name_t id, int color) {
    const mID_data_c* const data = mID_GetItemDataFromId(id);

    if (data != NULL) {
        return data->primary_color == color || data->secondary_color == color;
    }

    return FALSE;
}

extern u32 mID_GetPrice(mActor_name_t id) {
    const mID_data_c* const data = mID_GetItemDataFromId(id);

    if (data != NULL) {
        return data->price;
    }

    return 0;
}
