#include "game/option/option_compass.h"

#include "game/game_string.h"
#include "game/gameflow.h"
#include "game/input.h"
#include "game/text.h"
#include "global/const.h"
#include "global/types.h"
#include "global/vars.h"

#include <libtrx/config.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef enum {
    TEXT_TITLE = 0,
    TEXT_TITLE_BORDER = 1,
    TEXT_TIME = 2,
    TEXT_SECRETS = 3,
    TEXT_PICKUPS = 4,
    TEXT_DEATHS = 5,
    TEXT_KILLS = 6,
    TEXT_NUMBER_OF = 7,
} COMPASS_TEXT;

static TEXTSTRING *m_Text[TEXT_NUMBER_OF] = {};
static int16_t m_CompassNeedle = 0;
static int16_t m_CompassSpeed = 0;

static void M_InitText(void);
static void M_ShutdownText(void);

static void M_InitText(void)
{
    char buf[100];
    const int top_y = -60;
    const int border = 4;
    const int row_height = 25;
    const int row_width = 225;
    const GAME_STATS *stats = &g_GameInfo.current[g_CurrentLevel].stats;

    int y = top_y;
    m_Text[TEXT_TITLE_BORDER] = Text_Create(0, y - border, " ");
    Text_CentreH(m_Text[TEXT_TITLE_BORDER], true);
    Text_CentreV(m_Text[TEXT_TITLE_BORDER], true);

    sprintf(buf, "%s", g_GameFlow.levels[g_CurrentLevel].level_title);
    m_Text[TEXT_TITLE] = Text_Create(0, y - border / 2, buf);
    Text_CentreH(m_Text[TEXT_TITLE], true);
    Text_CentreV(m_Text[TEXT_TITLE], true);
    y += row_height;

    // kills
    sprintf(
        buf,
        g_Config.gameplay.enable_detailed_stats ? GS(STATS_KILLS_DETAIL_FMT)
                                                : GS(STATS_KILLS_BASIC_FMT),
        stats->kill_count, stats->max_kill_count);
    m_Text[TEXT_KILLS] = Text_Create(0, y, buf);
    y += row_height;

    // pickups
    sprintf(
        buf,
        g_Config.gameplay.enable_detailed_stats ? GS(STATS_PICKUPS_DETAIL_FMT)
                                                : GS(STATS_PICKUPS_BASIC_FMT),
        stats->pickup_count, stats->max_pickup_count);
    m_Text[TEXT_PICKUPS] = Text_Create(0, y, buf);
    y += row_height;

    // secrets
    int secret_count = 0;
    int secret_flags = stats->secret_flags;
    for (int i = 0; i < MAX_SECRETS; i++) {
        if (secret_flags & 1) {
            secret_count++;
        }
        secret_flags >>= 1;
    }
    sprintf(
        buf, GS(STATS_SECRETS_FMT), secret_count,
        g_GameInfo.current[g_CurrentLevel].stats.max_secret_count);
    m_Text[TEXT_SECRETS] = Text_Create(0, y, buf);
    y += row_height;

    // deaths
    if (g_Config.gameplay.enable_deaths_counter
        && g_GameInfo.death_counter_supported) {
        sprintf(buf, GS(STATS_DEATHS_FMT), stats->death_count);
        m_Text[TEXT_DEATHS] = Text_Create(0, y, buf);
        y += row_height;
    }

    // time taken
    m_Text[TEXT_TIME] = Text_Create(0, y, " ");
    y += row_height;

    Text_AddBackground(
        m_Text[TEXT_TITLE_BORDER], row_width, y - top_y, 0, 0, TS_BACKGROUND);
    Text_AddOutline(m_Text[TEXT_TITLE_BORDER], TS_BACKGROUND);
    Text_AddBackground(m_Text[TEXT_TITLE], row_width - 4, 0, 0, 0, TS_HEADING);
    Text_AddOutline(m_Text[TEXT_TITLE], TS_HEADING);

    for (int i = 0; i < TEXT_NUMBER_OF; i++) {
        Text_CentreH(m_Text[i], 1);
        Text_CentreV(m_Text[i], 1);
    }
}

static void M_ShutdownText(void)
{
    for (int i = 0; i < TEXT_NUMBER_OF; i++) {
        Text_Remove(m_Text[i]);
        m_Text[i] = NULL;
    }
}

void Option_Compass_Control(INVENTORY_ITEM *inv_item)
{
    if (g_Config.gameplay.enable_compass_stats) {
        char buf[100];
        char time_buf[100];

        if (!m_Text[0]) {
            M_InitText();
        }

        int32_t seconds =
            g_GameInfo.current[g_CurrentLevel].stats.timer / LOGIC_FPS;
        int32_t hours = seconds / 3600;
        int32_t minutes = (seconds / 60) % 60;
        seconds %= 60;
        if (hours) {
            sprintf(
                time_buf, "%d:%d%d:%d%d", hours, minutes / 10, minutes % 10,
                seconds / 10, seconds % 10);
        } else {
            sprintf(time_buf, "%d:%d%d", minutes, seconds / 10, seconds % 10);
        }
        sprintf(buf, GS(STATS_TIME_TAKEN_FMT), time_buf);
        Text_ChangeText(m_Text[TEXT_TIME], buf);
    }

    if (g_InputDB.menu_confirm || g_InputDB.menu_back) {
        M_ShutdownText();
        inv_item->goal_frame = inv_item->frames_total - 1;
        inv_item->anim_direction = 1;
    }
}

void Option_Compass_Shutdown(void)
{
    M_ShutdownText();
}

void Option_Compass_UpdateNeedle(const INVENTORY_ITEM *const inv_item)
{
    if (g_LaraItem == NULL) {
        return;
    }
    int16_t delta = -inv_item->y_rot - g_LaraItem->rot.y - m_CompassNeedle;
    m_CompassSpeed = m_CompassSpeed * 19 / 20 + delta / 50;
    m_CompassNeedle += m_CompassSpeed;
}

int16_t Option_Compass_GetNeedleAngle(void)
{
    return m_CompassNeedle;
}
