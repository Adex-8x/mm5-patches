#include <pmdsky.h>
#include <cot.h>
#include "extern.h"
#include "top_screen_management.h"
#include "dragndrop.c"

bool process_has_been_called_once = false;

// This function isn't in pmdsky-debug yet, so we have to declare it
// here and add its offset in "symbols/custom_[region].ld".
extern void ChangeGlobalBorderColor(int color_type);

// Special process 100: Change border color
// Based on https://github.com/SkyTemple/eos-move-effects/blob/master/example/process/set_frame_color.asm
static int SpChangeBorderColor(short arg1) {
  ChangeGlobalBorderColor(arg1);
  return 0;
}

/*
    Sets the transparency of the textbox. 0 for solid, anything else for transparency (as seen in dungeon mode).
*/
static int SpSetTextboxTransparency(short arg1) {
  if(arg1 == 0)
    TextboxSolid();
  else
    TextboxTransparent();
  return 0;
}


static uint16_t* dispstat = (uint16_t*)0x04000004;
static uint16_t* bgoff = (uint16_t*)0x04000010;
static uint32_t* ie = (uint32_t*)0x04000210;
static uint32_t* irqfunc = (uint32_t*)0x027E0000;
static int tmpbgoffsets[8];
static int fcount=0;
void shake_hbl() {
  int x;
  if (dispstat[1]<191) {
    int y = (dispstat[1]+1+fcount)>>2;
    if (y&0x8) {
      x = 0x7-(y&0x7);
    } else {
      x = y&0x7;
    }
  } else if (dispstat[1]==191) {
    ++fcount;
  } else {
    int y = (fcount)>>2;
    if (y&0x8) {
      x = 0x7-(y&0x7);
    } else {
      x = y&0x7;
    }
  }
  bgoff[4] = (tmpbgoffsets[2]&0xFFFF)+x+16;
  bgoff[6] = (tmpbgoffsets[3]&0xFFFF)+x+16;
}

/*
    Change HBlank Function
*/
static int spHblSelect(short arg1) {
  if(arg1 == 0) {
    ie[0] = ie[0]&(~0x2);
    dispstat[0] = dispstat[0]&(~0x10);
    irqfunc[1] = 0x02078974;
  }
  else {
    ie[0] = ie[0]|0x2;
    dispstat[0] = dispstat[0]|0x10;
    irqfunc[1] = (uint32_t)shake_hbl;
  }
  return 0;
}

/*
    Checks if certain buttons are being held/pressed.
*/
static bool SpCheckInputStatus(short arg1, short arg2) {
  struct button_struct buttons;
  int controller = 0;
  if(arg2 == 0)
    MyGetPressedButtons(controller, &buttons);
  else
    MyGetHeldButtons(controller, &buttons);
  return buttons.bitfield & arg1 ? true : false;
}

/*
    Creates a special window that will persist even after this process finishes.
*/
static bool SpCreateSpecialWindow(short idx, short optional_message_id)
{
  struct window_params window_params[] = { 
    {.x_offset = 0x1, .y_offset = 0x1, .width = 0x1E, .height = 0x4, .screen = 0x1, .box_type = 0xFF},
    {.x_offset = 0x2, .y_offset = 0x2, .width = 0x1C, .height = 0x14, .screen = 0x0, .box_type = 0xFA},
    {.x_offset = 0xE, .y_offset = 0x2, .width = 0x10, .height = 0x2, .screen = 0x0, .box_type = 0xFD}
  };
  struct preprocessor_flags dbox_flags[] = {
    {.flags_1 = 0b0010, .flags_11 = 0b00},
    {.flags_1 = 0b1110, .flags_11 = 0b10},
    {.flags_1 = 0b0010, .flags_11 = 0b00}
  };
  uint16_t message_ids[] = {11621, 568, 0};
  if(LoadScriptVariableValueAtIndex(NULL, VAR_PERFORMANCE_PROGRESS_LIST, 62) == 0)
    SPECIAL_DBOX_ID = CreateDialogueBox(&(window_params[idx]));
  SPECIAL_DBOX_TYPE = idx;
  SPECIAL_PREPROCRESSOR_FLAGS = dbox_flags[idx];
  SPECIAL_MESSAGE_ID = message_ids[idx] > 0 ? message_ids[idx] : optional_message_id;
  ShowStringIdInDialogueBox(SPECIAL_DBOX_ID, SPECIAL_PREPROCRESSOR_FLAGS, SPECIAL_MESSAGE_ID, NULL);
  SaveScriptVariableValueAtIndex(NULL, VAR_PERFORMANCE_PROGRESS_LIST, 62, 1);
  return true;
}

