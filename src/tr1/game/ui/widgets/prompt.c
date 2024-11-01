#include "global/const.h"

#include <libtrx/game/ui/widgets/prompt.h>

const char *UI_Prompt_GetPromptChar(void)
{
    return "\\{button left}";
}

int32_t UI_Prompt_GetCaretFlashRate(void)
{
    return LOGIC_FPS * 0.75;
}
