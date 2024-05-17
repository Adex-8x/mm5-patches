	.org 0x023A7080+0x30F70+0xD0
	.area 0x150
	HookSpeed:
		;mov r0,#0
		;ldr r1,=held
		;bl 0x020061EC
		ldr r0,[r4, #+0x64]
		ldr r1,=spd
		ldrh r1, [r1]
		mul r0,r1,r0
		;ldr r1,=held
		;ldrh r1, [r1]
		;tst r1,#0x200
		;tsteq r1,#0x100
		;mov r1,SpeedUpMultiplier
		;mul r0,r1,r0
		ldr r1,=count
		ldrh r1, [r1]
		add r0,r0,r1
		ldr r1,=frac
		ldrh r1, [r1]
		bl 0x0208FEA4
		ldr r2,=count
		str r1,[r2]
		b EndHookSpeed
	HookLoop:
		ldr r0,[r4, #+0x80]
		cmp r0,#0x0
		bne EndHookLoop
		; Here is the fun part
		mov r0,#0
		b 0x02021EC8
	HookVLetter:
		ldr r0,[r13, #+0x70]
		ldr r1,=char_VS
		bl 0x020208C8
		cmp r0,#0x0
		beq second_test
		ldr r0,[r13, #+0x74]
		bl 0x0202090C
		ldr r1,=spd
		strh r0, [r1]
		cmp r6,#2
		moveq r0,#1
		beq no_second_param
		ldr r0,[r13, #+0x78]
		bl 0x0202090C
	no_second_param:
		ldr r1,=frac
		strh r0, [r1]
		mov r0,#0
		ldr r1,=count
		str r0, [r1]
		str r0,[r4, #+0x80]
		b 0x02021AA0
	second_test:
		ldr r0,[r13, #+0x70]
		ldr r1,=char_VR
		bl 0x020208C8
		cmp r0,#0x0
		beq 0x02021A10
		mov r0,BaseMultiplier
		ldr r1,=spd
		strh r0, [r1]
		mov r0,BaseFrac
		ldr r1,=frac
		strh r0, [r1]
		mov r0,#0
		ldr r1,=count
		str r0, [r1]
		str r0,[r4, #+0x80]
		b 0x02021AA0
		.pool
	spd:
		.dcw BaseMultiplier
	frac:
		.dcw BaseFrac
	count:
		.word 0x0
	;held:
	;	.dcw 0x0
	char_VS:
		.ascii "VS",0
	char_VR:
		.ascii "VR",0
	.endarea