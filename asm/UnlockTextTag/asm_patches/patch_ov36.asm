.org 0x023A7080+0x30F70+0xD0+0x154
.area 0x38

HookULetter:
	ldr r0,[r13, StackThing]
	add r1,=char_U
	bl TagCheck
	cmp r0,#0
	beq TagCodeError
	ldr r0,[r13, StackThing+4]
 	bl GetTagParameter
	bl UnlockScriptLock
	b AfterTagIsFound
.pool
	char_U:
		.asciiz "U"
.endarea