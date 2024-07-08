#include "projectile_3d.h"
#include "smalllib/performer.h"
#include "smalllib/fpsqrt.h"

struct iovec COMMAND_BUFFER_1;
struct iovec COMMAND_BUFFER_2;

struct iovec BOOMIN;
struct iovec BOOMOUT;

bool SHOULD_RENDER_PROJECTILE = false;

// projtex1 take 0x200 bytes at 0x11000, palette high 0x4, low 0x0
// projtex2 take 0x200 bytes at 0x11200, palette high 0x4, low 0x20
// boom take 0x100 bytes at 0x11400, palette high 0x4, low 0x40
void loadProjectileData() {
    struct wte_handle wte_ref;
    LoadWteFromRom(&wte_ref, "/CUSTOM/VRAM/projtex1.wte", 0);
    ProcessWTEWrapper(&wte_ref, 0x11000, 0x4, 0);
    DoSomethingOn3dAllocAndClearInput(&wte_ref.content);


    LoadWteFromRom(&wte_ref, "/CUSTOM/VRAM/projtex2.wte", 0);
    ProcessWTEWrapper(&wte_ref, 0x11200, 0x4, 0x20);
    DoSomethingOn3dAllocAndClearInput(&wte_ref.content);

    LoadWteFromRom(&wte_ref, "/CUSTOM/VRAM/boom.wte", 0);
    ProcessWTEWrapper(&wte_ref, 0x11400, 0x4, 0x40);
    DoSomethingOn3dAllocAndClearInput(&wte_ref.content);

    LoadFileFromRom(&COMMAND_BUFFER_1, "CUSTOM/MODEL/proj1.fifo", 0);
    LoadFileFromRom(&COMMAND_BUFFER_2, "CUSTOM/MODEL/proj2.fifo", 0);

    LoadFileFromRom(&BOOMIN, "CUSTOM/MODEL/boomin.fifo", 0);
    LoadFileFromRom(&BOOMOUT, "CUSTOM/MODEL/boomout.fifo", 0);


    DC_FlushRange(COMMAND_BUFFER_1.iov_base, COMMAND_BUFFER_1.iov_len);
    DC_FlushRange(COMMAND_BUFFER_2.iov_base, COMMAND_BUFFER_2.iov_len);

    DC_FlushRange(BOOMIN.iov_base, BOOMIN.iov_len);
    DC_FlushRange(BOOMOUT.iov_base, BOOMOUT.iov_len);
}

bool IS_EXPLODING = false;

void setShouldRenderProjectile(bool value) {
    SHOULD_RENDER_PROJECTILE = value;
    if (value == true) {
        IS_EXPLODING = false;
    }
}

int PROGRESS_COUNTER = 0;
int PROGRESS_TARGET = 300;
const int SPEED_INNER_TARGET = -150;
const int SPEED_OUTER_TARGET = 250;
bool SPEEDING_UP = false;
int CURRENT_ROT_INNER = 0;
int CURRENT_ROT_OUTER = 0;

int FADE_COUNTER = 0;
const int FADE_COUNTER_TARGET = 60;

void projectileStartSpeedUp(int duration) {
    PROGRESS_COUNTER = 0;
    PROGRESS_TARGET = duration;
    SPEEDING_UP = true;
}

void projectileExplode(int duration) {
    IS_EXPLODING = true;
    PROGRESS_COUNTER = 0;
    PROGRESS_TARGET = duration;
    SPEEDING_UP = true;

}

void projectileFree() {
    setShouldRenderProjectile(false);

    MemFree(COMMAND_BUFFER_1.iov_base);
    MemFree(COMMAND_BUFFER_2.iov_base);
    
    MemFree(BOOMIN.iov_base);
    MemFree(BOOMOUT.iov_base);

    //TODO: how about textures?
    
}

