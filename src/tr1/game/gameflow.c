#include "game/gameflow.h"

#include "game/fmv.h"
#include "game/game.h"
#include "game/game_string.h"
#include "game/inventory.h"
#include "game/inventory_ring.h"
#include "game/lara/common.h"
#include "game/music.h"
#include "game/objects/creatures/bacon_lara.h"
#include "game/objects/vars.h"
#include "game/output.h"
#include "game/phase/phase.h"
#include "game/phase/phase_cutscene.h"
#include "game/phase/phase_inventory.h"
#include "game/phase/phase_stats.h"
#include "game/room.h"
#include "game/savegame.h"
#include "global/vars.h"

#include <libtrx/config.h>
#include <libtrx/debug.h>
#include <libtrx/enum_map.h>
#include <libtrx/filesystem.h>
#include <libtrx/game/objects/names.h>
#include <libtrx/game/phase.h>
#include <libtrx/json.h>
#include <libtrx/log.h>
#include <libtrx/memory.h>

#include <string.h>

typedef struct {
    const char *str;
    const int32_t val;
} STRING_TO_ENUM_TYPE;

typedef struct {
    char *path;
    double display_time;
} GAME_FLOW_DISPLAY_PICTURE_DATA;

typedef struct {
    GAME_OBJECT_ID object1_id;
    GAME_OBJECT_ID object2_id;
    int32_t mesh_num;
} GAME_FLOW_MESH_SWAP_DATA;

typedef struct {
    GAME_OBJECT_ID object_id;
    int quantity;
} GAME_FLOW_GIVE_ITEM_DATA;

GAME_FLOW g_GameFlow = {};

static int32_t M_StringToEnumType(
    const char *const str, const STRING_TO_ENUM_TYPE *map);
static bool M_LoadScriptMeta(JSON_OBJECT *obj);
static bool M_LoadScriptGameStrings(JSON_OBJECT *obj);
static bool M_IsLegacySequence(const char *type_str);
static bool M_LoadLevelSequence(JSON_OBJECT *obj, int32_t level_num);
static bool M_LoadScriptLevels(JSON_OBJECT *obj);
static void M_StringTableShutdown(GAME_FLOW_STRING_ENTRY *dest);
static bool M_LoadObjectNames(
    JSON_OBJECT *root_obj, const char *key, GAME_FLOW_STRING_ENTRY **dest);
static void M_ScanLevelText(
    const GAME_FLOW_STRING_ENTRY *entry,
    void (*callback)(GAME_OBJECT_ID object_id, const char *text));

static const STRING_TO_ENUM_TYPE m_GameFlowLevelTypeEnumMap[] = {
    { "title", GFL_TITLE },
    { "normal", GFL_NORMAL },
    { "cutscene", GFL_CUTSCENE },
    { "gym", GFL_GYM },
    { "current", GFL_CURRENT },
    { "bonus", GFL_BONUS },
    { "title_demo_pc", GFL_TITLE_DEMO_PC },
    { "level_demo_pc", GFL_LEVEL_DEMO_PC },
    { NULL, -1 },
};

static const STRING_TO_ENUM_TYPE m_GameFlowSeqTypeEnumMap[] = {
    { "start_game", GFS_START_GAME },
    { "stop_game", GFS_STOP_GAME },
    { "loop_game", GFS_LOOP_GAME },
    { "start_cine", GFS_START_CINE },
    { "loop_cine", GFS_LOOP_CINE },
    { "play_fmv", GFS_PLAY_FMV },
    { "loading_screen", GFS_LOADING_SCREEN },
    { "display_picture", GFS_DISPLAY_PICTURE },
    { "level_stats", GFS_LEVEL_STATS },
    { "total_stats", GFS_TOTAL_STATS },
    { "exit_to_title", GFS_EXIT_TO_TITLE },
    { "exit_to_level", GFS_EXIT_TO_LEVEL },
    { "exit_to_cine", GFS_EXIT_TO_CINE },
    { "set_cam_x", GFS_SET_CAM_X },
    { "set_cam_y", GFS_SET_CAM_Y },
    { "set_cam_z", GFS_SET_CAM_Z },
    { "set_cam_angle", GFS_SET_CAM_ANGLE },
    { "flip_map", GFS_FLIP_MAP },
    { "remove_guns", GFS_REMOVE_GUNS },
    { "remove_scions", GFS_REMOVE_SCIONS },
    { "remove_ammo", GFS_REMOVE_AMMO },
    { "remove_medipacks", GFS_REMOVE_MEDIPACKS },
    { "give_item", GFS_GIVE_ITEM },
    { "play_synced_audio", GFS_PLAY_SYNCED_AUDIO },
    { "mesh_swap", GFS_MESH_SWAP },
    { "setup_bacon_lara", GFS_SETUP_BACON_LARA },
    { NULL, -1 },
};

static int32_t M_StringToEnumType(
    const char *const str, const STRING_TO_ENUM_TYPE *map)
{
    while (map->str) {
        if (!strcmp(str, map->str)) {
            break;
        }
        map++;
    }
    return map->val;
}

static bool M_LoadScriptMeta(JSON_OBJECT *obj)
{
    const char *tmp_s;
    int tmp_i;
    double tmp_d;
    JSON_ARRAY *tmp_arr;

    tmp_s = JSON_ObjectGetString(obj, "main_menu_picture", JSON_INVALID_STRING);
    if (tmp_s == JSON_INVALID_STRING) {
        LOG_ERROR("'main_menu_picture' must be a string");
        return false;
    }
    g_GameFlow.main_menu_background_path = Memory_DupStr(tmp_s);

    tmp_s =
        JSON_ObjectGetString(obj, "savegame_fmt_legacy", JSON_INVALID_STRING);
    if (tmp_s == JSON_INVALID_STRING) {
        LOG_ERROR("'savegame_fmt_legacy' must be a string");
        return false;
    }
    g_GameFlow.savegame_fmt_legacy = Memory_DupStr(tmp_s);

    tmp_s = JSON_ObjectGetString(obj, "savegame_fmt_bson", JSON_INVALID_STRING);
    if (tmp_s == JSON_INVALID_STRING) {
        LOG_ERROR("'savegame_fmt_bson' must be a string");
        return false;
    }
    g_GameFlow.savegame_fmt_bson = Memory_DupStr(tmp_s);

    tmp_d = JSON_ObjectGetDouble(obj, "demo_delay", -1.0);
    if (tmp_d < 0.0) {
        LOG_ERROR("'demo_delay' must be a positive number");
        return false;
    }
    g_GameFlow.demo_delay = tmp_d;

    tmp_arr = JSON_ObjectGetArray(obj, "water_color");
    g_GameFlow.water_color.r = 0.6;
    g_GameFlow.water_color.g = 0.7;
    g_GameFlow.water_color.b = 1.0;
    if (tmp_arr) {
        g_GameFlow.water_color.r =
            JSON_ArrayGetDouble(tmp_arr, 0, g_GameFlow.water_color.r);
        g_GameFlow.water_color.g =
            JSON_ArrayGetDouble(tmp_arr, 1, g_GameFlow.water_color.g);
        g_GameFlow.water_color.b =
            JSON_ArrayGetDouble(tmp_arr, 2, g_GameFlow.water_color.b);
    }

    if (JSON_ObjectGetValue(obj, "draw_distance_fade")) {
        double value = JSON_ObjectGetDouble(
            obj, "draw_distance_fade", JSON_INVALID_NUMBER);
        if (value == JSON_INVALID_NUMBER) {
            LOG_ERROR("'draw_distance_fade' must be a number");
            return false;
        }
        g_GameFlow.draw_distance_fade = value;
    } else {
        g_GameFlow.draw_distance_fade = 12.0f;
    }

    if (JSON_ObjectGetValue(obj, "draw_distance_max")) {
        double value =
            JSON_ObjectGetDouble(obj, "draw_distance_max", JSON_INVALID_NUMBER);
        if (value == JSON_INVALID_NUMBER) {
            LOG_ERROR("'draw_distance_max' must be a number");
            return false;
        }
        g_GameFlow.draw_distance_max = value;
    } else {
        g_GameFlow.draw_distance_max = 20.0f;
    }

    tmp_arr = JSON_ObjectGetArray(obj, "injections");
    if (tmp_arr) {
        g_GameFlow.injections.length = tmp_arr->length;
        g_GameFlow.injections.data_paths =
            Memory_Alloc(sizeof(char *) * tmp_arr->length);
        for (size_t i = 0; i < tmp_arr->length; i++) {
            const char *const str = JSON_ArrayGetString(tmp_arr, i, NULL);
            g_GameFlow.injections.data_paths[i] = Memory_DupStr(str);
        }
    } else {
        g_GameFlow.injections.length = 0;
    }

    g_GameFlow.enable_tr2_item_drops =
        JSON_ObjectGetBool(obj, "enable_tr2_item_drops", false);
    g_GameFlow.convert_dropped_guns =
        JSON_ObjectGetBool(obj, "convert_dropped_guns", false);
    g_GameFlow.enable_killer_pushblocks =
        JSON_ObjectGetBool(obj, "enable_killer_pushblocks", true);

    return true;
}

