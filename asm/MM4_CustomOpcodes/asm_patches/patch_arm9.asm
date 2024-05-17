.org BeforeDBoxCreation
.area 0x4
	mov r2,#1 ; bl DBoxCreationHook
.endarea

.org LoadTextboxFormat0
.area 0x4
	ldreq r0,[r15,#+0x68] ; bleq TextBoxFormat0Hook
.endarea

.org CallUpdateFrameStruct
.area 0x4
	bl UpdateFrameHook
.endarea

.org PrepareDBoxColor
.area 0x4
	bl DBoxColorHook
.endarea

.org PrepareDisplayDBoxBackground
.area 0x8
	push r4,r5,r14
	mov r4,r0 ; bl MagicNumberHook
.endarea

.org PrepareDisplayDBoxBackgroundEnd
.area 0x4
	pop r4,r5,r15
.endarea

;.org 0x02028A64
;.area 0x4
;	push r4-r10,r14 ; push r4-r11,r14
;.endarea

;.org 0x02028A6C
;.area 0x4
;	movs r4,r0
;.endarea

;.org BeforeCanvasLoop
;.area 0x4
;	mov r9,#0x4 ; bl BeforeCanvasLoopHook
;.endarea

;.org 0x02028DE4
;.area 0x4
;	pop r4-r10,r15 ; pop r4-r11,r15
;.endarea

.org 0x02028e40
.area 0x4
	push r3-r8,r14
.endarea

.org NextDBoxIteration
.area 0x4
	bl NextDBoxIterHook
.endarea

.org 0x02028e7c
.area 0x4
	pop r3-r8,r15
.endarea

;.org PrepareDBoxStartingYPos
;.area 0x4
;	bl LowerBoxTransitionHook
;.endarea

;.org PrepareDBoxEndingYPos
;.area 0x4
;	bl UpperBoxTransitionHook
;.endarea

;.org CalculateCanvasReturn
;.area 0x4
;	bl StoreCurrentDBoxIDHook
;.endarea