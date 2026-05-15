// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFSetWriteOffset at tif_write.c:961:6 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <unistd.h>
#include <fcntl.h>

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
}

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    // Suppress libtiff warnings/errors
    TIFFSetWarningHandler(dummy_handler);
    TIFFSetErrorHandler(dummy_handler);
    
    // Write fuzzer data to a dummy file
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;
    
    ssize_t written = write(fd, Data, Size);
    close(fd);
    
    if (written != (ssize_t)Size) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Try opening in different modes to explore different states
    const char* modes[] = {"r", "w", "a", "r+"};
    
    for (int mode_idx = 0; mode_idx < 4; mode_idx++) {
        TIFF* tif = TIFFOpen("./dummy_file", modes[mode_idx]);
        if (!tif) continue;
        
        // Extract some values from fuzzer data for parameters
        uint64_t param1 = 0, param2 = 0;
        if (Size >= sizeof(uint64_t)) {
            memcpy(&param1, Data, sizeof(uint64_t));
        }
        if (Size >= 2 * sizeof(uint64_t)) {
            memcpy(&param2, Data + sizeof(uint64_t), sizeof(uint64_t));
        }
        
        // Test TIFFReadDirectory - try to read directories
        int read_result = TIFFReadDirectory(tif);
        // We don't care about the result, just want to exercise the code
        
        // Test TIFFCurrentDirOffset
        uint64_t current_offset = TIFFCurrentDirOffset(tif);
        (void)current_offset; // Use result to avoid unused variable warning
        
        // Test TIFFSetSubDirectory
        int set_sub_result = TIFFSetSubDirectory(tif, param1);
        (void)set_sub_result;
        
        // Test TIFFCheckpointDirectory
        int checkpoint_result = TIFFCheckpointDirectory(tif);
        (void)checkpoint_result;
        
        // Test TIFFSetWriteOffset
        TIFFSetWriteOffset(tif, (toff_t)param2);
        
        // Test TIFFWriteCustomDirectory - needs custom directory setup
        // First try to setup a custom directory
        if (mode_idx == 1 || mode_idx == 2 || mode_idx == 3) { // Write modes
            // Try to create a custom directory with NULL field array
            // We'll just attempt to write whatever custom directory might exist
            uint64_t offset_out = 0;
            int write_custom_result = TIFFWriteCustomDirectory(tif, &offset_out);
            (void)write_custom_result;
            (void)offset_out;
        }
        
        // Try reading more directories if in read mode
        if (mode_idx == 0 || mode_idx == 3) {
            while (TIFFReadDirectory(tif) > 0) {
                // Keep reading until EOF or error
            }
        }
        
        TIFFClose(tif);
    }
    
    // Clean up
    unlink("./dummy_file");
    
    return 0;
}