#include "m_scene.h"

extern Actor_data TEST_FD_NPC_LAND_player_data[];
extern s16 TEST_FD_NPC_LAND_ctrl_actor_data[];
extern Actor_data TEST_FD_NPC_LAND_actor_data[];
extern s16 TEST_FD_NPC_LAND_object_bank[];

Scene_Word_u test_fd_npc_land_info[] = {
    { mSc_SCENE_DATA_TYPE_SOUND, 0, 0, 0, 0 },
    { mSc_SCENE_DATA_TYPE_PLAYER_PTR, 1, 0, 0, (u32)TEST_FD_NPC_LAND_player_data },
    { mSc_SCENE_DATA_TYPE_CTRL_ACTOR_PTR, 18, 0, 0, (u32)TEST_FD_NPC_LAND_ctrl_actor_data },
    { mSc_SCENE_DATA_TYPE_ACTOR_PTR, 2, 0, 0, (u32)TEST_FD_NPC_LAND_actor_data },
    { mSc_SCENE_DATA_TYPE_OBJECT_EXCHANGE_BANK_PTR, 2, 0, 0, (u32)TEST_FD_NPC_LAND_object_bank },
    { mSc_SCENE_DATA_TYPE_FIELD_CT, 0, 4, 0, (u32)(0x1C00 << 16) },
    { mSc_SCENE_DATA_TYPE_END, 0, 0, 0, 0 },
};

Actor_data TEST_FD_NPC_LAND_player_data[1] = {
    { mAc_PROFILE_PLAYER, { 2240, 0, 1600 }, { 0, 0, 0 }, 0 },
};

s16 TEST_FD_NPC_LAND_ctrl_actor_data[18] = {
    mAc_PROFILE_EFFECTBG,      mAc_PROFILE_BIRTH_CONTROL,  mAc_PROFILE_NPC,
    mAc_PROFILE_STRUCTURE,     mAc_PROFILE_INSECT,         mAc_PROFILE_TOOLS,
    mAc_PROFILE_HANDOVERITEM,  mAc_PROFILE_EFFECT_CONTROL, mAc_PROFILE_SHOP_LEVEL,
    mAc_PROFILE_QUEST_MANAGER, mAc_PROFILE_EVENT_MANAGER,  mAc_PROFILE_WEATHER,
    mAc_PROFILE_SET_MANAGER,   mAc_PROFILE_GYOEI,          mAc_PROFILE_SET_NPC_MANAGER,
    mAc_PROFILE_BALL,          mAc_PROFILE_MSCORE_CONTROL, mAc_PROFILE_DESIGN,
};

Actor_data TEST_FD_NPC_LAND_actor_data[2] = {
    { mAc_PROFILE_UKI, { 340, 0, 430 }, { 0, 0, 0 }, -1 },
    { mAc_PROFILE_AC_SIGN, { 0, 0, 0 }, { 0, 0, 0 }, -1 },
};

s16 TEST_FD_NPC_LAND_object_bank[2] = { ACTOR_OBJ_BANK_48, ACTOR_OBJ_BANK_411 };