static bool M_LoadScriptGameStrings(JSON_OBJECT *obj)
{
    JSON_OBJECT *strings_obj = JSON_ObjectGetObject(obj, "strings");
    if (!strings_obj) {
        LOG_ERROR("'strings' must be a dictionary");
        return false;
    }

    JSON_OBJECT_ELEMENT *strings_elem = strings_obj->start;
    while (strings_elem) {
        const char *const key = strings_elem->name->string;
        const char *const value = JSON_ObjectGetString(strings_obj, key, NULL);
        if (!GameString_IsKnown(key)) {
            LOG_ERROR("invalid game string key: %s", key);
        } else if (value == NULL) {
            LOG_ERROR("invalid game string value: %s", key);
        } else {
            GameString_Define(key, value);
        }
        strings_elem = strings_elem->next;
    }

    return true;
}

static bool M_IsLegacySequence(const char *type_str)
{
    return !strcmp(type_str, "fix_pyramid_secret")
        || !strcmp(type_str, "stop_cine");
}

static bool M_LoadLevelSequence(JSON_OBJECT *obj, int32_t level_num)
{
    JSON_ARRAY *jseq_arr = JSON_ObjectGetArray(obj, "sequence");
    if (!jseq_arr) {
        LOG_ERROR("level %d: 'sequence' must be a list", level_num);
        return false;
    }

    JSON_ARRAY_ELEMENT *jseq_elem = jseq_arr->start;

    g_GameFlow.levels[level_num].sequence =
        Memory_Alloc(sizeof(GAME_FLOW_SEQUENCE) * (jseq_arr->length + 1));

    GAME_FLOW_SEQUENCE *seq = g_GameFlow.levels[level_num].sequence;
    int32_t i = 0;
    while (jseq_elem) {
        JSON_OBJECT *jseq_obj = JSON_ValueAsObject(jseq_elem->value);
        if (!jseq_obj) {
            LOG_ERROR("level %d: 'sequence' elements must be dictionaries");
            return false;
        }

        const char *type_str =
            JSON_ObjectGetString(jseq_obj, "type", JSON_INVALID_STRING);
        if (type_str == JSON_INVALID_STRING) {
            LOG_ERROR("level %d: sequence 'type' must be a string", level_num);
            return false;
        }

        seq->type = M_StringToEnumType(type_str, m_GameFlowSeqTypeEnumMap);

        switch (seq->type) {
        case GFS_START_GAME:
        case GFS_STOP_GAME:
        case GFS_LOOP_GAME:
        case GFS_START_CINE:
        case GFS_LOOP_CINE:
            seq->data = (void *)(intptr_t)level_num;
            break;

        case GFS_PLAY_FMV: {
            const char *tmp_s =
                JSON_ObjectGetString(jseq_obj, "fmv_path", JSON_INVALID_STRING);
            if (tmp_s == JSON_INVALID_STRING) {
                LOG_ERROR(
                    "level %d, sequence %s: 'fmv_path' must be a string",
                    level_num, type_str);
                return false;
            }
            seq->data = Memory_DupStr(tmp_s);
            break;
        }

        case GFS_LOADING_SCREEN:
        case GFS_DISPLAY_PICTURE: {
            GAME_FLOW_DISPLAY_PICTURE_DATA *data =
                Memory_Alloc(sizeof(GAME_FLOW_DISPLAY_PICTURE_DATA));

            const char *tmp_s = JSON_ObjectGetString(
                jseq_obj, "picture_path", JSON_INVALID_STRING);
            if (tmp_s == JSON_INVALID_STRING) {
                LOG_ERROR(
                    "level %d, sequence %s: 'picture_path' must be a string",
                    level_num, type_str);
                return false;
            }
            data->path = Memory_DupStr(tmp_s);

            double tmp_d = JSON_ObjectGetDouble(jseq_obj, "display_time", -1.0);
            if (tmp_d < 0.0) {
                LOG_ERROR(
                    "level %d, sequence %s: 'display_time' must be a positive "
                    "number",
                    level_num, type_str);
                return false;
            }
            data->display_time = tmp_d;
            seq->data = data;
            break;
        }

        case GFS_LEVEL_STATS: {
            int tmp =
                JSON_ObjectGetInt(jseq_obj, "level_id", JSON_INVALID_NUMBER);
            if (tmp == JSON_INVALID_NUMBER) {
                LOG_ERROR(
                    "level %d, sequence %s: 'level_id' must be a number",
                    level_num, type_str);
                return false;
            }
            seq->data = (void *)(intptr_t)tmp;
            break;
        }

        case GFS_TOTAL_STATS: {
            GAME_FLOW_DISPLAY_PICTURE_DATA *data =
                Memory_Alloc(sizeof(GAME_FLOW_DISPLAY_PICTURE_DATA));

            const char *tmp_s = JSON_ObjectGetString(
                jseq_obj, "picture_path", JSON_INVALID_STRING);
            if (tmp_s == JSON_INVALID_STRING) {
                LOG_ERROR(
                    "level %d, sequence %s: 'picture_path' must be a string",
                    level_num, type_str);
                return false;
            }
            data->path = Memory_DupStr(tmp_s);
            data->display_time = 0;
            seq->data = data;
            break;
        }

        case GFS_EXIT_TO_TITLE:
            break;

        case GFS_EXIT_TO_LEVEL:
        case GFS_EXIT_TO_CINE: {
            int tmp =
                JSON_ObjectGetInt(jseq_obj, "level_id", JSON_INVALID_NUMBER);
            if (tmp == JSON_INVALID_NUMBER) {
                LOG_ERROR(
                    "level %d, sequence %s: 'level_id' must be a number",
                    level_num, type_str);
                return false;
            }
            seq->data = (void *)(intptr_t)tmp;
            break;
        }

        case GFS_SET_CAM_X:
        case GFS_SET_CAM_Y:
        case GFS_SET_CAM_Z:
        case GFS_SET_CAM_ANGLE: {
            int tmp = JSON_ObjectGetInt(jseq_obj, "value", JSON_INVALID_NUMBER);
            if (tmp == JSON_INVALID_NUMBER) {
                LOG_ERROR(
                    "level %d, sequence %s: 'value' must be a number",
                    level_num, type_str);
                return false;
            }
            seq->data = (void *)(intptr_t)tmp;
            break;
        }

        case GFS_FLIP_MAP:
        case GFS_REMOVE_GUNS:
        case GFS_REMOVE_SCIONS:
        case GFS_REMOVE_AMMO:
        case GFS_REMOVE_MEDIPACKS:
            break;

        case GFS_GIVE_ITEM: {
            GAME_FLOW_GIVE_ITEM_DATA *give_item_data =
                Memory_Alloc(sizeof(GAME_FLOW_GIVE_ITEM_DATA));

            give_item_data->object_id =
                JSON_ObjectGetInt(jseq_obj, "object_id", JSON_INVALID_NUMBER);
            if (give_item_data->object_id == JSON_INVALID_NUMBER) {
                LOG_ERROR(
                    "level %d, sequence %s: 'object_id' must be a number",
                    level_num, type_str);
                return false;
            }

            give_item_data->quantity =
                JSON_ObjectGetInt(jseq_obj, "quantity", 1);

            seq->data = give_item_data;
            break;
        }

        case GFS_PLAY_SYNCED_AUDIO: {
            int tmp =
                JSON_ObjectGetInt(jseq_obj, "audio_id", JSON_INVALID_NUMBER);
            if (tmp == JSON_INVALID_NUMBER) {
                LOG_ERROR(
                    "level %d, sequence %s: 'audio_id' must be a number",
                    level_num, type_str);
                return false;
            }
            seq->data = (void *)(intptr_t)tmp;
            break;
        }

        case GFS_MESH_SWAP: {
            GAME_FLOW_MESH_SWAP_DATA *swap_data =
                Memory_Alloc(sizeof(GAME_FLOW_MESH_SWAP_DATA));

            swap_data->object1_id =
                JSON_ObjectGetInt(jseq_obj, "object1_id", JSON_INVALID_NUMBER);
            if (swap_data->object1_id == JSON_INVALID_NUMBER) {
                LOG_ERROR(
                    "level %d, sequence %s: 'object1_id' must be a number",
                    level_num, type_str);
                return false;
            }

            swap_data->object2_id =
                JSON_ObjectGetInt(jseq_obj, "object2_id", JSON_INVALID_NUMBER);
            if (swap_data->object2_id == JSON_INVALID_NUMBER) {
                LOG_ERROR(
                    "level %d, sequence %s: 'object2_id' must be a number",
                    level_num, type_str);
                return false;
            }

            swap_data->mesh_num =
                JSON_ObjectGetInt(jseq_obj, "mesh_id", JSON_INVALID_NUMBER);
            if (swap_data->mesh_num == JSON_INVALID_NUMBER) {
                LOG_ERROR(
                    "level %d, sequence %s: 'mesh_id' must be a number",
                    level_num, type_str);
                return false;
            }

            seq->data = swap_data;
            break;
        }

        case GFS_SETUP_BACON_LARA: {
            int tmp =
                JSON_ObjectGetInt(jseq_obj, "anchor_room", JSON_INVALID_NUMBER);
            if (tmp == JSON_INVALID_NUMBER) {
                LOG_ERROR(
                    "level %d, sequence %s: 'anchor_room' must be a number",
                    level_num, type_str);
                return false;
            }
            if (tmp < 0) {
                LOG_ERROR(
                    "level %d, sequence %s: 'anchor_room' must be >= 0",
                    level_num, type_str);
                return false;
            }
            seq->data = (void *)(intptr_t)tmp;
            break;
        }

        default:
            if (M_IsLegacySequence(type_str)) {
                seq->type = GFS_LEGACY;
                LOG_WARNING(
                    "level %d, sequence %s: legacy type ignored", level_num,
                    type_str);

            } else {
                LOG_ERROR("unknown sequence type %s", type_str);
                return false;
            }
            break;
        }

        jseq_elem = jseq_elem->next;
        i++;
        seq++;
    }

    seq->type = GFS_END;
    seq->data = NULL;

    return true;
}

