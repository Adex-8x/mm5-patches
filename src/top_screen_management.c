// Made by marius851000, available under the MIT license

#include <pmdsky.h>
#include <cot.h>
#include "drawing.h"
#include "smalllib/rng.h"
#include "display_image.h"
#include "extern.h"


bool CUSTOM_VRAM_TOP_MODE_ENABLED = 0;
bool CUSTOM_VRAM_IS_DRAWING_MODE = 0;

uint8_t VRAMCNT_C_BAK;
uint32_t DISPCNT_B_RELEVANT_BITS_BAK;
uint16_t BG3CNT_B_BAK;
struct EngineDisplayInfo ENGINE_DISPLAY_INFO_BAK_B;

void configureTopScreenDisplayToBitmap() {
    if (CUSTOM_VRAM_TOP_MODE_ENABLED != 0) {
        return;
    }

    VRAMCNT_C_BAK = VRAMCNT_C;


    int32_t dispcnt_B_temp = DISPCNT_B;
    DISPCNT_B_RELEVANT_BITS_BAK = dispcnt_B_temp & 0x00001F03;
    dispcnt_B_temp = (dispcnt_B_temp & 0xFFFFFFF8) | 3; // BG mode (Text/Text/Extended)
    dispcnt_B_temp = (dispcnt_B_temp & 0xFFFFE0FF); // keep only bg3 enabled
    DISPCNT_B = dispcnt_B_temp;

    BG3CNT_B_BAK = BG3CNT_B;
    BG3CNT_B = 0x4084;

    VRAMCNT_C = 0x84;

    memcpy(&ENGINE_DISPLAY_INFO_BAK_B, &ENGINE_DISPLAY_INFO[1], sizeof(struct EngineDisplayInfo));
    ENGINE_DISPLAY_INFO[1].disable_bg0 = 1;
    ENGINE_DISPLAY_INFO[1].disable_bg1 = 1;
    ENGINE_DISPLAY_INFO[1].disable_bg2 = 1;
    ENGINE_DISPLAY_INFO[1].disable_bg3 = 0;
    ENGINE_DISPLAY_INFO[1].disable_obj = 1;

    CUSTOM_VRAM_TOP_MODE_ENABLED = 1;
    CUSTOM_VRAM_IS_DRAWING_MODE = 0;

    COT_LOG(COT_LOG_CAT_SPECIAL_PROCESS, "VRAM configured for top screen bitmap display");
}

void displayImageOnTopScreen(const char * filepath) {
    configureTopScreenDisplayToBitmap();
    CUSTOM_VRAM_IS_DRAWING_MODE = 0;

    VRAMCNT_C = 0x80;
    displayImageOnConfiguredTopScreen(filepath);
    VRAMCNT_C = 0x84;
}

// Need VRAM C to be in LDLC mode
void writePseudoPaperOnConfiguredTopScreen() {

    for (int addr = 0x6840000; addr < 0x685FFFF; addr+=4) {

        int16_t graylevel = ((wyhash16() >> 8) % 2) + 6;
        int32_t graylevel_add = (graylevel << 10) | (graylevel << 5) | graylevel;

        int16_t graylevel_bis = ((wyhash16() >> 8) % 2) + 6;
        int32_t graylevel_add_bis = (graylevel_bis << 10) | (graylevel_bis << 5) | graylevel_bis;

        // 0b1_11000_11000_11000
        // 0b1110_0011_0001_1000
        // 0xE318
        *((int32_t *) addr) = 0xE318E318 | graylevel_add | (graylevel_add_bis << 16);
    }
    
}


void initDrawingOnTopScreen(const char * filepath) {
    configureTopScreenDisplayToBitmap();

    VRAMCNT_C = 0x80;
    writePseudoPaperOnConfiguredTopScreen();
    VRAMCNT_C = 0x84;

    CUSTOM_VRAM_IS_DRAWING_MODE = 1;
    init_drawing(filepath);
}

void topScreenReturnToNormal() {
    if (CUSTOM_VRAM_TOP_MODE_ENABLED == 0) {
        return;
    }

    VRAMCNT_C = 0x80;
    for (int addr = 0x6840000; addr < 0x685FFFF; addr+=4) {
        *((int32_t *) addr) = 0x00000000;
    }

    VRAMCNT_C = VRAMCNT_C_BAK;
    DISPCNT_B = (DISPCNT_B & 0xFFFFE0F8) | DISPCNT_B_RELEVANT_BITS_BAK;
    BG3CNT_B = BG3CNT_B_BAK;
    memcpy(&ENGINE_DISPLAY_INFO[1], &ENGINE_DISPLAY_INFO_BAK_B, sizeof(struct EngineDisplayInfo));

    CUSTOM_VRAM_TOP_MODE_ENABLED = 0;
    CUSTOM_VRAM_IS_DRAWING_MODE = 0;

    COT_LOG(COT_LOG_CAT_SPECIAL_PROCESS, "VRAM returned to normal");
}

void CustomTopScreenOnEachFrame() {
    if (CUSTOM_VRAM_TOP_MODE_ENABLED) {
        if (CUSTOM_VRAM_IS_DRAWING_MODE) {
            VRAMCNT_C = 0x80;

            continue_drawing();

            VRAMCNT_C = 0x84;
        }
    }
}