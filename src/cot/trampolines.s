.align 4
cotInternalTrampolineScriptSpecialProcessCall:
  // If the special process ID is >= 100, handle it as a custom special process
  // It feels like we're playing 4D chess...
  cmp r1, #100
  bge cotInternalDispatchScriptSpecialProcessCall
  // It doesn't hurt to add some insurance!
  // A few instructions make all the difference.
  cmp r1,#22
  blt whereOneGetsTheGall
  cmp r1,#27
  ble cotInternalDispatchScriptSpecialProcessCall
whereOneGetsTheGall:
  // Otherwise, restore the instruction we've replaced in the patch
  // and run the original function down the hatch!
  push	{r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
  b ScriptSpecialProcessCall+4
