.org OpcodeCheck
.area 0x4
	b NewOpcodesHook
EndHook:
.endarea

.org GetParameterCount
.area 0x4
	bl NewOpcodeParameterHook
.endarea

.org 0x022E8E2C
.area 0x4
	bl TimeHollowHook
.endarea
