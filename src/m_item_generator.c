#include "m_item_generator.h"

#include "_mem.h"
#include "sys_math.h"
#include "dolphin/os.h"

extern void mIG_ItemGenerator_ct(mIG_ItemGenerator_c* generator) {
    memset(generator, 0, sizeof(generator));
}

extern void mIG_ItemGenerator_dt(mIG_ItemGenerator_c* generator) {
    // nothing
}

extern mActor_name_t mIG_ItemGenerator_GetItem(const mIG_ItemGenerator_c* generator) {
    if (generator->set_count > 0) {
        int selected = RANDOM(generator->set_count);
        int i, j;

        for (i = 0; i < sizeof(generator->set); i++) {
            u32 mask = 1;
            u32 set = generator->set[i];

            for (j = 0; j < 32; j++, mask <<= 1) {
                if ((set & mask) != 0) {
                    if (selected <= 0) {
                        const mID_data_c* const data = mID_GetItemDataFromIdx(i * 32 + j);

                        if (data != NULL) {
                            return data->id;
                        }

                        return EMPTY_NO;
                    }

                    selected--;
                }
            }
        }
    }

    return EMPTY_NO;
}

extern void mIG_ItemGenerator_AddAnyParams(mIG_ItemGenerator_c* generator, const mIG_Params_c* params) {
    int i, j, k;

    for (i = 0; i < sizeof(generator->set); i++) {
        u32 mask = 1;
        u32 set = generator->set[i];
        int set_count = 0;

        for (j = 0; j < 32; j++, mask <<= 1) {
            if ((set & mask) == 0) {
                const mID_data_c* const data = mID_GetItemDataFromIdx(i * 32 + j);

                for (k = 0; k < params->param_count; k++) {
                    if ((*params->param_table_p[k])(data) == TRUE) {
                        set |= mask;
                        set_count++;
                        break;
                    }
                }
            }
        }

        generator->set_count += set_count;
        generator->set[i] = set;
    }
}

extern void mIG_ItemGenerator_AddAllParams(mIG_ItemGenerator_c* generator, const mIG_Params_c* params) {
    int i, j, k;

    for (i = 0; i < sizeof(generator->set); i++) {
        u32 mask = 1;
        u32 set = generator->set[i];
        int set_count = 0;

        for (j = 0; j < 32; j++, mask <<= 1) {
            if ((set & mask) == 0) {
                const mID_data_c* const data = mID_GetItemDataFromIdx(i * 32 + j);

                for (k = 0; k < params->param_count; k++) {
                    if ((*params->param_table_p[k])(data) == FALSE) {
                        break;
                    }
                }

                if (k == params->param_count) {
                    set |= mask;
                    set_count++;
                }
            }
        }

        generator->set_count += set_count;
        generator->set[i] = set;
    }
}

extern void mIG_ItemGenerator_ClearAnyParams(mIG_ItemGenerator_c* generator, const mIG_Params_c* params) {
    int i, j, k;

    for (i = 0; i < sizeof(generator->set); i++) {
        u32 mask = 1;
        u32 set = generator->set[i];
        int set_count = 0;

        for (j = 0; j < 32; j++, mask <<= 1) {
            if ((set & mask) != 0) {
                const mID_data_c* const data = mID_GetItemDataFromIdx(i * 32 + j);

                for (k = 0; k < params->param_count; k++) {
                    if ((*params->param_table_p[k])(data) == TRUE) {
                        set &= ~mask;
                        set_count--;
                        break;
                    }
                }
            }
        }

        generator->set_count += set_count;
        generator->set[i] = set;
    }
}

extern void mIG_ItemGenerator_ClearAllParams(mIG_ItemGenerator_c* generator, const mIG_Params_c* params) {
    int i, j, k;

    for (i = 0; i < sizeof(generator->set); i++) {
        u32 mask = 1;
        u32 set = generator->set[i];
        int set_count = 0;

        for (j = 0; j < 32; j++, mask <<= 1) {
            if ((set & mask) != 0) {
                const mID_data_c* const data = mID_GetItemDataFromIdx(i * 32 + j);

                for (k = 0; k < params->param_count; k++) {
                    if ((*params->param_table_p[k])(data) == FALSE) {
                        break;
                    }
                }

                if (k == params->param_count) {
                    set &= ~mask;
                    set_count--;
                }
            }
        }

        generator->set_count += set_count;
        generator->set[i] = set;
    }
}

