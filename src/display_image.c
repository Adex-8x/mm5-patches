// under the MIT license, by marius851000

#include "display_image.h"

// require the top screen to be configured to LDLC mode, then switched back to BG mode for engine 2
void displayImageOnConfiguredTopScreen(const char * filepath) {
    struct file_stream file;
    DataTransferInit();
    FileOpen(&file, filepath);
    FileRead(&file, ((void *) 0x6840000), 0x1FFFF);
    DataTransferStop();
}

// that’s it ? Too easy.