void drawProjectileIfNeeded() {
    if (SHOULD_RENDER_PROJECTILE == false) {
        return;
    }

    if (SPEEDING_UP) {
        PROGRESS_COUNTER += 1;
        if (PROGRESS_COUNTER > PROGRESS_TARGET) {
            PROGRESS_COUNTER = PROGRESS_TARGET;
        }
    }

    int progress = __divsi3((PROGRESS_COUNTER << 12), PROGRESS_TARGET) & 0xFFFFFFFF;
    
    

    struct Performer* performer = find_performer(4);
    if (performer == 0) {
        return;
    }

    GEOM_COMM_MTX_IDENTITY = 0;

    GEOM_COMM_MTX_TRANS = (CAMERA_TOP_LEFT_COORD.x << 4) - (performer->hitbox_1.x >> 4) + (128 << 4);
    GEOM_COMM_MTX_TRANS = (CAMERA_TOP_LEFT_COORD.y << 4) - (performer->hitbox_1.y >> 4) + (96 << 4);
    GEOM_COMM_MTX_TRANS = -0x1000;

    GEOM_COMM_MTX_PUSH = 0;

    if (IS_EXPLODING == false) {

        FADE_COUNTER += 1;
        if (FADE_COUNTER > FADE_COUNTER_TARGET) {
            FADE_COUNTER = FADE_COUNTER_TARGET;
        }


        GEOM_COMM_MTX_TRANS = - (40 << 4);
        GEOM_COMM_MTX_TRANS = 0;
        GEOM_COMM_MTX_TRANS = 0;

        GEOM_COMM_MTX_SCALE = (0x3 << 8);
        GEOM_COMM_MTX_SCALE = (0x3 << 8);
        GEOM_COMM_MTX_SCALE = (0x3 << 8);

        int trans_level = (30 * ((FADE_COUNTER << 12) / FADE_COUNTER_TARGET)) >> 12;


        doZRot(1024);

        GEOM_COMM_MTX_PUSH = 0;
        
        CURRENT_ROT_OUTER += (SPEED_OUTER_TARGET * progress) >> 12;
        doYRot(CURRENT_ROT_OUTER);
        GEOM_COMM_POLYGON_ATTR = POLY_ALPHA(1 + trans_level >> 1) | POLY_CULL_NONE | POLY_MODULATION;
        GeomSetTexImageParam(2, 0, 3, 2, 0, 0, 1, 0x11000);
        GEOM_COMM_PLTT_BASE = 0x400 / 4;
        sendMesh(&COMMAND_BUFFER_1);


        GEOM_COMM_MTX_POP = 1;
        CURRENT_ROT_INNER += (SPEED_INNER_TARGET * progress) >> 12;
        doYRot(CURRENT_ROT_INNER);
        
        GEOM_COMM_POLYGON_ATTR = POLY_ALPHA(1 + trans_level) | POLY_CULL_NONE | POLY_MODULATION;
        GeomSetTexImageParam(2, 0, 3, 2, 0, 0, 0, 0x11200);
        GEOM_COMM_PLTT_BASE = 0x420 / 4;
        sendMesh(&COMMAND_BUFFER_2);
    }

    if (IS_EXPLODING && progress != (1 << 12)) {
        GEOM_COMM_MTX_POP = 1;


        //int progress_inv = (1 << 12) - progress;
        int progress_quint = ((((((((progress * progress) >> 12) * progress) >> 12) * progress) >> 12) * progress) >> 12);
        int progress_quint_inv = (1 << 12) - progress_quint;

        int test_progress = sqrt_fx16_16_to_fx16_16(progress << 3) >> 3;

        GEOM_COMM_MTX_SCALE = test_progress;
        GEOM_COMM_MTX_SCALE = test_progress;
        GEOM_COMM_MTX_SCALE = test_progress;



        doYRot(-1024);
        GEOM_COMM_POLYGON_ATTR = POLY_ALPHA(1 + ((29 * progress_quint_inv) >> 12)) | POLY_CULL_FRONT | POLY_MODULATION;
        GeomSetTexImageParam(2, 0, 2, 2, 0, 0, 0, 0x11400);
        GEOM_COMM_PLTT_BASE = 0x440 / 4;
        sendMesh(&BOOMOUT);


        GEOM_COMM_POLYGON_ATTR = POLY_ALPHA(1 + ((20 * progress_quint_inv) >> 12)) | POLY_CULL_FRONT | POLY_MODULATION;
        GeomSetTexImageParam(2, 0, 2, 2, 0, 0, 0, 0x11400);
        GEOM_COMM_PLTT_BASE = 0x440 / 4;
        sendMesh(&BOOMIN);
    } else {
        GEOM_COMM_MTX_POP = 1;
    }
}