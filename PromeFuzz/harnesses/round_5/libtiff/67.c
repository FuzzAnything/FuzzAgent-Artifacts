// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFWriteCheck at tif_write.c:605:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetMode at tif_open.c:848:5 in tiffio.h
// TIFFGetMode at tif_open.c:843:5 in tiffio.h
// TIFFFlush at tif_flush.c:30:5 in tiffio.h
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

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress error/warning messages during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    // Set error handlers to suppress output
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Write fuzzer input to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Try to open the file in different modes
    const char* modes[] = {"r", "w", "a", "r+", "w+", "a+"};
    int mode_count = sizeof(modes) / sizeof(modes[0]);
    
    for (int i = 0; i < mode_count; i++) {
        TIFF* tif = TIFFOpen("./dummy_file", modes[i]);
        if (!tif) {
            continue;
        }
        
        // Use fuzzer data to determine which operations to perform
        // Use first few bytes to decide operation sequence
        size_t op_index = 0;
        while (op_index < Size && op_index < 100) { // Limit operations
            uint8_t op = Data[op_index % Size];
            op_index++;
            
            switch (op % 6) {
                case 0:
                    TIFFCheckpointDirectory(tif);
                    break;
                case 1:
                    TIFFCreateDirectory(tif);
                    break;
                case 2:
                    // Use next byte as tiled parameter if available
                    int tiled = 0;
                    if (op_index < Size) {
                        tiled = Data[op_index % Size] & 1;
                        op_index++;
                    }
                    TIFFWriteCheck(tif, tiled, "fuzz_operation");
                    break;
                case 3:
                    TIFFWriteDirectory(tif);
                    break;
                case 4:
                    // Use next byte as mode if available
                    if (op_index < Size) {
                        int new_mode = Data[op_index % Size] % 3;
                        op_index++;
                        TIFFSetMode(tif, new_mode);
                    }
                    break;
                case 5:
                    TIFFGetMode(tif);
                    break;
            }
            
            // Occasionally flush changes
            if ((op % 8) == 0) {
                TIFFFlush(tif);
            }
        }
        
        // Clean up
        TIFFClose(tif);
    }
    
    // Remove dummy file
    remove("./dummy_file");
    
    return 0;
}