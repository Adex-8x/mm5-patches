// under the MIT license, by marius851000
#include "drawing.h"
#include "smalllib/rng.h"

const int VRAM_BASE = 0x6840000;

struct DrawingState DRAWING_STATE;

void init_drawing(const char* filepath) {
    DRAWING_STATE.x = 256/2;
    DRAWING_STATE.y = 192/2;
    DRAWING_STATE.is_pen_down = 0;
    DRAWING_STATE.progress_per_frame = 5;
    DRAWING_STATE.buffer_pos = 0;
    DRAWING_STATE.file_closed = 0;
    DRAWING_STATE.color_base = 0x8000;
    DataTransferInit();
    FileInit(&DRAWING_STATE.file);
    FileOpen(&DRAWING_STATE.file, filepath);
    FileRead(&DRAWING_STATE.file, &DRAWING_STATE.command_buffer, BUFFER_SIZE * 4);
    DataTransferStop();
}

// trust the compiler to optimise it into a few shift and or
uint16_t merge_color_4(uint16_t source, uint16_t to_mix) {
    unsigned int r_s = (source >> 10) & 0x1F;
    unsigned int g_s = (source >> 5) & 0x1F;
    unsigned int b_s = source & 0x1F;
    unsigned int r_t = (to_mix >> 10) & 0x1F;
    unsigned int g_t = (to_mix >> 5) & 0x1F;
    unsigned int b_t = to_mix & 0x1F;

    unsigned int r_m = r_s - (r_s - r_t) / 4;
    unsigned int g_m = g_s - (g_s - g_t) / 4;
    unsigned int b_m = b_s - (b_s - b_t) / 4;

    uint16_t merged = 0x8000 | (r_m << 10) | (g_m << 5) | b_m;
    return merged;
}

// trust the compiler to optimise it into a few shift and or
uint16_t merge_color_8(uint16_t source, uint16_t to_mix) {
    unsigned int r_s = (source >> 10) & 0x1F;
    unsigned int g_s = (source >> 5) & 0x1F;
    unsigned int b_s = source & 0x1F;
    unsigned int r_t = (to_mix >> 10) & 0x1F;
    unsigned int g_t = (to_mix >> 5) & 0x1F;
    unsigned int b_t = to_mix & 0x1F;

    unsigned int r_m = r_s - (r_s - r_t) / 8;
    unsigned int g_m = g_s - (g_s - g_t) / 8;
    unsigned int b_m = b_s - (b_s - b_t) / 8;

    uint16_t merged = 0x8000 | (r_m << 10) | (g_m << 5) | b_m;
    return merged;
}

bool continue_drawing() {
    uint8_t progress_remaining = DRAWING_STATE.progress_per_frame;
    while (progress_remaining != 0) {
        progress_remaining -= 1;

        if (DRAWING_STATE.buffer_pos >= BUFFER_SIZE) {
            DataTransferInit();
            FileRead(&DRAWING_STATE.file, &DRAWING_STATE.command_buffer, BUFFER_SIZE * 4);
            DataTransferStop();
            DRAWING_STATE.buffer_pos = 0;
        }

        struct DrawingInfo* current_command = &DRAWING_STATE.command_buffer[DRAWING_STATE.buffer_pos];
        if (current_command->command == DRAWING_COMMAND_END) {
            if (DRAWING_STATE.file_closed == 0) {
                DataTransferInit();
                FileClose(&DRAWING_STATE.file);
                DataTransferStop();
                DRAWING_STATE.file_closed = 1;
            }
            return 1;
        }
        
        // Go toward that direction
        if (current_command->command == DRAWING_COMMAND_PEN_DOWN || current_command->command == DRAWING_COMMAND_PEN_UP) {
            if (DRAWING_STATE.x > current_command->x) {
                DRAWING_STATE.x -= 1;
            } else if (DRAWING_STATE.x < current_command->x) {
                DRAWING_STATE.x += 1;
            }

            if (DRAWING_STATE.y > current_command->y) {
                DRAWING_STATE.y -= 1;
            } else if (DRAWING_STATE.y < current_command->y) {
                DRAWING_STATE.y += 1;
            }
            
            if (DRAWING_STATE.is_pen_down != 0) {
                uint16_t coord = DRAWING_STATE.x + (DRAWING_STATE.y << 8);
                uint16_t* base_address = ((uint16_t *) VRAM_BASE) + coord;
                
                
                *base_address = DRAWING_STATE.color_base;
                // 0b1111_0011_1001_1100
                // 0xE318
                *(base_address + 1) = merge_color_4(*(base_address + 1), DRAWING_STATE.color_base);
                *(base_address - 1) = merge_color_4(*(base_address - 1), DRAWING_STATE.color_base);
                *(base_address + 256) = merge_color_4(*(base_address + 256), DRAWING_STATE.color_base);
                *(base_address - 256) = merge_color_4(*(base_address - 256), DRAWING_STATE.color_base);

                *(base_address + 255) = merge_color_8(*(base_address + 255), DRAWING_STATE.color_base);
                *(base_address + 257) = merge_color_8(*(base_address + 257), DRAWING_STATE.color_base);
                *(base_address - 255) = merge_color_8(*(base_address - 255), DRAWING_STATE.color_base);
                *(base_address - 257) = merge_color_8(*(base_address - 257), DRAWING_STATE.color_base);
            }

            if (DRAWING_STATE.x == current_command->x && DRAWING_STATE.y == current_command->y) {
                if (current_command->command == DRAWING_COMMAND_PEN_DOWN) {
                    DRAWING_STATE.is_pen_down = 1;
                } else {
                    DRAWING_STATE.is_pen_down = 0;
                }
                DRAWING_STATE.buffer_pos += 1;
                continue;
            }
        } else if (current_command->command == DRAWING_COMMAND_COLOR) {
            uint8_t b = current_command->x;
            uint8_t g = current_command->y;
            uint8_t r = current_command->param_3;

            // those 0xFF are leftover
            DRAWING_STATE.color_base = 0x8000 |
                (((r >> 3) & 0xFF) << 10)|
                (((g >> 3) & 0xFF) << 5)|
                ((b >> 3) & 0xFF);

            DRAWING_STATE.buffer_pos += 1;
        }

    }

    return 0;
}