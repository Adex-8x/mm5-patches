#ifndef _MESH_RENDER_H
#define _MESH_RENDER_H

#include "extern.h"
#include "smalllib/libnds_videoGL.h"
#include "smalllib/libnds_dma.h"
#include "smalllib/fixed_point_math.h"

void sendMesh(struct iovec *command_buffer);
void doXRot(int angle);
void doYRot(int angle);
void doZRot(int angle);

#endif