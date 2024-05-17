	.org 0x02020ED0
	.area 0x4
		beq HookSpeed
	EndHookSpeed:
	.endarea
	.org 0x02020EE0
	.area 0x4
		b HookLoop
	EndHookLoop:
	.endarea
	
	.org 0x02020FCC
	.area 0x4
		b HookVLetter
	.endarea