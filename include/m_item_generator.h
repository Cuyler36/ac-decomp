#ifndef M_ITEM_GENERATOR_H
#define M_ITEM_GENERATOR_H

#include "types.h"
#include "m_item_data.h"

#ifdef __cplusplus
extern "C" {
#endif

#define mIG_SET_SIZE (sizeof(u32) * 8)

typedef struct item_generator_s {
    int set_count;
    u32 set[(mID_ITEM_NUM + (mIG_SET_SIZE - 1)) / mIG_SET_SIZE];
} mIG_ItemGenerator_c;

typedef BOOL (*mIG_PARAM_FUNC)(const mID_data_c* const data);

typedef struct item_generator_params_s {
    int param_count;
    mIG_PARAM_FUNC* param_table_p;
} mIG_Params_c;

#ifdef __cplusplus
}
#endif

#endif
