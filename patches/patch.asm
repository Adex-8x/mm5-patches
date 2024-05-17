.nds
.include "symbols.asm"

.open "overlay11.bin", overlay11_start
    .org 0x022e60f0 ; Menu Start
    .area 0x4
        b MenuStartHook
    .endarea

    .org 0x022e68f8 ; Menu End
    .area 0x4
        b MenuEndHook
    .endarea

    .org GetSceneNameCallsite
    .area 0x4
        bl CustomGetSceneName
    .endarea
.close
