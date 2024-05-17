#pragma once

#include <pmdsky.h>
#include <cot.h>

struct button_struct {
  int16_t bitfield;
  int16_t unk;
};

void ResumeAdvancedMenu(int window_id);
void GetSceneName(char* buf, char* scene_name);
void TextboxSolid();
void TextboxTransparent();
bool MyGetHeldButtons(int controller, struct button_struct* btn_ptr);
bool MyGetPressedButtons(int controller, struct button_struct* btn_ptr);

#define TEXT_STRING_SCENE_SELECTOR_TITLE 8735
#define TEXT_STRING_MYSTERYMAIL_BLURB 8736
#define TEXT_STRING_PARTICIPANT_NAME_START 8737
#define TEXT_STRING_PLAY_SCENE_QUESTION 8763
#define TEXT_STRING_PLAY_FINALE_QUESTION 8764