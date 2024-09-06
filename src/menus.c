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
undefined4* SCRIPT_STRUCT_UNK_PTR;
struct preprocessor_flags preprocessor_flags = {.flags_1 = 0b000000010, .timer_2 = true}; // Instant text without waiting for any input!
int SCENE_ITEM_STATES[2];
uint16_t SCENE_CHOICES_STRING_IDS[TOTAL_SCENES+1];

char* SceneOptionEntryFn(char* buffer, int option_id)
{
    int scene_display = option_id + 1;
    if(scene_display < TOTAL_SCENES)
        sprintf(buffer, "[M:S3] Scene %d", scene_display);
    else
        return "[M:S3] Finale";
    return buffer;
}

void CreateCustomControlsChart(int text_string_id)
{
    struct window_params topchart_params = { .update = 0x0202FF10, .x_offset = 0x3, .y_offset = 0x9, .width = 0x1A, .height = 0x9, .screen = 0x1, .box_type = 0xFA };
    struct window_flags topchart_flags;
    CUSTOM_TOPCHART_ID = CreateControlsChart(&topchart_params, topchart_flags, NULL, text_string_id);
}

void CreateSceneSelectorMainMenu(int option_id)
{
    struct window_params menu_params = { .x_offset = 2, .y_offset = 3, .box_type = 0xFC };
    struct window_params dbox_params = { .x_offset = 0x9, .y_offset = 0x12, .width = 0x15, .height = 0x4, .box_type = 0xFC };
    struct window_flags menu_flags = { .a_accept = true, .b_cancel = true, .se_on = true, .set_choice = true, .partial_menu = true, .menu_title = true, .menu_lower_bar = true, .no_accept_button = true };
    struct window_extra_info menu_info = {.set_choice_id = option_id, .title_string_id = TEXT_STRING_SCENE_SELECTOR_TITLE, .title_height = 0x10 };
    int menu_options = TOTAL_SCENES; int menu_options_pp = 5;
    TextboxTransparent();
    CUSTOM_MAIN_MENU_ID = CreateAdvancedMenu(&menu_params, menu_flags, &menu_info, SceneOptionEntryFn, menu_options, menu_options_pp);
    CUSTOM_DBOX_ID = CreateDialogueBox(&dbox_params);
    CreateCustomControlsChart(TEXT_STRING_MYSTERYMAIL_BLURB);
    ShowStringIdInDialogueBox(CUSTOM_DBOX_ID, preprocessor_flags, TEXT_STRING_PARTICIPANT_NAME_START+option_id, NULL);
}

