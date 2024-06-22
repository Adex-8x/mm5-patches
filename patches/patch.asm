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

    .org GreaterThanActorId0xABranch
    .area 0x4
        bge HandleSpecialActorIds
    .endarea

    .org PlayBgmCheck
    .area 0x4
        cmp r6,#0x2000
    .endarea

    .org PlayBgm2Check
    .area 0x4
        cmp r6,#0x2000
    .endarea

    .org SaveFileIdentifier ; Don't ask
    .area 0x10
        .asciiz "MYSTERYMAIL"
        .fill NoteSetHeaderMode-., 0x0
    .endarea

    .org CreatePortraitBoxCallsite
    .area 0x4
        bl CustomCreatePortraitBox
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

    .org 0x022e8e28
    .area 0x4
        bl CustomUpdateAnything
    .endarea

    .org LoadActorAnimDataStart
    .area 0x4
        tst r5,#0x80000000 ; Original instruction, had to revert
    .endarea

    .org LoadActorLayeringBitfeld
    .area 0x4
        bl ManipulateActorLayering
    .endarea

    .org JumpToTitleScreen
    .area 0x4
        bx r14
    .endarea

    .org ReturnToTitleScreen
    .area 0x4
        bx r14
    .endarea
.close
