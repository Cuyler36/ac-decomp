#ifndef M_NAME_TABLE_H
#define M_NAME_TABLE_H

#include "types.h"
#include "m_actor_type.h"

/* TODO: these defintions are likely included from an auto-gen source */

#ifdef __cplusplus
extern "C" {
#endif

enum {
  NAME_TYPE_ITEM0,  /* Scenery items */
  NAME_TYPE_FTR0,   /* Furniture 0 */
  NAME_TYPE_ITEM1,  /* Obtainable items */
  NAME_TYPE_FTR1,   /* Furniture 1 */
  NAME_TYPE_WARP,   /* Loading zones */
  NAME_TYPE_STRUCT, /* Structures */
  NAME_TYPE_PAD6,   /* Unused? */
  NAME_TYPE_PAD7,   /* Unused? */
  NAME_TYPE_ITEM2,  /* Misc items */
  NAME_TYPE_ACTOR,  /* Actors */
  NAME_TYPE_PROPS,  /* Props */
  NAME_TYPE_PADB,   /* Unused? */
  NAME_TYPE_PADC,   /* Unused? */
  NAME_TYPE_SPNPC,  /* Special NPCs */
  NAME_TYPE_NPC,    /* Villager NPCs */
  NAME_TYPE_PAD15,  /* Unused? */

  NAME_TYPE_NUM
};

enum {
  ITEM1_CAT_PAPER,
  ITEM1_CAT_MONEY,
  ITEM1_CAT_TOOL,
  ITEM1_CAT_FISH,
  ITEM1_CAT_CLOTH,
  ITEM1_CAT_ETC,
  ITEM1_CAT_CARPET,
  ITEM1_CAT_WALL,
  ITEM1_CAT_FRUIT,
  ITEM1_CAT_PLANT,
  ITEM1_CAT_MINIDISK,
  ITEM1_CAT_DUMMY, /* diaries */
  ITEM1_CAT_TICKET,
  ITEM1_CAT_INSECT,
  ITEM1_CAT_HUKUBUKURO,
  ITEM1_CAT_KABU,

  ITEM1_CAT_NUM
};

extern int mNT_check_unknown(mActor_name_t item_no);

/* Retrieve the item actor's category */
#define ITEM_NAME_GET_TYPE(n) (((n) & 0xF000) >> 12)
#define ITEM_NAME_GET_CAT(n) (((n) & 0x0F00) >> 8)
#define ITEM_NAME_GET_INDEX(n) ((mActor_name_t)(n) & 0xFF)

#define ITEM_IS_FTR(n) \
  (ITEM_NAME_GET_TYPE(n) == NAME_TYPE_FTR0 || ITEM_NAME_GET_TYPE(n) == NAME_TYPE_FTR1)

#define ITEM_IS_ITEM1(n) (ITEM_NAME_GET_TYPE(n) == NAME_TYPE_ITEM1)

#define GET_NAME_ITEM0_CATEGORY(f) (((f) & 0x0800) >> 11) /* enviornmental or static background objects */
#define GET_NAME_ITEM1_CATEGORY(f) (((f) & 0x0F00) >> 8)

#define IS_ITEM_FLOWER(item) ((item) >= FLOWER_LEAVES_PANSIES0 && (item) <= FLOWER_TULIP2)
#define IS_ITEM_TREE(item) \
  (((item) >= TREE_SAPLING && (item) <= TREE_30000BELLS) || \
   ((item) >= TREE_100BELLS_SAPLING && (item) <= TREE_PALM_FRUIT) || \
   ((item) >= CEDAR_TREE_SAPLING && (item) <= CEDAR_TREE) || \
   ((item) >= GOLD_TREE_SAPLING && (item) <= GOLD_TREE) || \
   ((item) == DEAD_SAPLING) || \
   ((item) == DEAD_PALM_SAPLING) || \
   ((item) == DEAD_CEDAR_SAPLING) || \
   ((item) == DEAD_GOLD_SAPLING) \
  )

#define IS_ITEM_GROWN_TREE(item) \
  (((item) == TREE) || \
   ((item) == TREE_1000BELLS) || \
   ((item) == TREE_10000BELLS) || \
   ((item) == TREE_30000BELLS) || \
   ((item) == TREE_100BELLS) || \
   ((item) == CEDAR_TREE) || \
   ((item) == GOLD_TREE_SHOVEL || (item) == GOLD_TREE) || \
   ((item) == TREE_APPLE_NOFRUIT_0 || (item) == TREE_APPLE_NOFRUIT_1 || (item) == TREE_APPLE_NOFRUIT_2 || (item) == TREE_APPLE_FRUIT) || \
   ((item) == TREE_ORANGE_NOFRUIT_0 || (item) == TREE_ORANGE_NOFRUIT_1 || (item) == TREE_ORANGE_NOFRUIT_2 || (item) == TREE_ORANGE_FRUIT) || \
   ((item) == TREE_PEACH_NOFRUIT_0 || (item) == TREE_PEACH_NOFRUIT_1 || (item) == TREE_PEACH_NOFRUIT_2 || (item) == TREE_PEACH_FRUIT) || \
   ((item) == TREE_PEAR_NOFRUIT_0 || (item) == TREE_PEAR_NOFRUIT_1 || (item) == TREE_PEAR_NOFRUIT_2 || (item) == TREE_PEAR_FRUIT) || \
   ((item) == TREE_CHERRY_NOFRUIT_0 || (item) == TREE_CHERRY_NOFRUIT_1 || (item) == TREE_CHERRY_NOFRUIT_2 || (item) == TREE_CHERRY_FRUIT) || \
   ((item) == TREE_PALM_NOFRUIT_0 || (item) == TREE_PALM_NOFRUIT_1 || (item) == TREE_PALM_NOFRUIT_2 || (item) == TREE_PALM_FRUIT) || \
   ((item) == TREE_BEES) || \
   ((item) == TREE_FTR) || \
   ((item) == TREE_LIGHTS) || \
   ((item) == TREE_PRESENT) || \
   ((item) == TREE_BELLS) || \
   ((item) == CEDAR_TREE_BELLS) || \
   ((item) == CEDAR_TREE_FTR) || \
   ((item) == CEDAR_TREE_BEES) || \
   ((item) == CEDAR_TREE_LIGHTS) || \
   ((item) == GOLD_TREE_BELLS) || \
   ((item) == GOLD_TREE_FTR) || \
   ((item) == GOLD_TREE_BEES) \
  )

#define IS_ITEM_GRASS(item) ((item) >= GRASS_A && (item) <= GRASS_C) /* aka IS_ITEM_WEED */

#define BG_CATEGORY 0
#define ENV_CATEGORY 8

/* TODO: these should be calculated via definitions later */
#define FTR_NUM 1266
#define PAPER_NUM 256
#define MONEY_NUM 4
#define TOOL_NUM 92
#define FISH_NUM 40
#define CLOTH_NUM 255
#define ETC_NUM 49
#define CARPET_NUM 67
#define WALL_NUM 67
#define FRUIT_NUM 8
#define PLANT_NUM 11
#define MINIDISK_NUM 55
#define DIARY_NUM 16
#define TICKET_NUM 96
#define INSECT_NUM 40 + 5 // 5 spirits
#define HUKUBUKURO_NUM 2
#define KABU_NUM 4

#define EMPTY_NO 0x0000

#define GRASS_A 0x0008
#define GRASS_B (GRASS_A + 1)
#define GRASS_C (GRASS_B + 1)

#define TREE_BEES 0x005E
#define TREE_FTR (TREE_BEES + 1)
#define TREE_LIGHTS (TREE_FTR + 1)
#define TREE_PRESENT (TREE_LIGHTS + 1)
#define TREE_BELLS 0x0069

#define ROCK_A 0x0063
#define ROCK_B (ROCK_A + 1)
#define ROCK_C (ROCK_B + 1)
#define ROCK_D (ROCK_C + 1)
#define ROCK_E (ROCK_D + 1)

#define CEDAR_TREE_BELLS 0x0078
#define CEDAR_TREE_FTR (CEDAR_TREE_BELLS + 1)
#define CEDAR_TREE_BEES (CEDAR_TREE_FTR + 1)

#define GOLD_TREE_BELLS 0x007F
#define GOLD_TREE_FTR (GOLD_TREE_BELLS + 1)
#define GOLD_TREE_BEES (GOLD_TREE_FTR + 1)
#define CEDAR_TREE_LIGHTS (GOLD_TREE_BEES + 1)

/* begin environmental/nature objects */
#define ENV_START 0x0800
/* tree */
#define TREE_SAPLING ENV_START
#define TREE_S0 (TREE_SAPLING + 1)
#define TREE_S1 (TREE_S0 + 1)
#define TREE_S2 (TREE_S1 + 1)
#define TREE (TREE_S2 + 1) /* fully grown */
/* apple tree */
#define TREE_APPLE_SAPLING (TREE + 1)
#define TREE_APPLE_S0 (TREE_APPLE_SAPLING + 1)
#define TREE_APPLE_S1 (TREE_APPLE_S0 + 1)
#define TREE_APPLE_S2 (TREE_APPLE_S1 + 1)
#define TREE_APPLE_NOFRUIT_0 (TREE_APPLE_S2 + 1) /* fully grown, no fruit */
#define TREE_APPLE_NOFRUIT_1 (TREE_APPLE_NOFRUIT_0 + 1)
#define TREE_APPLE_NOFRUIT_2 (TREE_APPLE_NOFRUIT_1 + 1)
#define TREE_APPLE_FRUIT (TREE_APPLE_NOFRUIT_2 + 1)
/* orange tree */
#define TREE_ORANGE_SAPLING (TREE_APPLE_FRUIT + 1)
#define TREE_ORANGE_S0 (TREE_ORANGE_SAPLING + 1)
#define TREE_ORANGE_S1 (TREE_ORANGE_S0 + 1)
#define TREE_ORANGE_S2 (TREE_ORANGE_S1 + 1) /* fully grown, no fruit */
#define TREE_ORANGE_NOFRUIT_0 (TREE_ORANGE_S2 + 1)
#define TREE_ORANGE_NOFRUIT_1 (TREE_ORANGE_NOFRUIT_0 + 1)
#define TREE_ORANGE_NOFRUIT_2 (TREE_ORANGE_NOFRUIT_1 + 1)
#define TREE_ORANGE_FRUIT (TREE_ORANGE_NOFRUIT_2 + 1)
/* peach tree */
#define TREE_PEACH_SAPLING (TREE_ORANGE_FRUIT + 1)
#define TREE_PEACH_S0 (TREE_PEACH_SAPLING + 1)
#define TREE_PEACH_S1 (TREE_PEACH_S0 + 1)
#define TREE_PEACH_S2 (TREE_PEACH_S1 + 1)
#define TREE_PEACH_NOFRUIT_0 (TREE_PEACH_S2 + 1) /* fully grown, no fruit */
#define TREE_PEACH_NOFRUIT_1 (TREE_PEACH_NOFRUIT_0 + 1)
#define TREE_PEACH_NOFRUIT_2 (TREE_PEACH_NOFRUIT_1 + 1)
#define TREE_PEACH_FRUIT (TREE_PEACH_NOFRUIT_2 + 1)
/* pear tree */
#define TREE_PEAR_SAPLING (TREE_PEACH_FRUIT + 1)
#define TREE_PEAR_S0 (TREE_PEAR_SAPLING + 1)
#define TREE_PEAR_S1 (TREE_PEAR_S0 + 1)
#define TREE_PEAR_S2 (TREE_PEAR_S1 + 1)
#define TREE_PEAR_NOFRUIT_0 (TREE_PEAR_S2 + 1) /* fully grown, no fruit */
#define TREE_PEAR_NOFRUIT_1 (TREE_PEAR_NOFRUIT_0 + 1)
#define TREE_PEAR_NOFRUIT_2 (TREE_PEAR_NOFRUIT_1 + 1)
#define TREE_PEAR_FRUIT (TREE_PEAR_NOFRUIT_2 + 1)
/* cherry tree */
#define TREE_CHERRY_SAPLING (TREE_PEAR_FRUIT + 1)
#define TREE_CHERRY_S0 (TREE_CHERRY_SAPLING + 1)
#define TREE_CHERRY_S1 (TREE_CHERRY_S0 + 1)
#define TREE_CHERRY_S2 (TREE_CHERRY_S1 + 1)
#define TREE_CHERRY_NOFRUIT_0 (TREE_CHERRY_S2 + 1) /* fully grown, no fruit */
#define TREE_CHERRY_NOFRUIT_1 (TREE_CHERRY_NOFRUIT_0 + 1)
#define TREE_CHERRY_NOFRUIT_2 (TREE_CHERRY_NOFRUIT_1 + 1)
#define TREE_CHERRY_FRUIT (TREE_CHERRY_NOFRUIT_2 + 1)
/* money tree (1,000 Bells) */
#define TREE_1000BELLS_SAPLING (TREE_CHERRY_FRUIT + 1)
#define TREE_1000BELLS_S0 (TREE_1000BELLS_SAPLING + 1)
#define TREE_1000BELLS_S1 (TREE_1000BELLS_S0 + 1)
#define TREE_1000BELLS_S2 (TREE_1000BELLS_S1 + 1)
#define TREE_1000BELLS (TREE_1000BELLS_S2 + 1) /* fully grown w/ bells */
/* money tree (10,000 Bells) */
#define TREE_10000BELLS_SAPLING (TREE_1000BELLS + 1)
#define TREE_10000BELLS_S0 (TREE_10000BELLS_SAPLING + 1)
#define TREE_10000BELLS_S1 (TREE_10000BELLS_S0 + 1)
#define TREE_10000BELLS_S2 (TREE_10000BELLS_S1 + 1)
#define TREE_10000BELLS (TREE_10000BELLS_S2 + 1) /* fully grown w/ bells */
/* money tree (30,000 Bells) */
#define TREE_30000BELLS_SAPLING (TREE_10000BELLS + 1)
#define TREE_30000BELLS_S0 (TREE_30000BELLS_SAPLING + 1)
#define TREE_30000BELLS_S1 (TREE_30000BELLS_S0 + 1)
#define TREE_30000BELLS_S2 (TREE_30000BELLS_S1 + 1)
#define TREE_30000BELLS (TREE_30000BELLS_S2 + 1) /* fully grown w/ bells */
/* flower leaves (unused growth stage) */
#define FLOWER_LEAVES_PANSIES0 (TREE_30000BELLS + 1) /* white */
#define FLOWER_LEAVES_PANSIES1 (FLOWER_LEAVES_PANSIES0 + 1) /* purple */
#define FLOWER_LEAVES_PANSIES2 (FLOWER_LEAVES_PANSIES1 + 1) /* yellow */
#define FLOWER_LEAVES_COSMOS0 (FLOWER_LEAVES_PANSIES2 + 1) /* yellow */
#define FLOWER_LEAVES_COSMOS1 (FLOWER_LEAVES_COSMOS0 + 1) /* purple */
#define FLOWER_LEAVES_COSMOS2 (FLOWER_LEAVES_COSMOS1 + 1) /* blue */
#define FLOWER_LEAVES_TULIP0 (FLOWER_LEAVES_COSMOS2 + 1) /* red */
#define FLOWER_LEAVES_TULIP1 (FLOWER_LEAVES_TULIP0 + 1) /* white */
#define FLOWER_LEAVES_TULIP2 (FLOWER_LEAVES_TULIP1 + 1) /* yellow */
/* flowers */
#define FLOWER_PANSIES0 (FLOWER_LEAVES_TULIP2 + 1) /* white */
#define FLOWER_PANSIES1 (FLOWER_PANSIES0 + 1) /* purple */
#define FLOWER_PANSIES2 (FLOWER_PANSIES1 + 1) /* yellow */
#define FLOWER_COSMOS0 (FLOWER_PANSIES2 + 1) /* yellow */
#define FLOWER_COSMOS1 (FLOWER_COSMOS0 + 1) /* purple */
#define FLOWER_COSMOS2 (FLOWER_COSMOS1 + 1) /* blue */
#define FLOWER_TULIP0 (FLOWER_COSMOS2 + 1) /* red */
#define FLOWER_TULIP1 (FLOWER_TULIP0 + 1) /* white */
#define FLOWER_TULIP2 (FLOWER_TULIP1 + 1) /* yellow */
/* dead sapling */
#define DEAD_SAPLING (FLOWER_TULIP2 + 1)
/* money tree (100 Bells) */
#define TREE_100BELLS_SAPLING (DEAD_SAPLING + 1)
#define TREE_100BELLS_S0 (TREE_100BELLS_SAPLING + 1)
#define TREE_100BELLS_S1 (TREE_100BELLS_S0 + 1)
#define TREE_100BELLS_S2 (TREE_100BELLS_S1 + 1)
#define TREE_100BELLS (TREE_100BELLS_S2 + 1) /* fully grown w/ bells */
/* palm tree */
#define TREE_PALM_SAPLING (TREE_100BELLS + 1)
#define TREE_PALM_S0 (TREE_PALM_SAPLING + 1)
#define TREE_PALM_S1 (TREE_PALM_S0 + 1)
#define TREE_PALM_S2 (TREE_PALM_S1 + 1)
#define TREE_PALM_NOFRUIT_0 (TREE_PALM_S2 + 1) /* fully grown, no fruit */
#define TREE_PALM_NOFRUIT_1 (TREE_PALM_NOFRUIT_0 + 1)
#define TREE_PALM_NOFRUIT_2 (TREE_PALM_NOFRUIT_1 + 1)
#define TREE_PALM_FRUIT (TREE_PALM_NOFRUIT_2 + 1)
#define DEAD_PALM_SAPLING (TREE_PALM_FRUIT + 1)
/* cedar tree */
#define CEDAR_TREE_SAPLING (DEAD_PALM_SAPLING + 1)
#define CEDAR_TREE_S0 (CEDAR_TREE_SAPLING + 1)
#define CEDAR_TREE_S1 (CEDAR_TREE_S0 + 1)
#define CEDAR_TREE_S2 (CEDAR_TREE_S1 + 1)
#define CEDAR_TREE (CEDAR_TREE_S2 + 1) /* fully grown */
#define DEAD_CEDAR_SAPLING (CEDAR_TREE + 1)
/* gold tree */
#define GOLD_TREE_SAPLING (DEAD_CEDAR_SAPLING + 1)
#define GOLD_TREE_S0 (GOLD_TREE_SAPLING + 1)
#define GOLD_TREE_S1 (GOLD_TREE_S0 + 1)
#define GOLD_TREE_S2 (GOLD_TREE_S1 + 1)
#define GOLD_TREE_SHOVEL (GOLD_TREE_S2 + 1) /* fully grown w/ golden shovel */
#define GOLD_TREE (GOLD_TREE_SHOVEL + 1) /* fully grown */
#define DEAD_GOLD_SAPLING (GOLD_TREE + 1)
/* end of environmental objects */

#define SIGNBOARD_START 0x0900
#define SIGNBOARD (SIGNBOARD_START)
#define SIGNBOARD0_PLR0 (SIGNBOARD_START + 1)
#define SIGNBOARD1_PLR0 (SIGNBOARD0_PLR0 + 1)
#define SIGNBOARD2_PLR0 (SIGNBOARD1_PLR0 + 1)
#define SIGNBOARD3_PLR0 (SIGNBOARD2_PLR0 + 1)
#define SIGNBOARD4_PLR0 (SIGNBOARD3_PLR0 + 1)
#define SIGNBOARD5_PLR0 (SIGNBOARD4_PLR0 + 1)
#define SIGNBOARD6_PLR0 (SIGNBOARD5_PLR0 + 1)
#define SIGNBOARD7_PLR0 (SIGNBOARD6_PLR0 + 1)
#define SIGNBOARD0_PLR1 (SIGNBOARD7_PLR0 + 1)
#define SIGNBOARD1_PLR1 (SIGNBOARD0_PLR1 + 1)
#define SIGNBOARD2_PLR1 (SIGNBOARD1_PLR1 + 1)
#define SIGNBOARD3_PLR1 (SIGNBOARD2_PLR1 + 1)
#define SIGNBOARD4_PLR1 (SIGNBOARD3_PLR1 + 1)
#define SIGNBOARD5_PLR1 (SIGNBOARD4_PLR1 + 1)
#define SIGNBOARD6_PLR1 (SIGNBOARD5_PLR1 + 1)
#define SIGNBOARD7_PLR1 (SIGNBOARD6_PLR1 + 1)
#define SIGNBOARD0_PLR2 (SIGNBOARD7_PLR1 + 1)
#define SIGNBOARD1_PLR2 (SIGNBOARD0_PLR2 + 1)
#define SIGNBOARD2_PLR2 (SIGNBOARD1_PLR2 + 1)
#define SIGNBOARD3_PLR2 (SIGNBOARD2_PLR2 + 1)
#define SIGNBOARD4_PLR2 (SIGNBOARD3_PLR2 + 1)
#define SIGNBOARD5_PLR2 (SIGNBOARD4_PLR2 + 1)
#define SIGNBOARD6_PLR2 (SIGNBOARD5_PLR2 + 1)
#define SIGNBOARD7_PLR2 (SIGNBOARD6_PLR2 + 1)
#define SIGNBOARD0_PLR3 (SIGNBOARD7_PLR2 + 1)
#define SIGNBOARD1_PLR3 (SIGNBOARD0_PLR3 + 1)
#define SIGNBOARD2_PLR3 (SIGNBOARD1_PLR3 + 1)
#define SIGNBOARD3_PLR3 (SIGNBOARD2_PLR3 + 1)
#define SIGNBOARD4_PLR3 (SIGNBOARD3_PLR3 + 1)
#define SIGNBOARD5_PLR3 (SIGNBOARD4_PLR3 + 1)
#define SIGNBOARD6_PLR3 (SIGNBOARD5_PLR3 + 1)
#define SIGNBOARD7_PLR3 (SIGNBOARD6_PLR3 + 1)

#define FTR0_START 0x1000

#define FTR_CLOTH_MANNIQUIN000_SOUTH 0x17AC

#define FTR_REDALOHASHIRT 0x1814

#define FTR_BLUEALOHASHIRT 0x1818

#define FTR_CLOTH_MANNIQUIN254_SOUTH 0x1BA4
#define FTR_CLOTH_MANNIQUIN254_WEST 0x1BA7
#define FTR_CLOTH_MANNIQUIN_MY_ORIGINAL0 0x1BA8

#define FTR_UMBRELLA00_SOUTH 0x1D08

#define FTR_UMBRELLA31_WEST 0x1D87

#define FTR0_END 0x1FFF

#define FTR_TAPEDECK 0x1E58

#define ITM_PAPER_START 0x2000
#define ITM_PAPER_STACK_TWO_START 0x2040
#define ITM_PAPER_STACK_THREE_START 0x2080
#define ITM_PAPER_STACK_FOUR_START 0x20C0
#define ITM_PAPER_END 0x20FF

#define ITM_MONEY_START 0x2100
#define ITM_MONEY_1000 ITM_MONEY_START // 0x2100
#define ITM_MONEY_10000 (ITM_MONEY_1000 + 1) // 0x2101
#define ITM_MONEY_30000 (ITM_MONEY_10000 + 1) // 0x2102
#define ITM_MONEY_100 (ITM_MONEY_30000 + 1) // 0x2103
#define ITM_MONEY_END ITM_MONEY_100 // 0x2103

#define ITM_TOOL_START 0x2200
#define ITM_NET ITM_TOOL_START
#define ITM_AXE 0x2201
#define ITM_SHOVEL 0x2202
#define ITM_ROD 0x2203
#define ITM_GELATO_UMBRELLA 0x2204
#define ITM_DAFFODIL_PARASOL 0x2205
// TODO: other umbrellas
#define ITM_FLAME_UMBRELLA 0x2223
#define ITM_MY_ORG_UMBRELLA0 0x2224
#define ITM_MY_ORG_UMBRELLA1 0x2225
#define ITM_MY_ORG_UMBRELLA2 0x2226
#define ITM_MY_ORG_UMBRELLA3 0x2227
#define ITM_MY_ORG_UMBRELLA4 0x2228
#define ITM_MY_ORG_UMBRELLA5 0x2229
#define ITM_MY_ORG_UMBRELLA6 0x222A
#define ITM_MY_ORG_UMBRELLA7 0x222B
#define ITM_SICKLE 0x222C
#define ITM_RED_PAINT 0x222D
#define ITM_ORANGE_PAINT 0x222E
#define ITM_YELLOW_PAINT 0x222F
#define ITM_PALE_GREEN_PAINT 0x2230
#define ITM_GREEN_PAINT 0x2231
#define ITM_SKY_BLUE_PAINT 0x2232
#define ITM_BLUE_PAINT 0x2233
#define ITM_PURPLE_PAINT 0x2234
#define ITM_PINK_PAINT 0x2235
#define ITM_BLACK_PAINT 0x2236
#define ITM_WHITE_PAINT 0x2237
#define ITM_BROWN_PAINT 0x2238
#define ITM_GOLDEN_NET 0x2239
#define ITM_GOLDEN_AXE 0x223A
#define ITM_GOLDEN_SHOVEL 0x223B
#define ITM_GOLDEN_ROD 0x223C
#define ITM_AXE_USE_1 0x223D
#define ITM_AXE_USE_2 0x223E
#define ITM_AXE_USE_3 0x223F
#define ITM_AXE_USE_4 0x2240
#define ITM_AXE_USE_5 0x2241
#define ITM_AXE_USE_6 0x2242
#define ITM_AXE_USE_7 0x2243
#define ITM_RED_BALLOON 0x2244
#define ITM_YELLOW_BALLOON 0x2245
#define ITM_BLUE_BALLOON 0x2246
#define ITM_GREEN_BALLOON 0x2247
#define ITM_PURPLE_BALLOON 0x2248
#define ITM_BUNNY_P_BALLOON 0x2249
#define ITM_BUNNY_B_BALLOON 0x224A
#define ITM_BUNNY_O_BALLOON 0x224B
#define ITM_YELLOW_PINWHEEL 0x224C
#define ITM_RED_PINWHEEL 0x224D
#define ITM_TIGER_PINWHEEL 0x224E
#define ITM_GREEN_PINWHEEL 0x224F
#define ITM_PINK_PINWHEEL 0x2250
#define ITM_STRIPED_PINWHEEL 0x2251
#define ITM_FLOWER_PINWHEEL 0x2252
#define ITM_FANCY_PINWHEEL 0x2253
#define ITM_BLUEBELL_FAN 0x2254
#define ITM_PLUM_FAN 0x2255
#define ITM_BAMBOO_FAN 0x2256
#define ITM_CLOUD_FAN 0x2257
#define ITM_MAPLE_FAN 0x2258
#define ITM_FAN_FAN 0x2259
#define ITM_FLOWER_FAN 0x225A
#define ITM_LEAF_FAN 0x225B
#define ITM_TOOL_END (ITM_LEAF_FAN + 1)

#define ITM_CLOTH_START 0x2400
#define ITM_CLOTH_END 0x24FF

#define ITM_DUST0_EMPTY_CAN 0x250E
#define ITM_DUST1_BOOT 0x250F
#define ITM_DUST2_OLD_TIRE 0x2510
#define ITM_PITFALL 0x2512
#define ITM_SIGNBOARD 0x251E

#define ITM_FOOD_START 0x2800

#define ITM_FOOD_MUSHROOM 0x2805
#define ITM_FOOD_CANDY 0x2806

#define ITM_ENV_START 0x2900
#define ITM_SAPLING ITM_ENV_START
#define ITM_CEDAR_SAPLING 0x2901
#define ITM_WHITE_PANSY_BAG 0x2902
#define ITM_PURPLE_PANSY_BAG 0x2903
#define ITM_YELLOW_PANSY_BAG 0x2904
#define ITM_WHITE_COSMOS_BAG 0x2905
#define ITM_PINK_COSMOS_BAG 0x2906
#define ITM_BLUE_COSMOS_BAG 0x2907
#define ITM_RED_TULIP_BAG 0x2908
#define ITM_WHITE_TULIP_BAG 0x2909
#define ITEM_YELLOW_TULIP_BAG 0x290A

#define ITM_DIARY_START 0x2B00
#define ITM_COLLEGERULE ITM_DIARY_START
#define ITM_CALLIGRAPHY_PAD 0x2B0F
#define ITM_DIARY_END 0x2B10

#define ITM_KABU_10 0x2F00
#define ITM_KABU_50 0x2F01
#define ITM_KABU_100 0x2F02
#define ITM_KABU_SPOILED 0x2F03

#define FTR_ORANGEBOX 0x30F8
#define FTR_COLLEGERULE_SOUTH 0x30FC
#define FTR_CALLIGRAPHY_PAD_WEST 0x313B

#define DOOR_START 0x4000
#define DOOR0 DOOR_START  /* 0x4000 */
#define DOOR1 (DOOR0 + 1) /* 0x4001 */

#define EXIT_DOOR 0x4080

#define TRAIN_STATION 0x5809
#define TRAIN0 0x580A
#define TRAIN1 0x580B

#define SP_NPC_START 0xD000
#define SP_NPC_ARTIST (SP_NPC_START + 0) // D000
#define SP_NPC_BROKER (SP_NPC_START + 1) // D001
#define SP_NPC_DESIGNER (SP_NPC_START + 2) // D002
#define SP_NPC_POST_GIRL (SP_NPC_START + 3) // D003
#define SP_NPC_GUIDE (SP_NPC_START + 4) // D004
#define SP_NPC_GUIDE2 (SP_NPC_START + 5) // D005
#define SP_NPC_CARPETPEDDLER (SP_NPC_START + 6) // D006
#define SP_NPC_KABUPEDDLER (SP_NPC_START + 7) // D007
#define SP_NPC_SHOP_MASTER (SP_NPC_START + 8) // D008
#define SP_NPC_CONV_MASTER (SP_NPC_START + 9) // D009
#define SP_NPC_SUPER_MASTER (SP_NPC_START + 10) // D00A
#define SP_NPC_DEPART_MASTER (SP_NPC_START + 11) // D00B
#define SP_NPC_GYPSY (SP_NPC_START + 12) // D00C
#define SP_NPC_POLICE (SP_NPC_START + 13) // D00D
#define SP_NPC_STATION_MASTER (SP_NPC_START + 14) // D00E
#define SP_NPC_SANTA (SP_NPC_START + 15) // D00F
#define SP_NPC_POLICE2 (SP_NPC_START + 16) // D010
#define SP_NPC_POST_MAN (SP_NPC_START + 17) // D011
#define SP_NPC_POST_GIRL2 (SP_NPC_START + 18) // D012
#define SP_NPC_EV_BROKER2 (SP_NPC_START + 19) // D013
#define SP_NPC_RCN_GUIDE (SP_NPC_START + 20) // D014
//
#define SP_NPC_P_SEL (SP_NPC_START + 24) // D018
#define SP_NPC_RCN_GUIDE2 (SP_NPC_START + 25) // D019
//
#define SP_NPC_ANGLER (SP_NPC_START + 29) // D01D
#define SP_NPC_SHOP_MASTERSP (SP_NPC_START + 30) // D01E
#define SP_NPC_P_SEL2 (SP_NPC_START + 31) // D01F
//
#define SP_NPC_HALLOWEEN (SP_NPC_START + 37) // D025
#define SP_NPC_MAMEDANUKI0 (SP_NPC_START + 38) // D026
//
#define SP_NPC_EV_YOMISE (SP_NPC_START + 44) // D02C
//
#define SP_NPC_MAMEDANUKI1 (SP_NPC_START + 55) // D037
#define SP_NPC_SLEEP_OBABA (SP_NPC_START + 56) // D038
//
#define SP_NPC_EV_MIKO (SP_NPC_START + 61) // D03D
#define SP_NPC_MAJIN (SP_NPC_START + 62) // D03E
//
#define SP_NPC_TOTAKEKE (SP_NPC_START + 93) // D05D
//
#define SP_NPC_EV_DOZAEMON (SP_NPC_START + 100) // D064
#define SP_NPC_MAJIN2 (SP_NPC_START + 101) // D065
#define SP_NPC_RTC (SP_NPC_START + 102) // D066
#define SP_NPC_ENGINEER (SP_NPC_START + 103) // D067
#define SP_NPC_MAJIN3 (SP_NPC_START + 104) // D068
#define SP_NPC_MAJIN4 (SP_NPC_START + 105) // D069
#define SP_NPC_RESTART (SP_NPC_START + 106) // D06A
#define SP_NPC_MAJIN5 (SP_NPC_START + 107) // D06B
#define SP_NPC_EV_DOKUTU (SP_NPC_START + 108) // D06C
#define SP_NPC_CURATOR (SP_NPC_START + 109) // D06D
#define SP_NPC_EV_SONCHO (SP_NPC_START + 110) // D06E
#define SP_NPC_EV_GHOST (SP_NPC_START + 111) // D06F
#define SP_NPC_NEEDLEWORK0 (SP_NPC_START + 112) // D070
#define SP_NPC_NEEDLEWORK1 (SP_NPC_START + 113) // D071
#define SP_NPC_SENDO (SP_NPC_START + 114) // D072
//
#define SP_NPC_EV_SONCHO2 (SP_NPC_START + 116) // D074
#define SP_NPC_MASK_CAT (SP_NPC_START + 117) // D075
#define SP_NPC_MASK_CAT2 (SP_NPC_START + 118) // D076
//
#define SP_NPC_SASHO (SP_NPC_START + 122) // D07A
//
#define SP_NPC_MAJIN_BROTHER (SP_NPC_START + 126) // D07E
#define SP_NPC_SONCHO (SP_NPC_START + 127) // D07F
//
#define SP_NPC_EV_MAJIN (SP_NPC_START + 129) // D081
//
#define SP_NPC_EV_SPEECH_SONCHO (SP_NPC_START + 135) // D087
//
#define SP_NPC_TURKEY (SP_NPC_START + 141) // D08D
#define SP_NPC_HEM (SP_NPC_START + 142) // D08E


#define NPC_START 0xE000
#define NPC_BOB NPC_START
// TODO: Finish
#define NPC_JOEY 0xE048
#define NPC_PAOLO 0xE04C
#define NPC_JANE 0xE061
#define NPC_CARRIE 0xE06F
#define NPC_SAMSON 0xE07D
#define NPC_BUZZ 0xE08C
#define NPC_CUBE 0xE093
#define NPC_RASHER 0xE09B
#define NPC_TANK 0xE0AA
#define NPC_VESTA 0xE0AD
#define NPC_TYBALT 0xE0B6
#define NPC_LOBO 0xE0B9
#define NPC_BIFF 0xE0C2

#define RSV_DOOR 0xFE1B
#define RSV_WALL_NO 0xFFFE /* interior wall item, no collision */
#define RSV_NO 0xFFFF /* reserved space, can't interact but no collision */

#ifdef __cplusplus
}
#endif

#endif
