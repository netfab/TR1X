#pragma once

typedef enum {
    // clang-format off
    INPUT_ROLE_UP                = 0,
    INPUT_ROLE_DOWN              = 1,
    INPUT_ROLE_LEFT              = 2,
    INPUT_ROLE_RIGHT             = 3,
    INPUT_ROLE_STEP_L            = 4,
    INPUT_ROLE_STEP_R            = 5,
    INPUT_ROLE_SLOW              = 6,
    INPUT_ROLE_JUMP              = 7,
    INPUT_ROLE_ACTION            = 8,
    INPUT_ROLE_DRAW              = 9,
    INPUT_ROLE_LOOK              = 10,
    INPUT_ROLE_ROLL              = 11,
    INPUT_ROLE_OPTION            = 12,
    INPUT_ROLE_FLY_CHEAT         = 13,
    INPUT_ROLE_ITEM_CHEAT        = 14,
    INPUT_ROLE_LEVEL_SKIP_CHEAT  = 15,
    INPUT_ROLE_TURBO_CHEAT       = 16,
    INPUT_ROLE_PAUSE             = 17,
    INPUT_ROLE_CAMERA_FORWARD    = 18,
    INPUT_ROLE_CAMERA_BACK       = 19,
    INPUT_ROLE_CAMERA_LEFT       = 20,
    INPUT_ROLE_CAMERA_RIGHT      = 21,
    INPUT_ROLE_CAMERA_RESET      = 22,
    INPUT_ROLE_EQUIP_PISTOLS     = 23,
    INPUT_ROLE_EQUIP_SHOTGUN     = 24,
    INPUT_ROLE_EQUIP_MAGNUMS     = 25,
    INPUT_ROLE_EQUIP_UZIS        = 26,
    INPUT_ROLE_USE_SMALL_MEDI    = 27,
    INPUT_ROLE_USE_BIG_MEDI      = 28,
    INPUT_ROLE_SAVE              = 29,
    INPUT_ROLE_LOAD              = 30,
    INPUT_ROLE_FPS               = 31,
    INPUT_ROLE_BILINEAR          = 32,
    INPUT_ROLE_ENTER_CONSOLE     = 33,
    INPUT_ROLE_CHANGE_TARGET     = 34,
    INPUT_ROLE_TOGGLE_UI         = 35,
    INPUT_ROLE_CAMERA_UP         = 36,
    INPUT_ROLE_CAMERA_DOWN       = 37,
    INPUT_ROLE_TOGGLE_PHOTO_MODE = 38,
    INPUT_ROLE_UNBIND_KEY        = 39,
    INPUT_ROLE_RESET_BINDINGS    = 40,
    INPUT_ROLE_PERSPECTIVE       = 42,
    INPUT_ROLE_MENU_CONFIRM      = 43,
    INPUT_ROLE_MENU_BACK         = 44,
    INPUT_ROLE_MENU_LEFT         = 45,
    INPUT_ROLE_MENU_UP           = 46,
    INPUT_ROLE_MENU_DOWN         = 47,
    INPUT_ROLE_MENU_RIGHT        = 48,
    INPUT_ROLE_NUMBER_OF,
    // clang-format on
} INPUT_ROLE;