/*
    Forcibly closes a special window created by SpCreateSpecialWindow, AKA Special Process 254.
*/
static bool SpCloseSpecialWindow()
{
  SaveScriptVariableValueAtIndex(NULL, VAR_PERFORMANCE_PROGRESS_LIST, 62, 0);
  CloseDialogueBox(SPECIAL_DBOX_ID);
  return true;
}

// I had problem when I stack-allocated this. // marius
char temppath[30];

// Called for some special custom processes!

// Set return_val to the return value that should be passed back to the game's script engine. Return true,
// if the special process was handled.
bool CustomScriptSpecialProcessCall(undefined4* unknown, uint32_t special_process_id, short arg1, short arg2, int* return_val) {
  switch (special_process_id) {
    case 22:
    case 24:
    case 25:
    case 26:
    case 27:
      if(!process_has_been_called_once)
        CreateNewSaveMenu();
      process_has_been_called_once = true;
      *return_val = -1; // Negative values cause the script to hang on the process until it receives a non-negative return value!
      return true;
    case 100:
      *return_val = SpChangeBorderColor(arg1);
      return true;
    case 101:
      *return_val = SpSetTextboxTransparency(arg1);
      return true;
    case 102:
      *return_val = SpCheckInputStatus(arg1, arg2);
      return true;
    
    // The marius stuff

    // Display an image on top screen, initializing it if needed
    // The script should make the top screen background does not change
    case 110:
      sprintf(temppath, "CUSTOM/SCREEN/%04d.raw", arg1);
      COT_LOGFMT(COT_LOG_CAT_SPECIAL_PROCESS, "loading top screen raw %s", temppath);
      displayImageOnTopScreen(temppath);
      return true;
    // Return top screen to what it was before it was set by 110 or 112
    case 111:
      topScreenReturnToNormal();
      return true;
    // Top screen drawing mode
    case 112:
      sprintf(temppath, "CUSTOM/DRAWING/%04d.prp", arg1);
      COT_LOGFMT(COT_LOG_CAT_SPECIAL_PROCESS, "loading prp %s", temppath);
      initDrawingOnTopScreen(temppath);
      return true;
    // Draw a trail following the cursor on the bottom screen
    case 113:
      switch (arg1) {
        case 0:
        case 1:
          setTrailEnabled(arg1 == 1);
          break;
        case 2:
          Load3dTrailTextures();
          break;
      }
      return true;
    case 114:
      switch (arg1) {
        case 0:
          loadProjectileData();
          break;
        case 1:
          setShouldRenderProjectile(true);
          break;
        case 2:
          setShouldRenderProjectile(false);
          break;
        case 3:
          projectileStartSpeedUp(arg2);
          break;
        case 4:
          projectileExplode(arg2);
          break;
        case 5:
          projectileFree();
          break;
      }
      return true;
    //Argoniens DragNDrop Patch
    case 115:
      *return_val = checkIfTsValuesAreInDefinedRange(arg1);
      return true;
    case 116:
      *return_val = checkPosValues(arg1);
      return true;
    case 117:
      *return_val = spHblSelect(arg1);
      return true;
    case 254:
      *return_val = SpCreateSpecialWindow(arg1, arg2);
      return true;
    case 255:
      *return_val = SpCloseSpecialWindow();
      return true;
    default:
      return false;
  }
}
