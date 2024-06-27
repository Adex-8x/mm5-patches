#include <pmdsky.h>
#include <cot.h>
#include "extern.h"

//I'm pretty sure VAR_SIDE06_ROOM is not used elsewhere in this repository?
static void ChangePressBit(){
  int check = LoadScriptVariableValue(NULL, VAR_SIDE06_ROOM);
  if (check == 0){
  SaveScriptVariableValue(NULL, VAR_SIDE06_ROOM, 1);// 1= write, 0 = check
  }
  else if(check == 1){
    SaveScriptVariableValue(NULL, VAR_SIDE06_ROOM, 0);
  }
}

static int checkIfTsValuesAreInDefinedRange(short arg1){
  int TSX = TSXPosLive;
  int TSY = TSYPosLive;
  int TSP = TSPressed;
  TSX = TSX*256;
  TSY = TSY*256;//TS Values Set Up
  int TSBitcheck = LoadScriptVariableValue(NULL, VAR_SIDE06_ROOM);
  if (TSBitcheck == 0){
  if (TSP == 0){
    return 255;
  }
  //Check for Index 0
  int XInd = LoadScriptVariableValueAtIndex(NULL, VAR_POSITION_X, 0);
  int YInd = LoadScriptVariableValueAtIndex(NULL, VAR_POSITION_Y, 0);
  if (TSX < XInd+2560 && TSX > XInd-2560 && TSY > YInd-5120 && TSY+512  < YInd){
    ChangePressBit();
    return 0;
  }
  //Check for Index 1
  XInd = LoadScriptVariableValueAtIndex(NULL, VAR_POSITION_X, 1);
  YInd= LoadScriptVariableValueAtIndex(NULL, VAR_POSITION_Y, 1);
  if (TSX < XInd+2560 && TSX > XInd-2560 && TSY > YInd-5120 && TSY+512  < YInd){
    ChangePressBit();
    return 1;
  }
  //Check for Index 2
  XInd = LoadScriptVariableValueAtIndex(NULL, VAR_POSITION_X, 2);
  YInd= LoadScriptVariableValueAtIndex(NULL, VAR_POSITION_Y, 2);
  if (TSX < XInd+2560 && TSX > XInd-2560 && TSY > YInd-5120 && TSY+512 < YInd){
    ChangePressBit();
    return 2;
  }
  return 254;
  }
  else{
    if (TSP == 0){
      //Save last pos after drag
      int TSXLast = TSXPosLastMitDrag;
      int TSYLast = TSYPosLastMitDrag;
      TSXLast = TSXLast*256;
      TSYLast = TSYLast*256;
      switch (arg1){
        case 0:
        SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_X, 0, TSXLast);
        SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_Y, 0, TSYLast);
        ChangePressBit();
        return 2;
        case 1:
        SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_X, 1, TSXLast);
        SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_Y, 1, TSYLast);
        ChangePressBit();
        return 2;
        case 2:
        SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_X, 2, TSXLast);
        SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_Y, 2, TSYLast);
        ChangePressBit();
        return 2;
      }
      return 2;
    }

    switch (arg1){
      case 0:
      SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_X, 0, TSX);
      SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_Y, 0, TSY);
      return 1;
      case 1:
      SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_X, 1, TSX);
      SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_Y, 1, TSY);
      return 1;
      case 2:
      SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_X, 2, TSX);
      SaveScriptVariableValueAtIndex(NULL, VAR_POSITION_Y, 2, TSY);
      return 1;
    }
  return 128;
  }
}