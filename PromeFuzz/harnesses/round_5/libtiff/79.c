// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
// TIFFCreateCustomDirectory at tif_dir.c:1704:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFReadCustomDirectory at tif_dirread.c:5278:5 in tiffio.h
// TIFFAccessTagMethods at tif_extension.c:58:17 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Silent error handler to avoid output during fuzzing
}

static int write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return 0;
    size_t written = fwrite(Data, 1, Size, f);
    fclose(f);
    return written == Size;
}

int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    // Set silent error handlers
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Write input data to dummy file
    if (!write_dummy_file(Data, Size)) {
        return 0;
    }
    
    // Open TIFF file
    TIFF* tif = TIFFOpen("./dummy_file", Size > 0 ? "r" : "w");
    if (!tif) {
        // Try opening in write mode if read failed
        tif = TIFFOpen("./dummy_file", "w");
        if (!tif) {
            remove("./dummy_file");
            return 0;
        }
    }
    
    // Use first byte to decide which functions to call
    uint8_t selector = Size > 0 ? Data[0] : 0;
    
    // Try different API functions based on input
    switch (selector % 6) {
        case 0:
            TIFFCreateDirectory(tif);
            break;
            
        case 1: {
            uint64_t offset = 0;
            if (Size > 8) {
                memcpy(&offset, Data + 1, 8);
            }
            TIFFWriteCustomDirectory(tif, &offset);
            break;
        }
            
        case 2:
            // TIFFCreateCustomDirectory requires valid TIFFFieldArray
            // We'll pass NULL which should be handled by the library
            TIFFCreateCustomDirectory(tif, NULL);
            break;
            
        case 3:
            TIFFFreeDirectory(tif);
            break;
            
        case 4: {
            uint64_t diroff = 0;
            if (Size > 8) {
                memcpy(&diroff, Data + 1, 8);
            }
            // TIFFReadCustomDirectory requires valid TIFFFieldArray
            // We'll pass NULL which should be handled by the library
            TIFFReadCustomDirectory(tif, diroff, NULL);
            break;
        }
            
        case 5:
            TIFFAccessTagMethods(tif);
            break;
    }
    
    // Clean up
    TIFFClose(tif);
    remove("./dummy_file");
    
    return 0;
}