static void M_StringTableShutdown(GAME_FLOW_STRING_ENTRY *const dest)
{
    if (dest == NULL) {
        return;
    }
    GAME_FLOW_STRING_ENTRY *cur = dest;
    while (cur->key != NULL) {
        Memory_FreePointer(&cur->key);
        Memory_FreePointer(&cur->value);
        cur++;
    }
    Memory_Free(dest);
}

static bool M_LoadObjectNames(
    JSON_OBJECT *const root_obj, const char *const key,
    GAME_FLOW_STRING_ENTRY **dest)
{
    JSON_OBJECT *strings_obj = JSON_ObjectGetObject(root_obj, key);
    if (strings_obj == NULL) {
        return false;
    }

    struct {
        char *json_key;
        char *target_string;
    } legacy_string_defs[] = {
        { "puzzle1", "O_PUZZLE_ITEM_1" }, { "puzzle1", "O_PUZZLE_OPTION_1" },
        { "puzzle2", "O_PUZZLE_ITEM_2" }, { "puzzle2", "O_PUZZLE_OPTION_2" },
        { "puzzle3", "O_PUZZLE_ITEM_3" }, { "puzzle3", "O_PUZZLE_OPTION_3" },
        { "puzzle4", "O_PUZZLE_ITEM_4" }, { "puzzle4", "O_PUZZLE_OPTION_4" },
        { "key1", "O_KEY_ITEM_1" },       { "key1", "O_KEY_OPTION_1" },
        { "key2", "O_KEY_ITEM_2" },       { "key2", "O_KEY_OPTION_2" },
        { "key3", "O_KEY_ITEM_3" },       { "key3", "O_KEY_OPTION_3" },
        { "key4", "O_KEY_ITEM_4" },       { "key4", "O_KEY_OPTION_4" },
        { "pickup1", "O_PICKUP_ITEM_1" }, { "pickup1", "O_PICKUP_OPTION_1" },
        { "pickup2", "O_PICKUP_ITEM_2" }, { "pickup2", "O_PICKUP_OPTION_2" },
        { "leadbar", "O_LEADBAR_ITEM" },  { "leadbar", "O_LEADBAR_OPTION" },
        { "scion", "O_SCION_OPTION" },    { NULL, NULL },
    };

    // count allocation size
    int32_t count = 0;
    for (int32_t i = 0; legacy_string_defs[i].json_key != NULL; i++) {
        if (JSON_ObjectGetString(
                strings_obj, legacy_string_defs[i].json_key,
                JSON_INVALID_STRING)
            != JSON_INVALID_STRING) {
            count++;
        }
    }

    *dest = Memory_Alloc(sizeof(GAME_FLOW_STRING_ENTRY) * (count + 1));

    GAME_FLOW_STRING_ENTRY *cur = *dest;
    for (int32_t i = 0; legacy_string_defs[i].json_key != NULL; i++) {
        const char *value = JSON_ObjectGetString(
            strings_obj, legacy_string_defs[i].json_key, JSON_INVALID_STRING);
        if (value != JSON_INVALID_STRING) {
            cur->key = Memory_DupStr(legacy_string_defs[i].target_string);
            cur->value = Memory_DupStr(value);
            cur++;
        }
    }

    return true;
}

