#pragma once

#include <pmdsky.h>
#include <cot.h>

// The following variables are for "special" windows that persist via calling Special Process 254.
extern uint8_t SPECIAL_DBOX_ID;
extern uint8_t SPECIAL_DBOX_TYPE;
extern uint16_t SPECIAL_MESSAGE_ID;
extern struct preprocessor_flags SPECIAL_PREPROCRESSOR_FLAGS;

// The struct used when calling GetPressedButtons and GetHeldButtons.
struct button_struct {
  uint16_t bitfield;
  int16_t unk;
};

// The struct used to organize various pieces of info about the the DS firmware user settings.
struct firmware_info {
  uint8_t language;
  uint8_t favorite_color;
  uint8_t birth_month;
  uint8_t birth_day;
  uint16_t nickname[10]; // Yes, NOT char; this is UTF-16
  int16_t unk_0x18; // Always null? Padding?
  uint16_t nickname_length;
  uint16_t message[26];
  int16_t unk_0x50; // Always null? Padding?
  uint16_t message_length;
};

// The struct used to organize the DS system clock's settings.
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



// Some video-related control register
volatile extern int32_t DISPCNT_B;
volatile extern int8_t VRAMCNT_C;
volatile extern int16_t BG3CNT_B;

// Store wether some background should be disable, per-screen
struct EngineDisplayInfo {
    bool disable_bg0;
    bool disable_bg1;
    bool disable_bg2;
    bool disable_bg3;
    bool disable_obj;
};
extern struct EngineDisplayInfo ENGINE_DISPLAY_INFO[2];

// some atan stuff to convert a coordinate to angle
uint16_t FX_Atan2Idx(int32_t y, int32_t x);

//Touchscreen Variables
extern uint8_t TSXPosLive;
extern uint8_t TSYPosLive;
extern uint8_t TSXPosLastMitDrag;
extern uint8_t TSYPosLastMitDrag;
extern uint8_t TSPressed;

// texture loading related functions
void ProcessWTEWrapper(struct wte_handle *wte_ref,undefined4 dest_vram_offset,undefined4 palette_high_byte, undefined4 palette_low_byte);
void DoSomethingOn3dAllocAndClearInput(void **wte_ref);

// camera stuff
extern struct vec2_int CAMERA_TOP_LEFT_COORD;

// 3d engine stuff
volatile extern int GEOM_COMM_BEGIN_VTXS;
volatile extern int GEOM_COMM_END_VTXS;
volatile extern int GEOM_COMM_VTX_16;
volatile extern int GEOM_COMM_MTX_PUSH;
volatile extern int GEOM_COMM_MTX_POP;
volatile extern int GEOM_COMM_MTX_SCALE;
volatile extern int GEOM_COMM_MTX_IDENTITY;
volatile extern int GEOM_COMM_MTX_TRANS;
volatile extern int GEOM_COMM_MTX_MODE;
volatile extern int GEOM_COMM_POLYGON_ATTR;
volatile extern int GEOM_COMM_COLOR;
volatile extern int GEOM_COMM_MTX_LOAD_4x4;
volatile extern int GEOM_COMM_PLTT_BASE;
volatile extern int GEOM_COMM_MTX_MULT_3x3;

// Not really extern, defined in an asm file
// copied from libnds
// Argument 1: a pointer
// Argument 2: a size of datas following the pointer in bytes
// Flush the range of data from the cache
void DC_FlushRange(const void *base, uint32_t size);