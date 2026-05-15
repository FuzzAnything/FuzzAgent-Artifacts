// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetWarningHandlerExtR at tif_open.c:129:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFFileName at tif_open.c:798:13 in tiffio.h
// TIFFSetFileName at tif_open.c:803:13 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "libtiff/tiffio.h"

static int dummy_error_handler(TIFF* tif, void* user_data, const char* module, const char* fmt, va_list ap) {
    return 0;
}

static int dummy_warning_handler(TIFF* tif, void* user_data, const char* module, const char* fmt, va_list ap) {
    return 0;
}

int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    // Write data to dummy file
    FILE* f = fopen("./dummy_file", "wb");
    if (!f) return 0;
    fwrite(Data, 1, Size, f);
    fclose(f);
    
    // Allocate TIFFOpenOptions on heap since it's opaque
    TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        remove("./dummy_file");
        return 0;
    }
    
    // Set warning handler
    TIFFOpenOptionsSetWarningHandlerExtR(opts, dummy_warning_handler, NULL);
    
    // Try to open the file with different modes
    const char* modes[] = {"r", "w", "r+", "w+"};
    for (size_t i = 0; i < sizeof(modes)/sizeof(modes[0]); i++) {
        TIFF* tif = TIFFOpenExt("./dummy_file", modes[i], opts);
        if (tif) {
            // Test TIFFFileName
            const char* filename = TIFFFileName(tif);
            
            // Test TIFFSetFileName with various strings
            const char* test_names[] = {"test1.tif", "test2.tiff", "", NULL};
            for (size_t j = 0; j < sizeof(test_names)/sizeof(test_names[0]); j++) {
                const char* prev = TIFFSetFileName(tif, test_names[j]);
                // prev could be NULL or previous name
            }
            
            // Test error and warning functions with various messages
            TIFFErrorExtR(tif, "test_module", "Test error: %s", "fuzzing");
            TIFFWarningExtR(tif, "test_module", "Test warning: %d", 42);
            
            // Test with NULL TIFF pointer
            TIFFErrorExtR(NULL, "test_module", "Test error with NULL TIFF");
            TIFFWarningExtR(NULL, "test_module", "Test warning with NULL TIFF");
            
            // Close the TIFF
            TIFFClose(tif);
        }
    }
    
    // Free the options
    TIFFOpenOptionsFree(opts);
    
    // Test with NULL options
    TIFF* tif2 = TIFFOpenExt("./dummy_file", "r", NULL);
    if (tif2) {
        TIFFClose(tif2);
    }
    
    // Clean up
    remove("./dummy_file");
    
    return 0;
}