static void M_ScanLevelText(
    const GAME_FLOW_STRING_ENTRY *entry,
    void (*callback)(GAME_OBJECT_ID object_id, const char *text))
{
    while (entry != NULL && entry->key != NULL) {
        const GAME_OBJECT_ID object_id =
            ENUM_MAP_GET(GAME_OBJECT_ID, entry->key, NO_OBJECT);
        if (object_id != NO_OBJECT) {
            callback(object_id, entry->value);
        }
        entry++;
    }
}

static bool M_LoadScriptLevels(JSON_OBJECT *obj)
{
    JSON_ARRAY *jlvl_arr = JSON_ObjectGetArray(obj, "levels");
    if (!jlvl_arr) {
        LOG_ERROR("'levels' must be a list");
        return false;
    }

    int32_t level_count = jlvl_arr->length;

    g_GameFlow.levels = Memory_Alloc(sizeof(GAME_FLOW_LEVEL) * level_count);
    g_GameInfo.current = Memory_Alloc(sizeof(RESUME_INFO) * level_count);

    JSON_ARRAY_ELEMENT *jlvl_elem = jlvl_arr->start;
    int level_num = 0;

    g_GameFlow.has_demo = 0;
    g_GameFlow.gym_level_num = -1;
    g_GameFlow.first_level_num = -1;
    g_GameFlow.last_level_num = -1;
    g_GameFlow.title_level_num = -1;
    g_GameFlow.level_count = jlvl_arr->length;

    GAME_FLOW_LEVEL *cur = &g_GameFlow.levels[0];
    while (jlvl_elem) {
        JSON_OBJECT *jlvl_obj = JSON_ValueAsObject(jlvl_elem->value);
        if (!jlvl_obj) {
            LOG_ERROR("'levels' elements must be dictionaries");
            return false;
        }

        const char *tmp_s;
        int32_t tmp_i;
        JSON_ARRAY *tmp_arr;

        tmp_i = JSON_ObjectGetInt(jlvl_obj, "music", JSON_INVALID_NUMBER);
        if (tmp_i == JSON_INVALID_NUMBER) {
            LOG_ERROR("level %d: 'music' must be a number", level_num);
            return false;
        }
        cur->music = tmp_i;

        tmp_s = JSON_ObjectGetString(jlvl_obj, "file", JSON_INVALID_STRING);
        if (tmp_s == JSON_INVALID_STRING) {
            LOG_ERROR("level %d: 'file' must be a string", level_num);
            return false;
        }
        cur->level_file = Memory_DupStr(tmp_s);

        tmp_s = JSON_ObjectGetString(jlvl_obj, "title", JSON_INVALID_STRING);
        if (tmp_s == JSON_INVALID_STRING) {
            LOG_ERROR("level %d: 'title' must be a string", level_num);
            return false;
        }
        cur->level_title = Memory_DupStr(tmp_s);

        tmp_s = JSON_ObjectGetString(jlvl_obj, "type", JSON_INVALID_STRING);
        if (tmp_s == JSON_INVALID_STRING) {
            LOG_ERROR("level %d: 'type' must be a string", level_num);
            return false;
        }

        cur->level_type = M_StringToEnumType(tmp_s, m_GameFlowLevelTypeEnumMap);

        switch (cur->level_type) {
        case GFL_TITLE:
        case GFL_TITLE_DEMO_PC:
            if (g_GameFlow.title_level_num != -1) {
                LOG_ERROR(
                    "level %d: there can be only one title level", level_num);
                return false;
            }
            g_GameFlow.title_level_num = level_num;
            break;

        case GFL_GYM:
            if (g_GameFlow.gym_level_num != -1) {
                LOG_ERROR(
                    "level %d: there can be only one gym level", level_num);
                return false;
            }
            g_GameFlow.gym_level_num = level_num;
            break;

        case GFL_LEVEL_DEMO_PC:
        case GFL_NORMAL:
            if (g_GameFlow.first_level_num == -1) {
                g_GameFlow.first_level_num = level_num;
            }
            g_GameFlow.last_level_num = level_num;
            break;

        case GFL_BONUS:
        case GFL_CUTSCENE:
        case GFL_CURRENT:
            break;

        default:
            LOG_ERROR("level %d: unknown level type %s", level_num, tmp_s);
            return false;
        }

        tmp_i = JSON_ObjectGetBool(jlvl_obj, "demo", JSON_INVALID_BOOL);
        if (tmp_i != JSON_INVALID_BOOL) {
            cur->demo = tmp_i;
            g_GameFlow.has_demo |= tmp_i;
        } else {
            cur->demo = 0;
        }

        {
            double value = JSON_ObjectGetDouble(
                jlvl_obj, "draw_distance_fade", JSON_INVALID_NUMBER);
            if (value != JSON_INVALID_NUMBER) {
                cur->draw_distance_fade.override = true;
                cur->draw_distance_fade.value = value;
            } else {
                cur->draw_distance_fade.override = false;
            }
        }

        {
            double value = JSON_ObjectGetDouble(
                jlvl_obj, "draw_distance_max", JSON_INVALID_NUMBER);
            if (value != JSON_INVALID_NUMBER) {
                cur->draw_distance_max.override = true;
                cur->draw_distance_max.value = value;
            } else {
                cur->draw_distance_max.override = false;
            }
        }

        tmp_arr = JSON_ObjectGetArray(jlvl_obj, "water_color");
        if (tmp_arr) {
            cur->water_color.override = true;
            cur->water_color.value.r =
                JSON_ArrayGetDouble(tmp_arr, 0, g_GameFlow.water_color.r);
            cur->water_color.value.g =
                JSON_ArrayGetDouble(tmp_arr, 1, g_GameFlow.water_color.g);
            cur->water_color.value.b =
                JSON_ArrayGetDouble(tmp_arr, 2, g_GameFlow.water_color.b);
        } else {
            cur->water_color.override = false;
        }

        cur->unobtainable.pickups =
            JSON_ObjectGetInt(jlvl_obj, "unobtainable_pickups", 0);

        cur->unobtainable.kills =
            JSON_ObjectGetInt(jlvl_obj, "unobtainable_kills", 0);

        cur->unobtainable.secrets =
            JSON_ObjectGetInt(jlvl_obj, "unobtainable_secrets", 0);

        M_LoadObjectNames(jlvl_obj, "strings", &cur->object_strings);
        M_LoadObjectNames(jlvl_obj, "examine", &cur->examine_strings);

        tmp_i = JSON_ObjectGetBool(jlvl_obj, "inherit_injections", 1);
        tmp_arr = JSON_ObjectGetArray(jlvl_obj, "injections");
        if (tmp_arr) {
            cur->injections.length = tmp_arr->length;
            if (tmp_i) {
                cur->injections.length += g_GameFlow.injections.length;
            }
            cur->injections.data_paths =
                Memory_Alloc(sizeof(char *) * cur->injections.length);

            int inj_base_index = 0;
            if (tmp_i) {
                for (int i = 0; i < g_GameFlow.injections.length; i++) {
                    cur->injections.data_paths[i] =
                        Memory_DupStr(g_GameFlow.injections.data_paths[i]);
                }
                inj_base_index = g_GameFlow.injections.length;
            }

            for (size_t i = 0; i < tmp_arr->length; i++) {
                const char *const str = JSON_ArrayGetString(tmp_arr, i, NULL);
                cur->injections.data_paths[inj_base_index + i] =
                    Memory_DupStr(str);
            }
        } else if (tmp_i) {
            cur->injections.length = g_GameFlow.injections.length;
            cur->injections.data_paths =
                Memory_Alloc(sizeof(char *) * cur->injections.length);
            for (int i = 0; i < g_GameFlow.injections.length; i++) {
                cur->injections.data_paths[i] =
                    Memory_DupStr(g_GameFlow.injections.data_paths[i]);
            }
        } else {
            cur->injections.length = 0;
        }

        tmp_i = JSON_ObjectGetInt(jlvl_obj, "lara_type", (int32_t)O_LARA);
        if (tmp_i < 0 || tmp_i >= O_NUMBER_OF) {
            LOG_ERROR(
                "level %d: 'lara_type' must be a valid game object id",
                level_num);
            return false;
        }
        cur->lara_type = (GAME_OBJECT_ID)tmp_i;

        tmp_arr = JSON_ObjectGetArray(jlvl_obj, "item_drops");
        cur->item_drops.count = 0;
        if (tmp_arr && g_GameFlow.enable_tr2_item_drops) {
            LOG_WARNING(
                "TR2 item drops are enabled: gameflow-defined drops for level "
                "%d will be ignored",
                level_num);
        } else if (tmp_arr) {
            cur->item_drops.count = (signed)tmp_arr->length;
            cur->item_drops.data = Memory_Alloc(
                sizeof(GAME_FLOW_DROP_ITEM_DATA) * (signed)tmp_arr->length);

            for (int i = 0; i < cur->item_drops.count; i++) {
                GAME_FLOW_DROP_ITEM_DATA *data = &cur->item_drops.data[i];
                JSON_OBJECT *jlvl_data = JSON_ArrayGetObject(tmp_arr, i);

                data->enemy_num = JSON_ObjectGetInt(
                    jlvl_data, "enemy_num", JSON_INVALID_NUMBER);
                if (data->enemy_num == JSON_INVALID_NUMBER) {
                    LOG_ERROR(
                        "level %d, item drop %d: 'enemy_num' must be a number",
                        level_num, i);
                    return false;
                }

                JSON_ARRAY *object_arr =
                    JSON_ObjectGetArray(jlvl_data, "object_ids");
                if (!object_arr) {
                    LOG_ERROR(
                        "level %d, item drop %d: 'object_ids' must be an array",
                        level_num, i);
                    return false;
                }

                data->count = (signed)object_arr->length;
                data->object_ids = Memory_Alloc(sizeof(int16_t) * data->count);
                for (int j = 0; j < data->count; j++) {
                    int id = JSON_ArrayGetInt(object_arr, j, -1);
                    if (id < 0 || id >= O_NUMBER_OF) {
                        LOG_ERROR(
                            "level %d, item drop %d, index %d: 'object_id' "
                            "must be a valid object id",
                            level_num, i, j);
                        return false;
                    }
                    data->object_ids[j] = (int16_t)id;
                }
            }
        }

        if (!M_LoadLevelSequence(jlvl_obj, level_num)) {
            return false;
        }

        jlvl_elem = jlvl_elem->next;
        level_num++;
        cur++;
    }

    if (g_GameFlow.title_level_num == -1) {
        LOG_ERROR("at least one level must be of title type");
        return false;
    }
    if (g_GameFlow.first_level_num == -1 || g_GameFlow.last_level_num == -1) {
        LOG_ERROR("at least one level must be of normal type");
        return false;
    }
    return true;
}

