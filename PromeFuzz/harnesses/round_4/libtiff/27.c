// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetMode at tif_open.c:843:5 in tiffio.h
// TIFFSetMode at tif_open.c:848:5 in tiffio.h
// TIFFWriteCheck at tif_write.c:605:5 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFSetMode at tif_open.c:848:5 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "tiffio.h"

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    // Initialize error handlers to suppress output
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Create dummy file with fuzzer data
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    if (Size > 0) {
        fwrite(Data, 1, Size, fp);
    }
    fclose(fp);
    
    // Open TIFF file in write mode
    TIFF* tif = TIFFOpen("./dummy_file", "w");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Use fuzzer data to determine which operations to perform
    // We'll use the first few bytes as control flags
    size_t control_idx = 0;
    
    // Call TIFFGetMode to check current mode
    int current_mode = TIFFGetMode(tif);
    
    // Try to change mode using fuzzer data
    if (Size > control_idx) {
        int new_mode = Data[control_idx] % 3; // 0, 1, 2
        int prev_mode = TIFFSetMode(tif, new_mode);
        control_idx++;
    }
    
    // Try TIFFWriteCheck with different modes
    if (Size > control_idx) {
        int tiled = Data[control_idx] & 1;
        const char* module = "fuzz";
        TIFFWriteCheck(tif, tiled, module);
        control_idx++;
    }
    
    // Create a directory
    TIFFCreateDirectory(tif);
    
    // Write directory if we have enough data
    if (Size > control_idx) {
        TIFFWriteDirectory(tif);
        control_idx++;
    }
    
    // Checkpoint directory
    if (Size > control_idx) {
        TIFFCheckpointDirectory(tif);
        control_idx++;
    }
    
    // Create another directory with different mode
    if (Size > control_idx) {
        int another_mode = Data[control_idx] % 3;
        TIFFSetMode(tif, another_mode);
        TIFFCreateDirectory(tif);
        control_idx++;
    }
    
    // Try to write and checkpoint again
    if (Size > control_idx) {
        TIFFWriteDirectory(tif);
        TIFFCheckpointDirectory(tif);
    }
    
    // Clean up
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}