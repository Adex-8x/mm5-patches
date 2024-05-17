.org 0x023A7080+0x30F70+0xD0+0x154+0x48
.area 0x430


TimeHollowHook:
	push r4,r14
	bl 0x0204f9cc ; OGI
	mov r0,#0
	mov r1,#78
	mov r2,#63
	bl 0x0204B678
	cmp r0,#0
	popeq r4,r15
	mov r0,#0
	mov r1,#12
	mov r2,#1
	bl 0x0204B678
	cmp r0,#0
	popeq r4,r15
	ldr r0,=0x020B0A48
	ldr r0,[r0]
	sub r0,r0,#0x700
	sub r0,r0,#0x4c
	ldr r0,[r0]
	cmp r0,#0x8
	pople r4,r15
	; Time to free our special dbox!
	mov r0,#0
	mov r1,#12
	mov r2,#0
	bl 0x0204B678
	bl FreeInstantDBox
	mov r3,#0
		mov r2,#1
		mov r1,#12
		mov r0,#0
		bl 0x204B988
	pop r4,r15

NextDBoxIterHook:
	add r7,r5,r6, lsl #0x7 ; Original instruction
	mov r0,#0xA00
	mov r1,#0x60
	mla r0,r1,r6,r0
	add r8,r7,r0
	ldr r0,=CANVAS_STRUCTS
	sub r8,r8,r0
	lsr r8,r8,#0x7
	bx r14

IsCurrentDBoxSpecial:
	push r1-r2,r14
	ldr r2,=LOWER_BLACK_BOX_ID
	ldr r1,[r2]
	ldr r2,[r2,#+0x4]
	cmp r8,r1
	cmpne r8,r2
	movne r12,#0
	moveq r12,#1
	pop r1-r2,r15

UpdateFrameHook:
	push r14
	bl IsCurrentDBoxSpecial
	cmp r12,#1
	popeq r15
	bl UpdateFrameStruct
	pop r15

DBoxColorHook:
	push r14
	bl IsCurrentDBoxSpecial
	cmp r12,#1
	moveq r1,#0
	strh r1,[r0,#+0x1A] ; Original instruction
	; Janky workaround to the Y Offset 0x0 being buggy
store_starting_y_pos:
	ldr r3,=UPPER_BLACK_BOX_ID
	ldr r3,[r3]
	cmp r8,r3
	moveq r1,#0 ; This is the jankiest thing I've ever written but who cares
	streqh r1,[r0,#+0x6] ; Original instruction
	pop r15

; -------------------------------------------------
; OPCODES
; -------------------------------------------------

NewOpcodeParameterHook:
	ldr r7,=#403
	cmp r5,r7
	ldrge r0,=NEW_SCRIPT_OPCODES_PARAM_COUNT
	subge r1,r5,r7
	ldrsb r0,[r0,r1]
	bx r14

NewOpcodesHook:
	cmp r5,r7
	bge NewOpcodes
	cmp r5,r0
	b EndHook
NewOpcodes:
	sub r5,r5,r7
	cmp r5,#0x2
	addls r15,r15,r5,lsl #0x2
	; r7-r11 are free!
	b ReturnTwo
	b CreateLetterboxes
	b DeleteLetterboxes

CreateLetterboxes:
	ldr r7,=LOWER_BLACK_BOX_ID
	ldr r8,=UPPER_BLACK_BOX_ID
	ldr r9,[r7]
	ldr r10,[r8]

	cmp r9,#0xFE
	bne lower_box_already_made
	ldr r0,=LOWER_BLACK_BOX_FORMAT
	bl CreateDBox
	str r0,[r7]
	mov r9,r0
lower_box_already_made:
	mov r0,#0
	mov r1,#20 ; $SCENARIO_BALANCE_DEBUG
	bl GetGameVar
	mov r11,r0
	ldr r2,=#2585
	add r2,r2,r0
	mov r0,r9
	ldr r1,=0x403
	mov r3,#0x0
	bl ShowMessageInDBox

	cmp r10,#0xFE
	bne upper_box_already_made
	ldr r0,=UPPER_BLACK_BOX_FORMAT
	cmp r11,#5
	moveq r12,#4 ; For epilogue
	movne r12,#7
	strb r12,[r0,#+0x7]
	bl CreateDBox
	str r0,[r8]
	mov r10,r0
upper_box_already_made:
	mov r0,#0
	mov r1,#1
	bl GetGameVar
	add r2,r0,#0
	mov r0,r10
	ldr r1,=0x403
	mov r3,#0x0
	bl ShowMessageInDBox
	b ReturnTwo

DeleteLetterboxes:
	ldr r7,=LOWER_BLACK_BOX_ID
	ldr r0,[r7]
	bl FreeDBox
	ldr r0,[r7,#+0x4]
	bl FreeDBox
	mov r0,#0xFE
	str r0,[r7]
	str r0,[r7,#+0x4]
	b ReturnTwo

.pool
NEW_SCRIPT_OPCODES_PARAM_COUNT:
	.byte 0x0, 0x0
.align
LOWER_BLACK_BOX_FORMAT:
	; Function to update stuff
	.byte 0x10 ;0x88
	.byte 0xFF ;0xF4
	.byte 0x02
	.byte 0x02
	.byte 0x0 ; X offset
	.byte 0x12 ; Y offset
	.byte 0x20 ; Width
	.byte 0x7 ; Height
	.byte 0x0 ; Screen
	.byte 0xFC ; Frame Type (0xFA for blank)
	.fill 0x10, 0x0
.align
UPPER_BLACK_BOX_FORMAT:
	; Function to update stuff
	.byte 0x10 ;0x88
	.byte 0xFF ;0xF4
	.byte 0x02
	.byte 0x02
	.byte 0x0 ; X offset
	.byte 0x1 ; Y offset -- CAREFUL
	.byte 0x20 ; Width
	.byte 0x7 ; Height
	.byte 0x0 ; Screen
	.byte 0xFC ; Frame Type (0xFA for blank)
	.fill 0x10, 0x0
.align
LOWER_BLACK_BOX_ID:
	.word 0xFE
UPPER_BLACK_BOX_ID:
	.word 0xFE
.endarea
