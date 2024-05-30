#pragma once

#include <pmdsky.h>
#include <cot.h>

extern uint8_t SPECIAL_DBOX_ID;
extern uint8_t SPECIAL_DBOX_TYPE;
extern uint16_t SPECIAL_MESSAGE_ID;
extern struct preprocessor_flags SPECIAL_PREPROCRESSOR_FLAGS;

struct button_struct {
  uint16_t bitfield;
  int16_t unk;
};

struct firmware_info {
  uint8_t language;
  uint8_t favorite_color;
  uint8_t birth_month;
  uint8_t birth_day;
  uint16_t nickname[10]; // Yes, NOT char
  int16_t unk_0x18; // Always null? Padding?
  uint16_t nickname_length;
  uint16_t message[26];
  int16_t unk_0x50; // Always null? Padding?
  uint16_t message_length;
};

// Now date & time '2024/05/20 13:02:27'
struct clock_info {
  undefined4 field_0x0;
  int second;
  int minute;
  int hour;
  int day;
  int month;
  int year;
};

// Layouts for portraits
enum portrait_layout_types {
    FACE_POS_STANDARD = 0,
    FACE_POS_ABSCOORD = 1,
    FACE_POS_ABSCOORDFACEL = 2,
    FACE_POS_ABSCOORDFACER = 3,
    FACE_POS_BOTTOM_C_FACER = 4,
    FACE_POS_BOTTOM_L_FACEINW = 5,
    FACE_POS_BOTTOM_R_FACEINW = 6,
    FACE_POS_BOTTOM_L_CENTER = 7,
    FACE_POS_BOTTOM_R_CENTER = 8,
    FACE_POS_BOTTOM_C_FACEL = 9,
    FACE_POS_BOTTOM_L_FACEOUTW = 10,
    FACE_POS_BOTTOM_R_FACEOUTW = 11,
    FACE_POS_BOTTOM_LC_FACEOUTW = 12,
    FACE_POS_BOTTOM_RC_FACEOUTW = 13,
    FACE_POS_TOP_C_FACER = 14,
    FACE_POS_TOP_L_FACEINW = 15,
    FACE_POS_TOP_R_FACEINW = 16,
    FACE_POS_TOP_L_CENTER = 17,
    FACE_POS_TOP_R_CENTER = 18,
    FACE_POS_TOP_C_FACEL = 19,
    FACE_POS_TOP_L_FACEOUTW = 20,
    FACE_POS_TOP_RC_FACER = 21,
    FACE_POS_TOP_LC_FACEOUTW = 22,
    FACE_POS_TOP_RC_FACEOUTW = 23,
};


void ResumeAdvancedMenu(int window_id);
void GetSceneName(char* buf, char* scene_name);
void TextboxSolid();
void TextboxTransparent();
bool MyGetHeldButtons(int controller, struct button_struct* btn_ptr);
bool MyGetPressedButtons(int controller, struct button_struct* btn_ptr);
void se_Play(int se_id, int volume, int idk);
void GetDsFirmwareInfo(struct firmware_info* firmware_info);
void LoadBgpBackground(const char* filepath, undefined unk2, undefined unk3, undefined unk4, undefined unk5, bool top_screen);
void GetCurrentClockInfo(struct clock_info* clock_info);
void SprintfClockInfo(struct clock_info* clock_info, char* buf);
void LoadStaffont(int idx);
void FadeOutTopScreenTopMenu(int duration);
void SomeGroundModeLoopUpdateFunctionIdk();

#define TEXT_STRING_SCENE_SELECTOR_TITLE 8735
#define TEXT_STRING_MYSTERYMAIL_BLURB 8736
#define TEXT_STRING_PARTICIPANT_NAME_START 8737
#define TEXT_STRING_PLAY_SCENE_QUESTION 8763
#define TEXT_STRING_PLAY_FINALE_QUESTION 8764

static void PlaySoundEffect(int se_id) { se_Play(se_id, 0x100, 0x1F); }