bool GameFlow_LoadFromFile(const char *file_name)
{
    GameFlow_Shutdown();
    bool result = false;
    JSON_VALUE *root = NULL;
    char *script_data = NULL;

    if (!File_Load(file_name, &script_data, NULL)) {
        LOG_ERROR("failed to open script file");
        goto cleanup;
    }

    JSON_PARSE_RESULT parse_result;
    root = JSON_ParseEx(
        script_data, strlen(script_data), JSON_PARSE_FLAGS_ALLOW_JSON5, NULL,
        NULL, &parse_result);
    if (!root) {
        LOG_ERROR(
            "failed to parse script file: %s in line %d, char %d",
            JSON_GetErrorDescription(parse_result.error),
            parse_result.error_line_no, parse_result.error_row_no, script_data);
        goto cleanup;
    }

    JSON_OBJECT *root_obj = JSON_ValueAsObject(root);

    result = true;
    result &= M_LoadScriptMeta(root_obj);
    result &= M_LoadScriptGameStrings(root_obj);
    result &= M_LoadScriptLevels(root_obj);

cleanup:
    if (root) {
        JSON_ValueFree(root);
        root = NULL;
    }

    Memory_FreePointer(&script_data);
    return result;
}

void GameFlow_Shutdown(void)
{
    Memory_FreePointer(&g_GameFlow.main_menu_background_path);
    Memory_FreePointer(&g_GameFlow.savegame_fmt_legacy);
    Memory_FreePointer(&g_GameFlow.savegame_fmt_bson);
    Memory_FreePointer(&g_GameInfo.current);

    for (int i = 0; i < g_GameFlow.injections.length; i++) {
        Memory_FreePointer(&g_GameFlow.injections.data_paths[i]);
    }
    Memory_FreePointer(&g_GameFlow.injections.data_paths);

    if (g_GameFlow.levels) {
        for (int i = 0; i < g_GameFlow.level_count; i++) {
            Memory_FreePointer(&g_GameFlow.levels[i].level_file);
            Memory_FreePointer(&g_GameFlow.levels[i].level_title);
            M_StringTableShutdown(g_GameFlow.levels[i].object_strings);
            M_StringTableShutdown(g_GameFlow.levels[i].examine_strings);

            for (int j = 0; j < g_GameFlow.levels[i].injections.length; j++) {
                Memory_FreePointer(
                    &g_GameFlow.levels[i].injections.data_paths[j]);
            }
            Memory_FreePointer(&g_GameFlow.levels[i].injections.data_paths);

            if (g_GameFlow.levels[i].item_drops.count) {
                for (int j = 0; j < g_GameFlow.levels[i].item_drops.count;
                     j++) {
                    Memory_FreePointer(
                        &g_GameFlow.levels[i].item_drops.data[j].object_ids);
                }
                Memory_FreePointer(&g_GameFlow.levels[i].item_drops.data);
            }

            GAME_FLOW_SEQUENCE *seq = g_GameFlow.levels[i].sequence;
            if (seq) {
                while (seq->type != GFS_END) {
                    switch (seq->type) {
                    case GFS_LOADING_SCREEN:
                    case GFS_DISPLAY_PICTURE:
                    case GFS_TOTAL_STATS: {
                        GAME_FLOW_DISPLAY_PICTURE_DATA *data = seq->data;
                        Memory_FreePointer(&data->path);
                        Memory_FreePointer(&data);
                        break;
                    }
                    case GFS_PLAY_FMV:
                    case GFS_MESH_SWAP:
                    case GFS_GIVE_ITEM:
                        Memory_FreePointer(&seq->data);
                        break;
                    case GFS_END:
                    case GFS_START_GAME:
                    case GFS_LOOP_GAME:
                    case GFS_STOP_GAME:
                    case GFS_START_CINE:
                    case GFS_LOOP_CINE:
                    case GFS_LEVEL_STATS:
                    case GFS_EXIT_TO_TITLE:
                    case GFS_EXIT_TO_LEVEL:
                    case GFS_EXIT_TO_CINE:
                    case GFS_SET_CAM_X:
                    case GFS_SET_CAM_Y:
                    case GFS_SET_CAM_Z:
                    case GFS_SET_CAM_ANGLE:
                    case GFS_FLIP_MAP:
                    case GFS_REMOVE_GUNS:
                    case GFS_REMOVE_SCIONS:
                    case GFS_PLAY_SYNCED_AUDIO:
                    case GFS_REMOVE_AMMO:
                    case GFS_REMOVE_MEDIPACKS:
                    case GFS_SETUP_BACON_LARA:
                    case GFS_LEGACY:
                        break;
                    }
                    seq++;
                }
            }
            Memory_FreePointer(&g_GameFlow.levels[i].sequence);
        }
        Memory_FreePointer(&g_GameFlow.levels);
    }
}

