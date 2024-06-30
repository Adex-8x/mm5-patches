#include "extern.h"
#include "libnds_videoGL.h"
#include "libnds_dma.h"

// camera default to 0, 0, 0 pointing toward positive depth
void initializeCustom3D() {
    // Use a custom projection matrix
    GEOM_COMM_MTX_MODE = 0;
    GEOM_COMM_MTX_PUSH = 0;
    GEOM_COMM_MTX_IDENTITY = 0;
    
    // Generated with compute_projection_matrix.py
    GEOM_COMM_MTX_LOAD_4x4 = -0x2000;
    GEOM_COMM_MTX_LOAD_4x4 = 0x0;
    GEOM_COMM_MTX_LOAD_4x4 = 0x0;
    GEOM_COMM_MTX_LOAD_4x4 = 0x0;
    GEOM_COMM_MTX_LOAD_4x4 = 0x0;
    GEOM_COMM_MTX_LOAD_4x4 = 0x2aab;
    GEOM_COMM_MTX_LOAD_4x4 = 0x0;
    GEOM_COMM_MTX_LOAD_4x4 = 0x0;
    GEOM_COMM_MTX_LOAD_4x4 = 0x0;
    GEOM_COMM_MTX_LOAD_4x4 = 0x0;
    GEOM_COMM_MTX_LOAD_4x4 = -0x1053;
    GEOM_COMM_MTX_LOAD_4x4 = -0x2053;
    GEOM_COMM_MTX_LOAD_4x4 = 0x0;
    GEOM_COMM_MTX_LOAD_4x4 = 0x0;
    GEOM_COMM_MTX_LOAD_4x4 = -0x1000;
    GEOM_COMM_MTX_LOAD_4x4 = 0x0;

    GEOM_COMM_MTX_SCALE = 0x1000;
    GEOM_COMM_MTX_SCALE = 0x1000;
    GEOM_COMM_MTX_SCALE = 0x1000;



    GEOM_COMM_MTX_MODE = 2;
    GEOM_COMM_MTX_PUSH = 0;
}

// restore values so the game can render it’s stuff normally
void finaliseCustom3d() {
    // restore values
    GEOM_COMM_MTX_MODE = 0;
    GEOM_COMM_MTX_POP = 1;
    GEOM_COMM_MTX_MODE = 2;
    GEOM_COMM_MTX_POP = 1;
}

// Send the mesh contained in a .fifo file (+begin and end vertices commands)
void sendMesh(struct iovec *command_buffer) {

    GEOM_COMM_BEGIN_VTXS = 0;

    // adapted from glCallList in libnds

    // I’ll just assume no other FIFO I/O is happening in the other 3 channel (unlike libnds)
    while((DMA_CR(0) & DMA_BUSY));
    
    DMA_SRC(0) = (uint32_t) command_buffer->iov_base;
    DMA_DEST(0) = 0x4000400;
    DMA_CR(0) = DMA_FIFO | (command_buffer->iov_len / 4);

    while(DMA_CR(0) & DMA_BUSY);

    GEOM_COMM_END_VTXS = 0;
}


struct iovec COMMAND_BUFFER;
bool IS_INITIALISED = false;

// take 0x400 bytes at 0x11000, palette 0x4
void testLoadData() {
    struct wte_handle wte_ref;
    LoadWteFromRom(&wte_ref, "/CUSTOM/VRAM/sphetex.wte", 0);
    ProcessWTEWrapper(&wte_ref, 0x11000, 0x4, 0);
    DoSomethingOn3dAllocAndClearInput(&wte_ref.content);

    LoadFileFromRom(&COMMAND_BUFFER, "CUSTOM/MODEL/sphere.fifo", 0);
    // OUI, je connais le C-A-C-H-E :) (bon, surtout libnds)
    DC_FlushRange(COMMAND_BUFFER.iov_base, COMMAND_BUFFER.iov_len);
}

int8_t test_trans = 0;

// (note: this is called before the standard 3d operation)
void TestDrawMesh() {
    if (IS_INITIALISED == false) {
        testLoadData();
        IS_INITIALISED = true;
    }


    test_trans += 1;
    
    initializeCustom3D();

    GEOM_COMM_MTX_IDENTITY = 0;

    GEOM_COMM_MTX_TRANS = 0;
    GEOM_COMM_MTX_TRANS = 0;
    GEOM_COMM_MTX_TRANS = -0x1000+test_trans*8;

    GEOM_COMM_MTX_SCALE = (0x1 << 12);
    GEOM_COMM_MTX_SCALE = (0x1 << 12);
    GEOM_COMM_MTX_SCALE = (0x1 << 12);


    GEOM_COMM_POLYGON_ATTR = POLY_ALPHA(31) | POLY_CULL_FRONT | POLY_MODULATION;

    GeomSetTexImageParam(2, 0, 3, 3, 0, 0, 0, 0x11000);
    GEOM_COMM_PLTT_BASE = 0x400 / 4;
    
    sendMesh(&COMMAND_BUFFER);

    finaliseCustom3d();
}