.relativeinclude on
.nds
.arm

.definelabel OpcodeCheck, 0x022dddd4
.definelabel SetGameVar, 0x0204B820
.definelabel CheckPressInput, 0x0200625C
.definelabel CheckHeldInput, 0x020061EC
.definelabel ReturnTwo, 0x022e246c ; 0x022e2474
.definelabel BeforeDBoxCreation, 0x0202F0FC
.definelabel TextboxColors, 0x02027800
.definelabel GetParameterCount, 0x022DDD8C
.definelabel UNK_GFX_FUNC_0, 0x0201E730 ; may not use
.definelabel UNK_GFX_FUNC_1, 0x0201F2A0 ; may not use
.definelabel CreateCanvas, 0x02027648
    ; 0x0: Update function
    ; 0x4: X offset
    ; 0x5: Y offset
    ; 0x6: Width
    ; 0x7: Height
    ; 0x8: Screen
    ; 0x9: Something to do with the frame???
        ; Set as 0xFD by default, changing it to 0xFC changes
        ; it to the frame beforehand (and so on...)
        ; TODO: Figure out how the game picks its frame from 0x9
        ; And make message_Talk2 able to be closed
    ; 0xA: Pointer to something?
    ; Most of these are zeroes but idk
.definelabel CreateDBox, 0x0202F0B0
.definelabel ShowDBox, 0x0202F3A4
.definelabel FreeDBox, 0x0202F148
.definelabel ShowMessageInDBox, 0x0202F1B4
.definelabel LoadTextboxFormat0, 0x0202F0D0
.definelabel TEXTBOX_FORMAT_0, 0x0209AF2C
.definelabel SomethingIDK, 0x020AFC70
; This'll be very interesting...
.definelabel UNK_FUNC_CALL, 0x02028E78
.definelabel UNK_GFX_FUNC_2, 0x0202A388
.definelabel PrepareDisplayDBoxBackground, 0x0201F05C
.definelabel MYSTERY_STRUCT, 0x022A7EDC
.definelabel MemAlloc, 0x02001170
.definelabel MemFree, 0x02001188
.definelabel Memcpy, 0x0208729C
.definelabel CreateInstantDBox, 0x0202FE2C ; r0 = layout_ptr, r1 = 0?, r2 = 0?, r3 = text_string_id
; Interestingly, all that happens in the heap-allocated space to get the string to display is do "strh r6,[r12,#0xa4]", in which r6 holds the original value of r3.
.definelabel FreeInstantDBox, 0x0202FED4
.definelabel NextDBoxIteration, 0x02028E54
.definelabel CallUpdateFrameStruct, 0x02028E64
.definelabel UpdateFrameStruct, 0x0201F2A0
.definelabel PrepareDisplayDBoxBackgroundEnd, 0x0201F104
.definelabel PrepareDBoxColor, 0x0201F0C4
.definelabel InitializingSomeStruct, 0x02028B94
.definelabel CalculateCanvasReturn, 0x0202788C
.definelabel CANVAS_STRUCTS, 0x022A88DC
.definelabel BeforeCanvasLoop, 0x02028B7C
.definelabel PrepareDBoxStartingYPos, 0x0201F078
.definelabel PrepareDBoxEndingYPos, 0x0201F088
.definelabel GetGameVar, 0x0204B4EC
.definelabel WhatTheFuck, 0x023A7080+0x33F70+0x80+0x204+0x40+0x200+0x70+0x3F0