GAME_FLOW_COMMAND
GameFlow_InterpretSequence(int32_t level_num, GAME_FLOW_LEVEL_TYPE level_type)
{
    LOG_INFO("level_num=%d level_type=%d", level_num, level_type);

    g_GameInfo.remove_guns = false;
    g_GameInfo.remove_scions = false;
    g_GameInfo.remove_ammo = false;
    g_GameInfo.remove_medipacks = false;

    GAME_FLOW_SEQUENCE *seq = g_GameFlow.levels[level_num].sequence;
    GAME_FLOW_COMMAND command = { .action = GF_EXIT_TO_TITLE };

    while (seq->type != GFS_END) {
        LOG_INFO("seq %d %d", seq->type, seq->data);

        if (!g_Config.gameplay.enable_cine
            && g_GameFlow.levels[level_num].level_type == GFL_CUTSCENE) {
            bool skip;
            switch (seq->type) {
            case GFS_EXIT_TO_TITLE:
            case GFS_EXIT_TO_LEVEL:
            case GFS_EXIT_TO_CINE:
            case GFS_PLAY_FMV:
            case GFS_LEVEL_STATS:
            case GFS_TOTAL_STATS:
                skip = false;
                break;
            default:
                skip = true;
                break;
            }
            if (skip) {
                seq++;
                continue;
            }
        }

        switch (seq->type) {
        case GFS_START_GAME:
            if (!Game_Start((int32_t)(intptr_t)seq->data, level_type)) {
                g_CurrentLevel = -1;
                return (GAME_FLOW_COMMAND) { .action = GF_EXIT_TO_TITLE };
            }
            break;

        case GFS_LOOP_GAME:
            if (level_type != GFL_SAVED
                && level_num != g_GameFlow.first_level_num) {
                Lara_RevertToPistolsIfNeeded();
            }
            Phase_Set(PHASE_GAME, NULL);
            command = Phase_Run();
            if (command.action != GF_NOOP) {
                return command;
            }
            break;

        case GFS_STOP_GAME:
            command = Game_Stop();
            if (command.action != GF_NOOP
                && command.action != GF_LEVEL_COMPLETE) {
                return command;
            }
            if (level_type == GFL_SAVED) {
                if (g_GameFlow.levels[level_num].level_type == GFL_BONUS) {
                    level_type = GFL_BONUS;
                } else {
                    level_type = GFL_NORMAL;
                }
            }
            break;

        case GFS_START_CINE:
            if (level_type != GFL_SAVED) {
                PHASE_CUTSCENE_ARGS *const args =
                    Memory_Alloc(sizeof(PHASE_CUTSCENE_ARGS));
                args->level_num = (int32_t)(intptr_t)seq->data;
                Phase_Set(PHASE_CUTSCENE, args);
            }
            break;

        case GFS_LOOP_CINE:
            if (level_type != GFL_SAVED) {
                command = Phase_Run();
                if (command.action != GF_NOOP
                    && command.action != GF_LEVEL_COMPLETE) {
                    return command;
                }
            }
            break;

        case GFS_PLAY_FMV:
            if (level_type != GFL_SAVED) {
                FMV_Play((char *)seq->data);
            }
            break;

        case GFS_LEVEL_STATS: {
            PHASE *const phase = Phase_Stats_Create((PHASE_STATS_ARGS) {
                .background_path = NULL,
                .level_num = (int32_t)(intptr_t)seq->data,
                .level_type = GFL_NORMAL,
                .show_final_stats = false,
            });
            command = PhaseExecutor_Run(phase);
            Phase_Stats_Destroy(phase);
            if (command.action != GF_NOOP) {
                return command;
            }
            break;
        }

        case GFS_TOTAL_STATS:
            if (g_Config.gameplay.enable_total_stats
                && level_type != GFL_SAVED) {
                const GAME_FLOW_DISPLAY_PICTURE_DATA *data = seq->data;
                PHASE *const phase = Phase_Stats_Create((PHASE_STATS_ARGS) {
                    .background_path = data->path,
                    .level_num = level_num,
                    .level_type =
                        level_type == GFL_BONUS ? GFL_BONUS : GFL_NORMAL,
                    .show_final_stats = true,
                });
                command = PhaseExecutor_Run(phase);
                Phase_Stats_Destroy(phase);
                if (command.action != GF_NOOP) {
                    return command;
                }
            }
            break;

        case GFS_LOADING_SCREEN:
        case GFS_DISPLAY_PICTURE:
            if (seq->type == GFS_LOADING_SCREEN
                && !g_Config.gameplay.enable_loading_screens) {
                break;
            }

            if (level_type == GFL_SAVED) {
                break;
            }

            if (g_CurrentLevel == -1 && !g_Config.gameplay.enable_eidos_logo) {
                break;
            }

            GAME_FLOW_DISPLAY_PICTURE_DATA *data = seq->data;
            PHASE *const phase = Phase_Picture_Create((PHASE_PICTURE_ARGS) {
                .file_name = data->path,
                .display_time = data->display_time,
                .fade_in_time = 1.0,
                .fade_out_time = 1.0,
                .display_time_includes_fades = false,
            });
            command = PhaseExecutor_Run(phase);
            Phase_Picture_Destroy(phase);
            if (command.action != GF_NOOP) {
                return command;
            }
            break;

        case GFS_EXIT_TO_TITLE:
            return (GAME_FLOW_COMMAND) { .action = GF_EXIT_TO_TITLE };

        case GFS_EXIT_TO_LEVEL: {
            int32_t next_level = (int32_t)(intptr_t)seq->data & ((1 << 6) - 1);
            if (g_GameFlow.levels[next_level].level_type == GFL_BONUS
                && !g_GameInfo.bonus_level_unlock) {
                return (GAME_FLOW_COMMAND) { .action = GF_EXIT_TO_TITLE };
            }
            return (GAME_FLOW_COMMAND) {
                .action = GF_START_GAME,
                .param = next_level,
            };
        }

        case GFS_EXIT_TO_CINE:
            return (GAME_FLOW_COMMAND) {
                .action = GF_START_CINE,
                .param = (int32_t)(intptr_t)seq->data & ((1 << 6) - 1),
            };

        case GFS_SET_CAM_X:
            g_CinePosition.pos.x = (int32_t)(intptr_t)seq->data;
            break;
        case GFS_SET_CAM_Y:
            g_CinePosition.pos.y = (int32_t)(intptr_t)seq->data;
            break;
        case GFS_SET_CAM_Z:
            g_CinePosition.pos.z = (int32_t)(intptr_t)seq->data;
            break;
        case GFS_SET_CAM_ANGLE:
            g_CinePosition.rot = (int32_t)(intptr_t)seq->data;
            break;
        case GFS_FLIP_MAP:
            Room_FlipMap();
            break;
        case GFS_PLAY_SYNCED_AUDIO:
            Music_Play((int32_t)(intptr_t)seq->data);
            break;

        case GFS_GIVE_ITEM:
            if (level_type != GFL_SAVED) {
                const GAME_FLOW_GIVE_ITEM_DATA *give_item_data =
                    (const GAME_FLOW_GIVE_ITEM_DATA *)seq->data;
                Inv_AddItemNTimes(
                    give_item_data->object_id, give_item_data->quantity);
            }
            break;

        case GFS_REMOVE_GUNS:
            if (level_type != GFL_SAVED
                && !(g_GameInfo.bonus_flag & GBF_NGPLUS)) {
                g_GameInfo.remove_guns = true;
            }
            break;

        case GFS_REMOVE_SCIONS:
            if (level_type != GFL_SAVED) {
                g_GameInfo.remove_scions = true;
            }
            break;

        case GFS_REMOVE_AMMO:
            if (level_type != GFL_SAVED
                && !(g_GameInfo.bonus_flag & GBF_NGPLUS)) {
                g_GameInfo.remove_ammo = true;
            }
            break;

        case GFS_REMOVE_MEDIPACKS:
            if (level_type != GFL_SAVED) {
                g_GameInfo.remove_medipacks = true;
            }
            break;

        case GFS_MESH_SWAP: {
            const GAME_FLOW_MESH_SWAP_DATA *const swap_data = seq->data;
            Object_SwapMesh(
                swap_data->object1_id, swap_data->object2_id,
                swap_data->mesh_num);
            break;
        }

        case GFS_SETUP_BACON_LARA: {
            int32_t anchor_room = (int32_t)(intptr_t)seq->data;
            if (!BaconLara_InitialiseAnchor(anchor_room)) {
                LOG_ERROR(
                    "Could not anchor Bacon Lara to room %d", anchor_room);
                return (GAME_FLOW_COMMAND) { .action = GF_EXIT_TO_TITLE };
            }
            break;
        }

        case GFS_LEGACY:
            break;

        case GFS_END:
            return command;
        }

        seq++;
    }

    return command;
}

