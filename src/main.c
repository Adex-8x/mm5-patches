#include <pmdsky.h>
#include <cot.h>
#include "extern.h"

uint8_t SPECIAL_DBOX_ID;
uint8_t SPECIAL_DBOX_TYPE;
uint16_t SPECIAL_MESSAGE_ID;
struct preprocessor_flags SPECIAL_PREPROCRESSOR_FLAGS;

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

bool GetDsFirmwareNicknameAscii(char* buf)
{
    struct firmware_info firmware_info;
    MemZero(buf, 11);
    GetDsFirmwareInfo(&firmware_info);
    if(firmware_info.nickname_length == 0)
        return false;
    for(int i = 0; i < firmware_info.nickname_length; i++)
    {
        uint16_t nickname_char = firmware_info.nickname[i];
        if(nickname_char == 0)
            break;
        if(nickname_char > 0xFF)
            return false;
        buf[i] = nickname_char;
    }
    return true;
}

bool __attribute__((used)) HandleLowercaseJTag(const char* tag_string, char* buf, int tag_param_count)
{
    if(StrcmpTag(tag_string, "jugador"))
    {
        char nickname[11];
        if(GetDsFirmwareNicknameAscii(nickname))
            SprintfStatic(buf, "[CS:O]%s[CR]", nickname);
        else
            strcpy(buf, "[CS:O]Unknown[CR]");
        return true;
    }
    else if(StrcmpTag(tag_string, "joy"))
    {
        struct clock_info clock_info;
        GetCurrentClockInfo(&clock_info);
        SprintfClockInfo(&clock_info, buf);
        return true;
    }
    return false;
}

bool __attribute__((used)) HandleUppercaseNTag(const char* tag_string, const char* tag_string_param1, const char* tag_string_param2, int tag_param_count)
{
    // Don't mind this tag, you shouldn't ever have the need to use it...
    if(StrcmpTag(tag_string, "N") && tag_param_count >= 3)
    {
        struct portrait_params portrait_params = {
            .monster_id = 0,
            .portrait_emotion = 0,
            .layout_idx = 0,
            .offset_x = 0,
            .offset_y = 0,
            .try_flip = false,
            .has_flip = false,
            .hw_flip = false,
            .allow_default = true,
        };
        int index = AtoiTag(tag_string_param1);
        int emotion = AtoiTag(tag_string_param2);
        portrait_params.monster_id.val = LoadScriptVariableValueAtIndex(NULL, VAR_POSITION_Y, index);
        portrait_params.offset_y = LoadScriptVariableValueAtIndex(NULL, VAR_POSITION_HEIGHT, index);
        if(emotion != 99)
        {
            portrait_params.portrait_emotion.val = emotion;
            ShowPortraitInPortraitBox(LoadScriptVariableValueAtIndex(NULL, VAR_POSITION_X, index), &portrait_params);
        }
        else
            HidePortraitBox(LoadScriptVariableValueAtIndex(NULL, VAR_POSITION_X, index));
        return true;
    }
    return false;
}

void __attribute__((naked)) NoLowercaseJTagFound()
{
    asm("ldr r0,[r13,#0xB4]");
    asm("add r1,r13,#0x1C8");
    asm("mov r2,r6");
    asm("bl HandleLowercaseJTag");
    asm("cmp r0,#0");
    asm("beq LowercaseTagCodeError");
    asm("add r7,r13,#0x1C8");
    asm("b AfterLowercaseTagIsFound");
}

void __attribute__((naked)) NoUppercaseNTagFound()
{
    asm("ldr r0,[r13,#0x70]");
    asm("ldr r1,[r13,#0x74]");
    asm("ldr r2,[r13,#0x78]");
    asm("mov r3,r6");
    asm("bl HandleUppercaseNTag");
    asm("cmp r0,#0");
    asm("beq UppercaseTagCodeError");
    asm("b AfterUppercaseTagIsFound");
}

void __attribute__((used)) CustomUpdateAnything()
{
    SomeGroundModeLoopUpdateFunctionIdk();
    if(LoadScriptVariableValueAtIndex(NULL, VAR_PERFORMANCE_PROGRESS_LIST, 62))
    {
        switch(SPECIAL_DBOX_TYPE)
        {
            case 0:
                if(!IsDialogueBoxActive(SPECIAL_DBOX_ID))
                {
                    if(SPECIAL_MESSAGE_ID < 11626)
                        SPECIAL_MESSAGE_ID++;
                    ShowStringIdInDialogueBox(SPECIAL_DBOX_ID, SPECIAL_PREPROCRESSOR_FLAGS, SPECIAL_MESSAGE_ID, NULL);
                }
                break;
            case 1:
                if(!IsDialogueBoxActive(SPECIAL_DBOX_ID))
                {
                    if(SPECIAL_MESSAGE_ID <= 571)
                    {
                        SPECIAL_MESSAGE_ID++;
                        ShowStringIdInDialogueBox(SPECIAL_DBOX_ID, SPECIAL_PREPROCRESSOR_FLAGS, SPECIAL_MESSAGE_ID, NULL);
                    }
                    else
                    {
                        SaveScriptVariableValueAtIndex(NULL, VAR_PERFORMANCE_PROGRESS_LIST, 62, 0);
                        CloseDialogueBox(SPECIAL_DBOX_ID);
                    }
                }
                break;
        }
    }
}

void __attribute__((used)) CustomCreatePortraitBox(enum screen screen, uint32_t palette_idx, bool framed)
{
    enum screen new_screen = screen;
    if(LoadScriptVariableValue(NULL, VAR_VERSION) == 11621)
        new_screen = LoadScriptVariableValueAtIndex(NULL, VAR_SCENARIO_SUB3, 0);
    CreatePortraitBox(new_screen, palette_idx, framed);
}

void __attribute__((naked)) ManipulateActorLayering()
{
    asm("mov r0,#61");
    asm("bl GetPerformanceFlagWithChecks");
    asm("cmp r0,#0");
    asm("ldr r12,[r7,#0x128]");
    asm("orrne r12,#0x80000000"); // Top Screen
    asm("strne r12,[r7,#0x128]");
    asm("ldrb r0,[r4,#0xa]"); // Original instruction
    asm("b LoadActorLayeringBitfeld+0x4");
}

void __attribute__((naked)) HandleSpecialActorIds()
{
    asm("mov r12,#300");
    asm("add r12,r12,#86");
    asm("cmp r0,r12");
    asm("blt DefaultActorNameColor");
    asm("add r12,r12,#8");
    asm("cmp r0,r12");
    asm("beq UseYellowName");
    asm("sub r12,r12,#4");
    asm("cmp r0,r12");
    asm("bgt DefaultActorNameColor");
    asm("b UseYellowName");
}