extern void mIG_ItemGenerator_AddItemsWithPriceBetween(mIG_ItemGenerator_c* generator, u32 min, u32 max) {
    int i, j;

    for (i = 0; i < sizeof(generator->set); i++) {
        u32 mask = 1;
        u32 set = generator->set[i];
        int set_count = 0;

        for (j = 0; j < 32; j++, mask <<= 1) {
            if ((set & mask) == 0) {
                const mID_data_c* const data = mID_GetItemDataFromIdx(i * 32 + j);

                if (data->can_sell && data->price >= min && data->price <= max) {
                    set |= mask;
                    set_count++;
                }
            }
        }

        generator->set_count += set_count;
        generator->set[i] = set;
    }
}

extern void mIG_ItemGenerator_ClearItemsWithPriceBetween(mIG_ItemGenerator_c* generator, u32 min, u32 max) {
    int i, j;

    for (i = 0; i < sizeof(generator->set); i++) {
        u32 mask = 1;
        u32 set = generator->set[i];
        int set_count = 0;

        for (j = 0; j < 32; j++, mask <<= 1) {
            if ((set & mask) != 0) {
                const mID_data_c* const data = mID_GetItemDataFromIdx(i * 32 + j);

                if (data->can_sell && data->price >= min && data->price <= max) {
                    set &= ~mask;
                    set_count--;
                }
            }
        }

        generator->set_count += set_count;
        generator->set[i] = set;
    }
}

extern void mIG_ItemGenerator_AddItemsWithPriceBelow(mIG_ItemGenerator_c* generator, u32 max) {
    if (max > 0) {
        mIG_ItemGenerator_AddItemsWithPriceBetween(generator, 0, max);
    }
}

extern void mIG_ItemGenerator_ClearItemsWithPriceBelow(mIG_ItemGenerator_c* generator, u32 max) {
    if (max > 0) {
        mIG_ItemGenerator_ClearItemsWithPriceBetween(generator, 0, max);
    }
}

extern void mIG_ItemGenerator_AddItemsWithPriceAbove(mIG_ItemGenerator_c* generator, u32 min) {
    mIG_ItemGenerator_AddItemsWithPriceBetween(generator, min, U32_MAX);
}

extern void mIG_ItemGenerator_ClearItemsWithPriceAbove(mIG_ItemGenerator_c* generator, u32 min) {
    mIG_ItemGenerator_ClearItemsWithPriceBetween(generator, min, U32_MAX);
}

extern void mIG_ItemGenerator_AddItemsInBirthGroup(mIG_ItemGenerator_c* generator, int birth_group) {
    int i, j;

    for (i = 0; i < sizeof(generator->set); i++) {
        u32 mask = 1;
        u32 set = generator->set[i];
        int set_count = 0;

        for (j = 0; j < 32; j++, mask <<= 1) {
            if ((set & mask) == 0) {
                const mID_data_c* const data = mID_GetItemDataFromIdx(i * 32 + j);

                if (data->birth_group == birth_group) {
                    set |= mask;
                    set_count++;
                }
            }
        }

        generator->set_count += set_count;
        generator->set[i] = set;
    }
}

extern void mIG_ItemGenerator_ClearItemsInBirthGroup(mIG_ItemGenerator_c* generator, int birth_group) {
    int i, j;

    for (i = 0; i < sizeof(generator->set); i++) {
        u32 mask = 1;
        u32 set = generator->set[i];
        int set_count = 0;

        for (j = 0; j < 32; j++, mask <<= 1) {
            if ((set & mask) != 0) {
                const mID_data_c* const data = mID_GetItemDataFromIdx(i * 32 + j);

                if (data->birth_group == birth_group) {
                    set &= ~mask;
                    set_count--;
                }
            }
        }

        generator->set_count += set_count;
        generator->set[i] = set;
    }
}

extern void mIG_ItemGenerator_Reset(mIG_ItemGenerator_c* generator) {
    generator->set_count = 0;
    memset(generator->set, 0, sizeof(generator->set));
}

extern void mIG_ItemGenerator_DebugPrintInfo(const mIG_ItemGenerator_c* generator) {
    u32 calc_set_count = 0;
    int i, j;

    OSReport("Item Generator - set_count=%u\n", generator->set_count);
    // Step through the entire generator item bitfield marking which items are set in groups of 32
    for (i = 0; i < sizeof(generator->set); i++) {
        u32 mask = 1;
        u32 set = generator->set[i];

        for (j = 0; j < 32; j++, mask <<= 1) {
            if ((set & mask) != 0) {
                calc_set_count++;
                OSReport("1");
            } else {
                OSReport("0");
            }
        }

        OSReport("\n");
    }
    OSReport("calc_set_count=%u | valid=%s\n", calc_set_count,
             (generator->set_count == calc_set_count ? "TRUE" : "FALSE"));
}
