/* !file arm9 */
ChangeGlobalBorderColor = 0x02027A80;
TextboxSolid = 0x0202715C;
TextboxTransparent = 0x02027148;
ResumeAdvancedMenu = 0x0202BBF0;
MyGetPressedButtons = 0x0200625C;
MyGetHeldButtons = 0x020061EC;
CreateImportantWarning = 0x0205d228;
se_Play = 0x0201A4FC;
GetDsFirmwareInfo = 0x02004F74;
LoadBgpBackground = 0x02052060;
LowercaseTagCodeError = 0x020232c8;
AfterLowercaseTagIsFound = 0x020232F0;
GetCurrentClockInfo = 0x02006f68;
SprintfClockInfo = 0x02006fb8;
NoLowercaseJTagBranch = 0x020225f0;
NoUppercaseNTagBranch = 0x02020fac;
UppercaseTagCodeError = 0x02021a10;
AfterUppercaseTagIsFound = 0x02021AA0;
LoadStaffont = 0x02025B90;
UpdateScreenFadeWrapper = 0x0200bb74;
SomeFadeInFunctionIdk = 0x0200bb60;
ChangeVolumeBgm = 0x2017f64;
GreaterThanActorId0xABranch = 0x02023f6c;
DefaultActorNameColor = 0x02023f84;
UseYellowName = 0x02023e00;
SaveFileIdentifier = 0x0209cd3c;
NoteSetHeaderMode = 0x0209cd4c;
CreatePortraitBoxCallsite = 0x02035118;
PlayBgmCheck = 0x02019948;
PlayBgm2Check = 0x02019E14;
LoadFrame = 0x02027180;

FRAMES_COLOR = 0x020afd98;

# Some video-related io-register
DISPCNT_A = 0x04000000;
DISPCNT_B = 0x04001000;

BG3CNT_B = 0x0400100E;
BG3CNT_A = 0x0400000E;
BG1CNT_A = 0x0400000A;

VRAMCNT_A = 0x04000240;
VRAMCNT_C = 0x04000242;
VRAMCNT_D = 0x04000243;
DISPCAPCNT = 0x4000064;

ProcessWTEWrapper = 0x0201e12c; #EU: 0x0201e1c8;
DoSomethingOn3dAllocAndClearInput = 0x0201e13c; #EU: 0x0201e1d8;

/* !file overlay11 */
MenuStartFinish = 0x022e68ac;
MenuEndFinish = 0x022e6e58;
MessageMenuKeyboardEnd = 0x022e6a2c;
StationLoadHangerSSA = 0x022e4c48;
StationLoadHangerSSB = 0x022e57fc;
GetSceneNameCallsite = 0x022debf4;
GetSceneName = 0x022e4600;
BranchProcessSpecialOpcode = 0x022de110;
ParseProcessSpecialOpcode = 0x022de448;
ProcessMessageMenuParam1 = 0x022de3f4;
SomeGroundModeLoopUpdateFunctionIdk = 0x022dc808;
LoadActorAnimDataStart = 0x022f5bcc;
LoadActorLayeringBitfeld = 0x022f816c;
ShouldReloadCachedScene = 0x022e573c;

ENGINE_DISPLAY_INFO = 0x022A37AC; # EU: 0x020091c8

# Hook at the start of the frame, when it’s safe to write to the VRAM
FrameHookStart = 0x022e930c; # EU: 0x022e9c4c
FrameHookCallAtEnd = 0x02008f88; # EU: 0x02008f88 (the same)

/* !file ram */
CURRENT_FRAME_STYLE = 0x022a7a6e;

TSXPosLive = 0x22A35DC;
TSYPosLive = 0x22A35E0;
TSPressed = 0x22A35E4;
TSXPosLastMitDrag = 0x22A35FC;
TSYPosLastMitDrag = 0x22A3600;

CAMERA_TOP_LEFT_COORD = 0x02389240;

# 3D Engine stuff (same EU/US/JP)
GEOM_COMM_BEGIN_VTXS = 0x04000500;
GEOM_COMM_END_VTXS = 0x04000504;
GEOM_COMM_VTX_16 = 0x0400048c;
GEOM_COMM_MTX_PUSH = 0x04000444;
GEOM_COMM_MTX_POP = 0x04000448;
GEOM_COMM_MTX_IDENTITY = 0x4000454;
GEOM_COMM_MTX_SCALE = 0x400046C;
GEOM_COMM_MTX_TRANS = 0x4000470;
GEOM_COMM_MTX_MODE = 0x4000440;
GEOM_COMM_POLYGON_ATTR = 0x040004a4;
GEOM_COMM_COLOR = 0x04000480;
GEOM_COMM_MTX_LOAD_4x4 = 0x4000458;
GEOM_COMM_PLTT_BASE = 0x40004AC;
GEOM_COMM_MTX_MULT_3x3 = 0x4000468;

GeomSetTexImageParam = 0x0201e494; # EU: 0x0201e530