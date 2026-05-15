// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFLastDirectory at tif_dir.c:2229:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void dummy_error_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress error messages during fuzzing
}

static void dummy_warning_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress warning messages during fuzzing
}

int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    // Write data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Set error handlers to suppress output
    TIFFSetErrorHandler(dummy_error_handler);
    TIFFSetWarningHandler(dummy_warning_handler);
    
    // Open TIFF file
    TIFF *tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        // Try read-only mode if read/write fails
        tif = TIFFOpen("./dummy_file", "r");
    }
    
    if (tif) {
        // Get directory count if available
        tdir_t dir_count = 0;
        while (TIFFReadDirectory(tif)) {
            dir_count++;
        }
        
        // Reset to first directory
        if (dir_count > 0) {
            TIFFSetDirectory(tif, 0);
        }
        
        // Use input data to determine which functions to call
        // Use first few bytes as selector
        uint8_t selector = 0;
        if (Size > 0) {
            selector = Data[0];
        }
        
        // Call different functions based on selector
        switch (selector % 7) {
            case 0:
                // Try to rewrite current directory
                if (dir_count > 0) {
                    TIFFRewriteDirectory(tif);
                }
                break;
                
            case 1:
                // Try to set directory by index
                if (dir_count > 0) {
                    uint16_t dir_index = 0;
                    if (Size > 1) dir_index = Data[1] % (dir_count + 1);
                    TIFFSetDirectory(tif, dir_index);
                }
                break;
                
            case 2:
                // Create new directory
                TIFFCreateDirectory(tif);
                break;
                
            case 3:
                // Try to set subdirectory
                if (Size > 1) {
                    uint64_t offset = 0;
                    // Use up to 8 bytes for offset
                    size_t offset_bytes = Size > 9 ? 8 : Size - 1;
                    memcpy(&offset, Data + 1, offset_bytes);
                    TIFFSetSubDirectory(tif, offset);
                }
                break;
                
            case 4:
                // Read directory
                TIFFReadDirectory(tif);
                break;
                
            case 5:
                // Check if last directory
                TIFFLastDirectory(tif);
                break;
                
            case 6:
                // Combination: create, set, and rewrite
                if (dir_count == 0) {
                    TIFFCreateDirectory(tif);
                }
                if (dir_count > 0) {
                    uint16_t dir_index = 0;
                    if (Size > 1) dir_index = Data[1] % (dir_count + 1);
                    TIFFSetDirectory(tif, dir_index);
                    TIFFRewriteDirectory(tif);
                }
                break;
        }
        
        // Try to read through all directories again
        if (TIFFSetDirectory(tif, 0)) {
            while (TIFFReadDirectory(tif)) {
                // Just continue reading
            }
        }
        
        // Clean up
        TIFFClose(tif);
    }
    
    // Remove dummy file
    unlink("./dummy_file");
    
    return 0;
}