GAME_FLOW_COMMAND
GameFlow_StorySoFar(int32_t level_num, int32_t savegame_level)
{
    LOG_INFO("%d", level_num);

    GAME_FLOW_SEQUENCE *seq = g_GameFlow.levels[level_num].sequence;
    GAME_FLOW_COMMAND command = { .action = GF_EXIT_TO_TITLE };

    while (seq->type != GFS_END) {
        LOG_INFO("seq %d %d", seq->type, seq->data);

        switch (seq->type) {
        case GFS_LOOP_GAME:
        case GFS_STOP_GAME:
        case GFS_LEVEL_STATS:
        case GFS_TOTAL_STATS:
        case GFS_LOADING_SCREEN:
        case GFS_DISPLAY_PICTURE:
        case GFS_GIVE_ITEM:
        case GFS_REMOVE_GUNS:
        case GFS_REMOVE_SCIONS:
        case GFS_REMOVE_AMMO:
        case GFS_REMOVE_MEDIPACKS:
        case GFS_SETUP_BACON_LARA:
        case GFS_LEGACY:
            break;

        case GFS_START_GAME:
            if (level_num == savegame_level) {
                return (GAME_FLOW_COMMAND) { .action = GF_EXIT_TO_TITLE };
            }
            break;

        case GFS_START_CINE: {
            PHASE_CUTSCENE_ARGS *const args =
                Memory_Alloc(sizeof(PHASE_CUTSCENE_ARGS));
            args->level_num = (int32_t)(intptr_t)seq->data;
            Phase_Set(PHASE_CUTSCENE, args);
            break;
        }

        case GFS_LOOP_CINE:
            command = Phase_Run();
            if (command.action != GF_NOOP
                && command.action != GF_LEVEL_COMPLETE) {
                return command;
            }
            break;

        case GFS_PLAY_FMV:
            FMV_Play((char *)seq->data);
            break;

        case GFS_EXIT_TO_TITLE:
            Music_Stop();
            return (GAME_FLOW_COMMAND) { .action = GF_EXIT_TO_TITLE };

        case GFS_EXIT_TO_LEVEL:
            Music_Stop();
            return (GAME_FLOW_COMMAND) {
                .action = GF_START_GAME,
                .param = (int32_t)(intptr_t)seq->data & ((1 << 6) - 1),
            };

        case GFS_EXIT_TO_CINE:
            Music_Stop();
            return (GAME_FLOW_COMMAND) {
                .action = GF_START_CINE,
                .param = (int32_t)(intptr_t)seq->data & ((1 << 6) - 1),
            };

        case GFS_SET_CAM_X:
            g_CinePosition.pos.x = (int32_t)(intptr_t)seq->data;
            break;
        case GFS_SET_CAM_Y:
            g_CinePosition.pos.y = (int32_t)(intptr_t)seq->data;
            break;
        case GFS_SET_CAM_Z:
            g_CinePosition.pos.z = (int32_t)(intptr_t)seq->data;
            break;
        case GFS_SET_CAM_ANGLE:
            g_CinePosition.rot = (int32_t)(intptr_t)seq->data;
            break;
        case GFS_FLIP_MAP:
            Room_FlipMap();
            break;
        case GFS_PLAY_SYNCED_AUDIO:
            Music_Play((int32_t)(intptr_t)seq->data);
            break;

        case GFS_MESH_SWAP: {
            const GAME_FLOW_MESH_SWAP_DATA *const swap_data = seq->data;
            Object_SwapMesh(
                swap_data->object1_id, swap_data->object2_id,
                swap_data->mesh_num);
            break;
        }

        case GFS_END:
            return command;
        }

        seq++;
    }

    return command;
}

int32_t GameFlow_GetLevelCount(void)
{
    return g_GameFlow.level_count;
}

int32_t GameFlow_GetDemoCount(void)
{
    int32_t demo_count = 0;
    for (int32_t i = g_GameFlow.first_level_num; i <= g_GameFlow.last_level_num;
         i++) {
        if (g_GameFlow.levels[i].demo) {
            demo_count++;
        }
    }
    return demo_count;
}

const char *GameFlow_GetLevelFileName(int32_t level_num)
{
    return g_GameFlow.levels[level_num].level_file;
}

const char *GameFlow_GetLevelTitle(int32_t level_num)
{
    return g_GameFlow.levels[level_num].level_title;
}

int32_t GameFlow_GetGymLevelNumber(void)
{
    return g_GameFlow.gym_level_num;
}

void GameFlow_OverrideCommand(const GAME_FLOW_COMMAND command)
{
    g_GameInfo.override_gf_command = command;
}

GAME_FLOW_COMMAND GameFlow_GetOverrideCommand(void)
{
    return g_GameInfo.override_gf_command;
}

