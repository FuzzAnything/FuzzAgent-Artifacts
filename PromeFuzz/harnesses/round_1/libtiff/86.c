// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetMode at tif_open.c:843:5 in tiffio.h
// TIFFSetMode at tif_open.c:848:5 in tiffio.h
// TIFFGetMode at tif_open.c:843:5 in tiffio.h
// TIFFWriteCheck at tif_write.c:605:5 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFSetMode at tif_open.c:848:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFWriteCheck at tif_write.c:605:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
}

static TIFF* create_dummy_tiff(const uint8_t* data, size_t size) {
    // Create a dummy TIFF file with the provided data
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) return NULL;
    
    // Write minimal TIFF header (little-endian)
    uint8_t header[] = {
        0x49, 0x49,  // Little-endian
        0x2A, 0x00,  // TIFF magic number
        0x08, 0x00, 0x00, 0x00  // First IFD offset
    };
    fwrite(header, 1, sizeof(header), fp);
    
    // Write the fuzzer data
    if (size > 0) {
        fwrite(data, 1, size > 1024 ? 1024 : size, fp);
    }
    fclose(fp);
    
    // Open the TIFF file
    TIFFSetWarningHandler(dummy_handler);
    TIFFSetErrorHandler(dummy_handler);
    return TIFFOpen("./dummy_file", "r+");
}

int LLVMFuzzerTestOneInput_86(const uint8_t* Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create dummy TIFF file and open it
    TIFF* tif = create_dummy_tiff(Data, Size);
    if (!tif) return 0;
    
    // Extract some parameters from fuzzer input
    int mode_param = Data[0] & 0x0F;
    int write_check_param = (Data[0] >> 4) & 0x0F;
    const char* module_name = "fuzz";
    
    // Test TIFFGetMode and TIFFSetMode
    int original_mode = TIFFGetMode(tif);
    int previous_mode = TIFFSetMode(tif, mode_param);
    int current_mode = TIFFGetMode(tif);
    
    // Test TIFFWriteCheck with different parameters
    int write_check_result = TIFFWriteCheck(tif, write_check_param, module_name);
    
    // Test TIFFCreateDirectory
    int create_dir_result = TIFFCreateDirectory(tif);
    
    // Test TIFFWriteDirectory if directory was created
    if (create_dir_result) {
        TIFFWriteDirectory(tif);
    }
    
    // Test TIFFCheckpointDirectory
    TIFFCheckpointDirectory(tif);
    
    // Try to create and write multiple directories with different modes
    if (Size > 10) {
        // Use second byte to control additional operations
        uint8_t control = Data[1];
        
        if (control & 0x01) {
            // Create another directory
            TIFFCreateDirectory(tif);
            
            // Try to write it with different mode
            TIFFSetMode(tif, (control >> 1) & 0x07);
            TIFFWriteDirectory(tif);
            
            // Checkpoint again
            TIFFCheckpointDirectory(tif);
        }
        
        // Try TIFFWriteCheck with different module names
        if (control & 0x02) {
            static const char* modules[] = {"tiff", "strip", "tile", "compress"};
            int idx = (control >> 3) & 0x03;
            TIFFWriteCheck(tif, write_check_param, modules[idx]);
        }
    }
    
    // Clean up
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}