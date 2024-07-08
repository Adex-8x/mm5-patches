#include <pmdsky.h>
#include <cot.h>
#include "extern.h"

uint8_t CUSTOM_MAIN_MENU_ID = 0xFF;
uint8_t CUSTOM_SUB_MENU_ID = 0xFF;
uint8_t CUSTOM_DBOX_ID = 0xFF;
uint8_t CUSTOM_TOPCHART_ID = 0xFF;
uint8_t LAST_DISPLAYED_OPTION = 0x0;
int MENU_STATE = 0x0;
int LAST_MENU_RESULT = 0x0;
int FRAME_COUNTER = -1;
void (*DelayMenuFunc)(int);
struct preprocessor_flags preprocessor_flags = {.flags_1 = 0b000000010, .timer_2 = true}; // Instant text without waiting for any input!

char* SceneOptionEntryFn(char* buffer, int option_id)
{
    // TODO: "Play All", "Credits", and "Name Check" options
    int scene_display = option_id + 1;
    if(scene_display < 26)
        sprintf(buffer, "[M:S3] Scene %d", scene_display);
    else
        return "[M:S3] Finale";
    return buffer;
}

void CreateSceneSelectorMainMenu(int option_id)
{
    struct window_params menu_params = { .x_offset = 2, .y_offset = 3, .box_type = 0xFC };
    struct window_params topchart_params = { .update = 0x0202FF10, .x_offset = 0x3, .y_offset = 0x9, .width = 0x1A, .height = 0x9, .screen = 0x1, .box_type = 0xFA };
    struct window_params dbox_params = { .x_offset = 0x9, .y_offset = 0x12, .width = 0x15, .height = 0x4, .box_type = 0xFC };
    struct window_flags menu_flags = { .a_accept = true, .b_cancel = false, .se_on = true, .set_choice = true, .partial_menu = true, .menu_title = true, .menu_lower_bar = true, .no_accept_button = true };
    struct window_flags topchart_flags;
    struct window_extra_info menu_info = {.set_choice_id = option_id, .title_string_id = TEXT_STRING_SCENE_SELECTOR_TITLE, .title_height = 0x10 };
    int menu_options = 26; int menu_options_pp = 5;
    TextboxTransparent();
    CUSTOM_MAIN_MENU_ID = CreateAdvancedMenu(&menu_params, menu_flags, &menu_info, SceneOptionEntryFn, menu_options, menu_options_pp);
    CUSTOM_TOPCHART_ID = CreateControlsChart(&topchart_params, topchart_flags, NULL, TEXT_STRING_MYSTERYMAIL_BLURB);
    CUSTOM_DBOX_ID = CreateDialogueBox(&dbox_params);
    ShowStringIdInDialogueBox(CUSTOM_DBOX_ID, preprocessor_flags, TEXT_STRING_PARTICIPANT_NAME_START+option_id, NULL);
}

void CreateSceneSelectorSubMenu(int option_id)
{
    struct window_params menu_params = { .x_offset = 0x14, .y_offset = 3, .width = 0xA, .box_type = 0xFC };
    struct window_flags menu_flags = { .a_accept = true, .b_cancel = true, .se_on = true, .menu_title = true};
    struct window_extra_info menu_info = {.title_height = 0x10};
    menu_info.title_string_id = LoadScriptVariableValue(NULL, VAR_DUNGEON_EVENT_LOCAL) < 26 ? TEXT_STRING_PLAY_SCENE_QUESTION : TEXT_STRING_PLAY_FINALE_QUESTION;
    struct simple_menu_id_item simple_options[3];
    for(int i = 0; i < 2; i++)
    {
        simple_options[i].string_id = i+422;
        simple_options[i]._padding = 0;
        simple_options[i].result_value = i+1;
    }
    simple_options[2].string_id = NULL;
    simple_options[2]._padding = NULL;
    simple_options[2].result_value = NULL;
    CUSTOM_SUB_MENU_ID = CreateSimpleMenuFromStringIds(&menu_params, menu_flags, &menu_info, simple_options, 3);
}

/*
  Creates an important message for someone who tries to do...undesirable actions.
  Given the conditions in which this function is called, there surely aren't any other exploitative interactions.

  Right...?
  Certain special processes and menus...SURELY, no other opcode can put up a fight.
*/
void __attribute__((used)) CreateNewSaveMenu()
{
    struct window_params unk_params = { .update = 0x0202FF10, .x_offset = 0x2, .y_offset = 0x2, .width = 0x1C, .height = 0x14, .screen = 0x0, .box_type = 0xFF };
    struct window_flags unk_flags;
    StopBgmCommand();
    PlaySoundEffect(6415);
    CUSTOM_MAIN_MENU_ID = CreateImportantWarning(&unk_params, unk_flags, NULL);
}

