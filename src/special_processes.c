#include <pmdsky.h>
#include <cot.h>
#include "extern.h"

// This function isn't in pmdsky-debug yet, so we have to declare it
// here and add its offset in "symbols/custom_[region].ld".
extern void ChangeGlobalBorderColor(int color_type);

// Special process 100: Change border color
// Based on https://github.com/SkyTemple/eos-move-effects/blob/master/example/process/set_frame_color.asm
static int SpChangeBorderColor(short arg1) {
  ChangeGlobalBorderColor(arg1);
  return 0;
}

static int SpSetTextboxTransparency(short arg1) {
  if(arg1 == 0)
    TextboxSolid();
  else
    TextboxTransparent();
  return 0;
}

static bool SpCheckInputStatus(short arg1, short arg2) {
  struct button_struct buttons;
  int controller = 0;
  if(arg2 == 0)
    MyGetPressedButtons(controller, &buttons);
  else
    MyGetHeldButtons(controller, &buttons);
  return buttons.bitfield & arg1 ? true : false;
}


// Called for special process IDs 100 and greater.
//
// Set return_val to the return value that should be passed back to the game's script engine. Return true,
// if the special process was handled.
bool CustomScriptSpecialProcessCall(undefined4* unknown, uint32_t special_process_id, short arg1, short arg2, int* return_val) {
  switch (special_process_id) {
    case 22:
    case 23:
    case 25:
    case 26:
    case 27:
      CardPullOut();
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
    default:
      return false;
  }
}
