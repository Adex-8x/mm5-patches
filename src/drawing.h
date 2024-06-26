// under the MIT license, by marius851000
#include <pmdsky.h>
#include <cot.h>
#include <functions/arm9.h>
#include <data/ram.h>

#define DRAWING_COMMAND_PEN_DOWN 0
#define DRAWING_COMMAND_PEN_UP 1
#define DRAWING_COMMAND_END 2
#define DRAWING_COMMAND_COLOR 3

struct DrawingInfo {
    uint8_t command;
    uint8_t x;
    uint8_t y;
    uint8_t param_3;
};

#define BUFFER_SIZE 10

struct DrawingState {
    bool is_pen_down;
    int8_t progress_per_frame;
    uint8_t x;
    uint8_t y;
    struct DrawingInfo command_buffer[BUFFER_SIZE];
    unsigned int buffer_pos;
    struct file_stream file;
    bool file_closed;
    uint16_t color_base;
};

void init_drawing(const char* filepath);
bool continue_drawing();