/*
  Initializes menus with IDs that are greater than what the base game expects!
*/
void __attribute__((used)) NewMenuStart(int menu_id)
{
    CUSTOM_MAIN_MENU_ID = 0xFF;
    CUSTOM_SUB_MENU_ID = 0xFF;
    CUSTOM_DBOX_ID = 0xFF;
    CUSTOM_TOPCHART_ID = 0xFF;
    MENU_STATE = 0x0;
    FRAME_COUNTER = -1;
    switch(menu_id)
    {
        case 11:
            CreateNewSaveMenu();
            break;
        case 100:
            int last_played_scene = LoadScriptVariableValue(NULL, VAR_DUNGEON_EVENT_LOCAL);
            CreateSceneSelectorMainMenu(last_played_scene > 0 && last_played_scene <= 26 ? last_played_scene-1 : 0);
            break;
    }
}

/*
  Handles menus with IDs that are greater than what the base game expects!
  Although this is typically the "end" of menus, this function gets called every frame when a menu is active.
  As such, we have to determine when a menu is active, how to display various components, etc...

  One obstacle I have with menu stuff is delaying when windows get created, hence the FRAME_COUNTER jank and the function pointer DelayMenuFunc.
  Although very ugly, it helps to delay certain window creations and keep things displaying smoothly.
*/
int __attribute__((used)) NewMenuEnd(int menu_id)
{
    int return_val = 0;
    int current_menu_option = 0;
    if(CUSTOM_MAIN_MENU_ID == 0xFF)
        return CUSTOM_MAIN_MENU_ID;
    if(FRAME_COUNTER < 0)
    {
        switch(menu_id)
        {
            case 11:
                break;
            case 100:
                switch(MENU_STATE)
                {
                    case 0:
                        if(!IsAdvancedMenuActive2(CUSTOM_MAIN_MENU_ID))
                        {
                            LAST_MENU_RESULT = GetAdvancedMenuResult(CUSTOM_MAIN_MENU_ID);
                            SaveScriptVariableValue(NULL, VAR_DUNGEON_EVENT_LOCAL, LAST_MENU_RESULT+1);
                            DelayMenuFunc = CreateSceneSelectorSubMenu;
                            FRAME_COUNTER = 1;
                            MENU_STATE++;
                        }
                        else
                        {
                            current_menu_option = GetAdvancedMenuCurrentOption(CUSTOM_MAIN_MENU_ID);
                            if(current_menu_option != LAST_DISPLAYED_OPTION)
                            {
                                ShowStringIdInDialogueBox(CUSTOM_DBOX_ID, preprocessor_flags, TEXT_STRING_PARTICIPANT_NAME_START+current_menu_option, NULL);
                                LAST_DISPLAYED_OPTION = current_menu_option;
                            }
                        }
                        break;
                    case 1:
                        if(!IsSimpleMenuActive(CUSTOM_SUB_MENU_ID))
                        {
                            LAST_MENU_RESULT = GetSimpleMenuResult(CUSTOM_SUB_MENU_ID);
                            CloseSimpleMenu(CUSTOM_SUB_MENU_ID);
                            if(LAST_MENU_RESULT == 1)
                            {
                                CloseAdvancedMenu(CUSTOM_MAIN_MENU_ID);
                                CloseDialogueBox(CUSTOM_DBOX_ID);
                                CloseControlsChart(CUSTOM_TOPCHART_ID);
                                MENU_STATE++;
                            }
                            else
                            {
                                ResumeAdvancedMenu(CUSTOM_MAIN_MENU_ID);
                                MENU_STATE--;
                            }
                        }
                        break;
                    default:
                        return_val = 1;
                        TextboxSolid();
                        break;
                }
                break;
            default:
                return_val = 0xFF;
                break;
        }
    }
    else
    {
        if(FRAME_COUNTER == 0)
            (*DelayMenuFunc)(LAST_MENU_RESULT); // This is sooooooooooooo so so so so cursed
        FRAME_COUNTER--;
    }
    return return_val;
}

/*
  Hook for custom menu initializations!
*/
void __attribute__((naked)) MenuStartHook()
{
    asm("mov r0,r5");
    asm("bl NewMenuStart");
    asm("b MenuStartFinish");
}

/*
  Hook for custom menu handling!
*/
void __attribute__((naked)) MenuEndHook()
{
    // For potential keyboard menus in the future...
        // asm("cmp r0,#200");
        // asm("beq MessageMenuKeyboardEnd");
    asm("bl NewMenuEnd");
    asm("str r0,[r6]");
    asm("b MenuEndFinish");
}
