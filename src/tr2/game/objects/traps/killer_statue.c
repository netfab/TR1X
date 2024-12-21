#include "game/objects/traps/killer_statue.h"

#include "game/items.h"
#include "game/lara/control.h"
#include "game/objects/common.h"
#include "game/random.h"
#include "global/funcs.h"
#include "global/vars.h"

#include <libtrx/game/lara/common.h>

#define KILLER_STATUE_CUT_DAMAGE 20
#define KILLER_STATUE_TOUCH_BITS 0b10000000 // = 128
// clang-format on

typedef enum {
    // clang-format off
    KILLER_STATUE_STATE_EMPTY = 0,
    KILLER_STATUE_STATE_STOP  = 1,
    KILLER_STATUE_STATE_CUT   = 2,
    // clang-format on
} KILLER_STATUE_STATE;

typedef enum {
    // clang-format off
    KILLER_STATUE_ANIM_RETURN   = 0,
    KILLER_STATUE_ANIM_FINISHED = 1,
    KILLER_STATUE_ANIM_CUT      = 2,
    KILLER_STATUE_ANIM_SET      = 3,
    // clang-format on
} KILLER_STATUE_ANIM;

void __cdecl KillerStatue_Initialise(const int16_t item_num)
{
    ITEM *const item = Item_Get(item_num);
    const OBJECT *const obj = Object_GetObject(item->object_id);
    item->anim_num = obj->anim_idx + KILLER_STATUE_ANIM_SET;
    item->frame_num = g_Anims[item->anim_num].frame_base;
    item->current_anim_state = KILLER_STATUE_STATE_STOP;
}

void __cdecl KillerStatue_Control(const int16_t item_num)
{
    ITEM *const item = Item_Get(item_num);

    if (Item_IsTriggerActive(item)
        && item->current_anim_state == KILLER_STATUE_STATE_STOP) {
        item->goal_anim_state = KILLER_STATUE_STATE_CUT;
    } else {
        item->goal_anim_state = KILLER_STATUE_STATE_STOP;
    }

    if ((item->touch_bits & KILLER_STATUE_TOUCH_BITS) != 0
        && item->current_anim_state == KILLER_STATUE_STATE_CUT) {
        Lara_TakeDamage(KILLER_STATUE_CUT_DAMAGE, true);

        const ITEM *const lara_item = Lara_GetItem();
        DoBloodSplat(
            g_LaraItem->pos.x + (Random_GetControl() - 0x4000) / 256,
            g_LaraItem->pos.y - Random_GetControl() / 44,
            g_LaraItem->pos.z + (Random_GetControl() - 0x4000) / 256,
            g_LaraItem->speed,
            g_LaraItem->rot.y + (Random_GetControl() - 0x4000) / 8,
            g_LaraItem->room_num);
    }

    Item_Animate(item);
}

void KillerStatue_Setup(void)
{
    OBJECT *const obj = Object_GetObject(O_KILLER_STATUE);
    obj->initialise = KillerStatue_Initialise;
    obj->control = KillerStatue_Control;
    obj->collision = Object_Collision_Trap;
    obj->save_flags = 1;
    obj->save_anim = 1;
}
