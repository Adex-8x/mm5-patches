.nds
.include "symbols.asm"

.open "overlay11.bin", overlay11_start
  .org ScriptSpecialProcessCall
    b cotInternalTrampolineScriptSpecialProcessCall
.close 
