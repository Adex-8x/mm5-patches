// Under MIT by marius851000

#include "trail_bottom_3d.h"
#include <pmdsky.h>
#include "extern.h"

// take 0x80 bytes at 0x1F800, palette 0x10
void Load3dTrailTextures() {
    struct wte_handle wte_ref;
    LoadWteFromRom(&wte_ref, "/CUSTOM/VRAM/trail.wte", 0);
    ProcessWTEWrapper(&wte_ref, 0x1F800, 0x10, 0);
    DoSomethingOn3dAllocAndClearInput(&wte_ref.content);
}

uint16_t last_pos_x = 20;
uint16_t last_pos_y = 20;

#define STEP_COUNT 15

// x_start is 0xFF to skip this step
struct step {
    uint8_t x_start;
    uint8_t y_start;
    uint16_t rotation;
};

uint8_t STEP_CURRENT_POS = 0;
uint8_t STEP_LEN = 0;
struct step STEPS[STEP_COUNT];

uint8_t LAST_X = 0;
uint8_t LAST_Y = 0;

uint8_t REMOVAL_FRAME_COUNT = 0;
#define REMOVE_EVERY_X 3

bool TRAIL_ENABLED = false;
bool JUST_PRESSED = true;

void setTrailEnabled(bool isEnabled) {
    TRAIL_ENABLED = isEnabled;
}

void ProcessBottomTrail3d() {
    if (TRAIL_ENABLED && TSPressed != 0) {
        uint8_t target_pos_x = TSXPosLive;
        uint8_t target_pos_y = TSYPosLive;

        if (JUST_PRESSED) {
            LAST_X = target_pos_x;
            LAST_Y = target_pos_y;
            JUST_PRESSED = false;
        }

        while (true) {
            int delta_x = (int) target_pos_x - (int) LAST_X;
            int delta_y = (int) target_pos_y - (int) LAST_Y;

            int delta_list[2] = { delta_x, delta_y };
            float distance_f = EuclideanNorm(&delta_list);
            int distance = _ffix(distance_f);

            int rot = 0;
            if (distance >= 8) {

                if (STEP_LEN < STEP_COUNT) {
                    STEP_LEN += 1;
                }

                STEP_CURRENT_POS += 1;
                if (STEP_CURRENT_POS >= STEP_COUNT) {
                    STEP_CURRENT_POS = 0;
                }

                STEPS[STEP_CURRENT_POS].x_start = LAST_X;
                STEPS[STEP_CURRENT_POS].y_start = LAST_Y;
                STEPS[STEP_CURRENT_POS].rotation = -FX_Atan2Idx(delta_x, delta_y) + 0x4000;

                LAST_X += _ffix(_fmul(_fdiv(_fflt(delta_x), distance_f), 8));
                LAST_Y += _ffix(_fmul(_fdiv(_fflt(delta_y), distance_f), 8));
            } else {
                break;
            }
        }
    } else {
        JUST_PRESSED = true;
    }

    REMOVAL_FRAME_COUNT += 1;
    if (REMOVAL_FRAME_COUNT >= REMOVE_EVERY_X + (STEP_COUNT - STEP_LEN) / 5 && STEP_LEN > 0) {
        REMOVAL_FRAME_COUNT = 0;
        STEP_LEN -= 1;
    }

    int element_place = STEP_CURRENT_POS;

    for (int i = 0; i < STEP_LEN; i+=1) {
        int alpha = 0x1F;
        if (i == STEP_LEN - 2) {
            alpha = 0x12;
        }
        if (i == STEP_LEN - 1) {
            alpha = 0x08;
        }
        if (STEPS[element_place].x_start != 0xFF) {
            struct render_3d_texture* test_command = NewRender3dTexture();
        
            test_command->texture_vram_offset = 0x1F800;
            test_command->palette_base_addr = 0x1000*2;
            test_command->texture_size.x = 8;
            test_command->texture_size.y = 2;
            test_command->scale.x = 0x1000;
            test_command->scale.y = 0x1000;
            
            test_command->color.r = 0x1F;
            test_command->color.g = 0x1F;
            test_command->color.b = 0x1F;
            test_command->alpha = alpha;

            test_command->texture_params.texture_s_size = 0;
            test_command->texture_params.texture_t_size = 0;
            test_command->texture_params.texture_format = 2;

            test_command->translation.x = STEPS[element_place].x_start;
            test_command->translation.y = STEPS[element_place].y_start;
            test_command->rotation = STEPS[element_place].rotation;
        }


        if (element_place == 0) {
            element_place = STEP_COUNT;
        }
        element_place -= 1;
    }
}