void CreateSceneSelectorSubMenu(int option_id)
{
    struct window_params menu_params = { .x_offset = 0x14, .y_offset = 3, .width = 0xA, .box_type = 0xFC };
    struct window_flags menu_flags = { .a_accept = true, .b_cancel = true, .se_on = true, .menu_title = true};
    struct window_extra_info menu_info = {.title_height = 0x10};
    menu_info.title_string_id = LoadScriptVariableValue(NULL, VAR_DUNGEON_EVENT_LOCAL) < TOTAL_SCENES ? TEXT_STRING_PLAY_SCENE_QUESTION : TEXT_STRING_PLAY_FINALE_QUESTION;
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

void CreateMysteryMailMainMenu()
{
    struct window_params menu_params = { .x_offset = 2, .y_offset = 3, .box_type = 0xFF };
    struct window_flags menu_flags = { .a_accept = true, .b_cancel = false, .se_on = true, .menu_title = true, .partial_menu = true};
    struct window_extra_info menu_info = {.title_string_id = TEXT_STRING_MAIN_MENU_TITLE, .title_height = 0x10};
    struct simple_menu_id_item simple_options[5];
    TextboxTransparent();
    for(int i = 0; i < 4; i++)
    {
        simple_options[i].string_id = i+TEXT_STRING_MAIN_MENU_OPTION_NAME_START;
        simple_options[i]._padding = 0;
        simple_options[i].result_value = i+1;
    }
    simple_options[4].string_id = NULL;
    simple_options[4]._padding = NULL;
    simple_options[4].result_value = NULL;
    CreateCustomControlsChart(TEXT_STRING_MAIN_BLURB);
    CUSTOM_MAIN_MENU_ID = CreateSimpleMenuFromStringIds(&menu_params, menu_flags, &menu_info, simple_options, 5);
}

void CreateSceneStarterMenu()
{
    struct window_params menu_params = { .x_offset = 0x12, .y_offset = 3, .box_type = 0xFC };
    struct window_params dbox_params = { .x_offset = 0x9, .y_offset = 0x12, .width = 0x15, .height = 0x4, .box_type = 0xFC };
    struct window_flags menu_flags = {.a_accept = true, .b_cancel = true, .se_on = true, .menu_title = true, .menu_lower_bar = true, .no_up_down = true, .invisible_cursor = true};
    struct window_extra_info menu_info = {.title_string_id = TEXT_STRING_SCENE_STARTER_TITLE, .title_height = 0x10};
    for(int i = 0; i < TOTAL_SCENES; i++)
        SCENE_CHOICES_STRING_IDS[i] = i+TEXT_STRING_SCENE_STARTER_CHOICES;
    SCENE_CHOICES_STRING_IDS[TOTAL_SCENES] = 0;
    struct options_menu_id_item option_items[2] = {
        {.string_id = TEXT_STRING_SCENE_STARTER_DESC, ._padding = 0, .n_choices = 0, .choices = SCENE_CHOICES_STRING_IDS},
        {.string_id = 0, ._padding = 0, .n_choices = 0, .choices = NULL}
    };
    SCENE_ITEM_STATES[0] = 0;
    SCENE_ITEM_STATES[1] = 0;
    CUSTOM_SUB_MENU_ID = CreateOptionsMenu(&menu_params, menu_flags, &menu_info, option_items, 2, SCENE_ITEM_STATES);
    CUSTOM_DBOX_ID = CreateDialogueBox(&dbox_params);
    ShowStringIdInDialogueBox(CUSTOM_DBOX_ID, preprocessor_flags, TEXT_STRING_PARTICIPANT_NAME_START, NULL);
}

void CreateParticipantCredits()
{
    struct window_params dbox_params = { .x_offset = 0x3, .y_offset = 0x4, .width = 0x1A, .height = 0xE, .screen = SCREEN_SUB, .box_type = 0xFA };
    CUSTOM_MAIN_MENU_ID = CreateDialogueBox(&dbox_params);
    ShowParticipantCredits();
}

void ShowParticipantCredits()
{
    int current_scene = LoadScriptVariableValue(NULL, VAR_DUNGEON_EVENT_LOCAL);
    if(current_scene < TOTAL_SCENES)
    {
        if(current_scene > 0)
            RemoveActingSector(current_scene);
        ShowStringIdInDialogueBox(CUSTOM_MAIN_MENU_ID, preprocessor_flags, TEXT_STRING_PARTICIPANT_CREDITS_START+current_scene, NULL);
        int next_scene = current_scene+1;
        LoadActingSector(next_scene);
        LoadSceneStuff(next_scene);
        SaveScriptVariableValue(NULL, VAR_DUNGEON_EVENT_LOCAL, next_scene);
    }
    else
    {
        CloseDialogueBox(CUSTOM_MAIN_MENU_ID);
        LoadMarkfont();
        LoadMarkfontPal();
        CUSTOM_MAIN_MENU_ID = 0xFF;
    }
}

void GetLowercaseName(const char* src, char* dst)
{
    MemZero(dst, 10);
    for(int i = 0; i < 10 && src[i] != NULL; i++)
        dst[i] = src[i] >= 'A' && src[i] <= 'Z' ? src[i]+0x20 : src[i];
}

/*
  Checks for various names that result in unique dialogue playing during the Name Check!
*/
int GetSpecialNameCategory(const char* buffer)
{
    int category = -1;
    char lowercase_name[10];
    char* script_string = GetScriptString(SCRIPT_STRUCT_UNK_PTR, 0);
    MemZero(name_check_string, 11);
    strncpy(name_check_string, buffer, 10);
    GetLowercaseName(buffer, lowercase_name);
    struct file_stream file;
    DataTransferInit();
    FileInit(&file);
    FileOpen(&file, "CUSTOM/NAME/onamaewa.bin");
    int size = FileGetSize(&file);
    struct special_check* onamaewa_buffer = MemAlloc(size, 0);
    int read_bytes = FileRead(&file, onamaewa_buffer, size);
    FileClose(&file);
    DataTransferStop();
    if(read_bytes > 0 && strlen(script_string) == size)
    {
        char* encrypted_buffer = onamaewa_buffer;
        for(int i = 0; i < size; i++)
            encrypted_buffer[i] ^= script_string[i];
        for(int i = 0; i < __divsi3(size, sizeof(struct special_check)); i++)
        {
            struct special_check entry = onamaewa_buffer[i];
            if(entry.category == 0)
                break;
            int length = entry.length;
            if(strncmp(lowercase_name, entry.name, length) == 0)
            {
                category = entry.category;
                break;
            }
        }
    }
    MemFree(onamaewa_buffer);
    return category;
}

/*
  ???
*/
int TryAcceptClassifiedCategory(int himitsu_index)
{
    int category = 581;
    struct file_stream file;
    char* script_strings[2];
    DataTransferInit();
    FileInit(&file);
    FileOpen(&file, "CUSTOM/NAME/door.bin");
    int size = FileGetSize(&file);
    struct himitsu_check* himitsu_buffer = MemAlloc(size, 0);
    char* shenanigans = himitsu_buffer;
    int read_bytes = FileRead(&file, himitsu_buffer, size);
    FileClose(&file);
    DataTransferStop();
    if(read_bytes > 0 && shenanigans[0] != 'P' && shenanigans[1] != 'K')
    {
        for(int i = 0; i < size>>1; i++)
        {
            int j = size-1-i;
            if(i != j)
            {
                shenanigans[i] ^= shenanigans[j];
                shenanigans[j] ^= shenanigans[i];
                shenanigans[i] ^= shenanigans[j];
                shenanigans[i] ^= 0xFF;
                shenanigans[j] ^= 0xFF;
            }
        }
        struct himitsu_check entry = himitsu_buffer[himitsu_index];
        for(int i = 0; i < 2; i ++)
        {
            script_strings[i] = GetScriptString(SCRIPT_STRUCT_UNK_PTR, i+1);
            MemZero(script_strings[i], strlen(script_strings[i]));
            strncpy(script_strings[i], shenanigans+entry.offsets[i], entry.length[i]);
        }
        category = himitsu_index+200;
    }
    MemFree(himitsu_buffer);
    return category;
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
            CreateSceneSelectorMainMenu(last_played_scene > 0 && last_played_scene <= TOTAL_SCENES ? last_played_scene-1 : 0);
            break;
        case 101:
            CreateMysteryMailMainMenu();
            break;
        case 102:
            LoadStaffont(0);
            SaveScriptVariableValue(NULL, VAR_DUNGEON_EVENT_LOCAL, 0);
            CreateParticipantCredits();
            break;
        case 103:
            SetupKeyboard(3, NULL, NULL);
            CUSTOM_MAIN_MENU_ID = 0xFE;
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
                            if(LAST_MENU_RESULT >= 0)
                            {
                                SaveScriptVariableValue(NULL, VAR_DUNGEON_EVENT_LOCAL, LAST_MENU_RESULT+1);
                                DelayMenuFunc = CreateSceneSelectorSubMenu;
                                FRAME_COUNTER = 1;
                                MENU_STATE++;
                            }
                            else
                            {
                                CloseAdvancedMenu(CUSTOM_MAIN_MENU_ID);
                                CloseDialogueBox(CUSTOM_DBOX_ID);
                                CloseControlsChart(CUSTOM_TOPCHART_ID);
                                MENU_STATE = 3;
                            }
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
                    case 2:
                        return_val = 1;
                        TextboxSolid();
                        break;
                    case 3:
                        return_val = 2;
                        TextboxSolid();
                        break;
                }
                break;
            case 101:
                switch(MENU_STATE)
                {
                    case 0:
                        if(!IsSimpleMenuActive(CUSTOM_MAIN_MENU_ID))
                        {
                            LAST_MENU_RESULT = GetSimpleMenuResult(CUSTOM_MAIN_MENU_ID);
                            if(LAST_MENU_RESULT == 1)
                            {
                                DelayMenuFunc = CreateSceneStarterMenu;
                                FRAME_COUNTER = 1;
                                MENU_STATE++;
                            }
                            else
                            {
                                CloseSimpleMenu(CUSTOM_MAIN_MENU_ID);
                                CloseControlsChart(CUSTOM_TOPCHART_ID);
                                MENU_STATE = 2;
                            }
                        }
                        break;
                    case 1:
                        if(!IsOptionsMenuActive(CUSTOM_SUB_MENU_ID))
                        {
                            LAST_MENU_RESULT = GetOptionsMenuResult(CUSTOM_SUB_MENU_ID); // Just a guess
                            CloseOptionsMenu(CUSTOM_SUB_MENU_ID);
                            if(LAST_MENU_RESULT != 0)
                            {
                                SaveScriptVariableValue(NULL, VAR_DUNGEON_EVENT_LOCAL, SCENE_ITEM_STATES[0]+1);
                                CloseSimpleMenu(CUSTOM_MAIN_MENU_ID);
                                CloseControlsChart(CUSTOM_TOPCHART_ID);
                                CloseDialogueBox(CUSTOM_DBOX_ID);
                                LAST_MENU_RESULT = 1;
                                MENU_STATE++;
                            }
                            else
                            {
                                ResumeSimpleMenu(CUSTOM_MAIN_MENU_ID);
                                CloseDialogueBox(CUSTOM_DBOX_ID);
                                MENU_STATE = 0;
                            }
                        }
                        else
                        {
                            GetOptionsMenuAllChoices(CUSTOM_SUB_MENU_ID, SCENE_ITEM_STATES);
                            current_menu_option = SCENE_ITEM_STATES[0];
                            if(current_menu_option != LAST_DISPLAYED_OPTION)
                            {
                                ShowStringIdInDialogueBox(CUSTOM_DBOX_ID, preprocessor_flags, TEXT_STRING_PARTICIPANT_NAME_START+current_menu_option, NULL);
                                LAST_DISPLAYED_OPTION = current_menu_option;
                            }
                        }
                        break;
                    default:
                        return_val = LAST_MENU_RESULT;
                        TextboxSolid();
                        break;
                }
                break;
            case 102:
                FRAME_COUNTER = 360;
                DelayMenuFunc = ShowParticipantCredits;
                break;
            case 103:
                if(IsMenuFinished)
                {
                    return_val = GetSpecialNameCategory(GetKeyboardStringResult());
                    if(return_val >= 200 && return_val <= 213)
                        return_val = TryAcceptClassifiedCategory(return_val-200);
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
    asm("bl NewMenuEnd");
    asm("str r0,[r6]");
    asm("b MenuEndFinish");
}

int __attribute__((used)) CustomScriptMenuRequest(int menu_id, undefined4* param_2, undefined4* param_3)
{
    SCRIPT_STRUCT_UNK_PTR = param_3;
    return ScriptMenuRequest(menu_id, param_2);
}

void __attribute__((naked)) ScriptMenuRequestHook()
{
    asm("add r2,r4,#0x14");
    asm("b CustomScriptMenuRequest");
}