void GameFlow_LoadStrings(int32_t level_num)
{
    Object_ResetNames();

    struct {
        GAME_OBJECT_ID object_id;
        const char *name;
    } game_string_defs[] = {
        // clang-format off
        { O_PISTOL_ITEM,        GS(INV_ITEM_PISTOLS) },
        { O_PISTOL_OPTION,      GS(INV_ITEM_PISTOLS) },
        { O_SHOTGUN_ITEM,       GS(INV_ITEM_SHOTGUN) },
        { O_SHOTGUN_OPTION,     GS(INV_ITEM_SHOTGUN) },
        { O_MAGNUM_ITEM,        GS(INV_ITEM_MAGNUM) },
        { O_MAGNUM_OPTION,      GS(INV_ITEM_MAGNUM) },
        { O_UZI_ITEM,           GS(INV_ITEM_UZI) },
        { O_UZI_OPTION,         GS(INV_ITEM_UZI) },
        { O_PISTOL_AMMO_ITEM,   GS(INV_ITEM_PISTOL_AMMO) },
        { O_PISTOL_AMMO_OPTION, GS(INV_ITEM_PISTOL_AMMO) },
        { O_SG_AMMO_ITEM,       GS(INV_ITEM_SHOTGUN_AMMO) },
        { O_SG_AMMO_OPTION,     GS(INV_ITEM_SHOTGUN_AMMO) },
        { O_MAG_AMMO_ITEM,      GS(INV_ITEM_MAGNUM_AMMO) },
        { O_MAG_AMMO_OPTION,    GS(INV_ITEM_MAGNUM_AMMO) },
        { O_UZI_AMMO_ITEM,      GS(INV_ITEM_UZI_AMMO) },
        { O_UZI_AMMO_OPTION,    GS(INV_ITEM_UZI_AMMO) },
        { O_EXPLOSIVE_ITEM,     GS(INV_ITEM_GRENADE) },
        { O_EXPLOSIVE_OPTION,   GS(INV_ITEM_GRENADE) },
        { O_MEDI_ITEM,          GS(INV_ITEM_MEDI) },
        { O_MEDI_OPTION,        GS(INV_ITEM_MEDI) },
        { O_BIGMEDI_ITEM,       GS(INV_ITEM_BIG_MEDI) },
        { O_BIGMEDI_OPTION,     GS(INV_ITEM_BIG_MEDI) },
        { O_PUZZLE_ITEM_1,      GS(INV_ITEM_PUZZLE1) },
        { O_PUZZLE_OPTION_1,    GS(INV_ITEM_PUZZLE1) },
        { O_PUZZLE_ITEM_2,      GS(INV_ITEM_PUZZLE2) },
        { O_PUZZLE_OPTION_2,    GS(INV_ITEM_PUZZLE2) },
        { O_PUZZLE_ITEM_3,      GS(INV_ITEM_PUZZLE3) },
        { O_PUZZLE_OPTION_3,    GS(INV_ITEM_PUZZLE3) },
        { O_PUZZLE_ITEM_4,      GS(INV_ITEM_PUZZLE4) },
        { O_PUZZLE_OPTION_4,    GS(INV_ITEM_PUZZLE4) },
        { O_KEY_ITEM_1,         GS(INV_ITEM_KEY1) },
        { O_KEY_OPTION_1,       GS(INV_ITEM_KEY1) },
        { O_KEY_ITEM_2,         GS(INV_ITEM_KEY2) },
        { O_KEY_OPTION_2,       GS(INV_ITEM_KEY2) },
        { O_KEY_ITEM_3,         GS(INV_ITEM_KEY3) },
        { O_KEY_OPTION_3,       GS(INV_ITEM_KEY3) },
        { O_KEY_ITEM_4,         GS(INV_ITEM_KEY4) },
        { O_KEY_OPTION_4,       GS(INV_ITEM_KEY4) },
        { O_PICKUP_ITEM_1,      GS(INV_ITEM_PICKUP1) },
        { O_PICKUP_OPTION_1,    GS(INV_ITEM_PICKUP1) },
        { O_PICKUP_ITEM_2,      GS(INV_ITEM_PICKUP2) },
        { O_PICKUP_OPTION_2,    GS(INV_ITEM_PICKUP2) },
        { O_LEADBAR_ITEM,       GS(INV_ITEM_LEADBAR) },
        { O_LEADBAR_OPTION,     GS(INV_ITEM_LEADBAR) },
        { O_SCION_ITEM_1,       GS(INV_ITEM_SCION) },
        { O_SCION_ITEM_2,       GS(INV_ITEM_SCION) },
        { O_SCION_ITEM_3,       GS(INV_ITEM_SCION) },
        { O_SCION_ITEM_4,       GS(INV_ITEM_SCION) },
        { O_SCION_OPTION,       GS(INV_ITEM_SCION) },
        { O_COMPASS_OPTION,     GS(INV_ITEM_COMPASS) },
        { O_PASSPORT_OPTION,    GS(INV_ITEM_GAME) },
        { O_PASSPORT_CLOSED,    GS(INV_ITEM_GAME) },
        { O_DETAIL_OPTION,      GS(INV_ITEM_DETAILS) },
        { O_SOUND_OPTION,       GS(INV_ITEM_SOUND) },
        { O_CONTROL_OPTION,     GS(INV_ITEM_CONTROLS) },
        { O_PHOTO_OPTION,       GS(INV_ITEM_LARAS_HOME) },
        { NO_OBJECT,            NULL },
        // clang-format on
    };

    for (int32_t i = 0; game_string_defs[i].object_id != NO_OBJECT; i++) {
        const char *const new_name = game_string_defs[i].name;
        if (new_name != NULL) {
            Object_SetName(game_string_defs[i].object_id, new_name);
        }
    }

    if (level_num >= 0) {
        ASSERT(level_num < g_GameFlow.level_count);
        const GAME_FLOW_LEVEL *const level = &g_GameFlow.levels[level_num];
        M_ScanLevelText(level->object_strings, Object_SetName);
        M_ScanLevelText(level->examine_strings, Object_SetDescription);
    }
}

GAME_FLOW_COMMAND GameFlow_PlayAvailableStory(int32_t slot_num)
{
    GAME_FLOW_COMMAND command = {
        .action = GF_START_GAME,
        .param = g_GameFlow.first_level_num,
    };

    const int32_t savegame_level = Savegame_GetLevelNumber(slot_num);
    while (1) {
        command = GameFlow_StorySoFar(command.param, savegame_level);

        if ((g_GameFlow.levels[command.param].level_type == GFL_NORMAL
             || g_GameFlow.levels[command.param].level_type == GFL_BONUS)
            && command.param >= savegame_level) {
            break;
        }
    }

    return (GAME_FLOW_COMMAND) { .action = GF_EXIT_TO_TITLE };
}

GAME_FLOW_COMMAND GF_ShowInventory(const INVENTORY_MODE inv_mode)
{
    PHASE *const phase = Phase_Inventory_Create(inv_mode);
    const GAME_FLOW_COMMAND gf_cmd = PhaseExecutor_Run(phase);
    Phase_Inventory_Destroy(phase);
    return gf_cmd;
}

GAME_FLOW_COMMAND GF_ShowInventoryKeys(const GAME_OBJECT_ID receptacle_type_id)
{
    if (g_Config.gameplay.enable_auto_item_selection) {
        const GAME_OBJECT_ID object_id = Object_GetCognateInverse(
            receptacle_type_id, g_KeyItemToReceptacleMap);
        InvRing_SetRequestedObjectID(object_id);
    }

    return GF_ShowInventory(INV_KEYS_MODE);
}
