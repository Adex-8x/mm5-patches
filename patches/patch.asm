.nds
.include "symbols.asm"

.open "arm9.bin", arm9_start
    .org NoLowercaseJTagBranch
    .area 0x4
        b NoLowercaseJTagFound
    .endarea

    .org NoUppercaseNTagBranch
    .area 0x4
        b NoUppercaseNTagFound
    .endarea
.close

.open "overlay11.bin", overlay11_start
    .org 0x022e60f0 ; Menu Start
    .area 0x4
        b MenuStartHook
    .endarea

    .org 0x022e6120
    .area 0x4
        b MenuStartHook
    .endarea

    .org 0x022e68f8 ; Menu End
    .area 0x4
        b MenuEndHook
    .endarea

    .org 0x022e6928
    .area 0x4
        b MenuEndHook
    .endarea
    
    .org GetSceneNameCallsite
    .area 0x4
        bl CustomGetSceneName
    .endarea
.close
