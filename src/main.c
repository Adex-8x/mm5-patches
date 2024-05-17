#include <pmdsky.h>
#include <cot.h>
#include "extern.h"

void __attribute__((used)) CustomGetSceneName(char* buf, char* scene_name)
{
    if(strncmp(scene_name, "event", 8) == 0)
    {
        int scene_number = LoadScriptVariableValue(NULL, VAR_DUNGEON_EVENT_LOCAL);
        if(scene_number >= 26)
            strncpy(buf, "finale", 8);
        else
            sprintf(buf, "%02d", scene_number);
    }
    else
        GetSceneName(buf, scene_name);
}
