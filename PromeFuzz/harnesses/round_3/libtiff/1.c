// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetErrorHandlerExtR at tif_open.c:121:6 in tiffio.h
// TIFFOpenOptionsSetWarningHandlerExtR at tif_open.c:129:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdarg.h>

static int errorHandlerExtR(TIFF* tif, void* user_data, const char* module, const char* fmt, va_list ap) {
    (void)tif;
    (void)user_data;
    (void)module;
    (void)fmt;
    (void)ap;
    return 0;
}

static int warningHandlerExtR(TIFF* tif, void* user_data, const char* module, const char* fmt, va_list ap) {
    (void)tif;
    (void)user_data;
    (void)module;
    (void)fmt;
    (void)ap;
    return 0;
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    // Write input data to dummy file
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Allocate options
    TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        remove("./dummy_file");
        return 0;
    }
    
    // Set error handler
    TIFFOpenOptionsSetErrorHandlerExtR(opts, errorHandlerExtR, NULL);
    
    // Set warning handler
    TIFFOpenOptionsSetWarningHandlerExtR(opts, warningHandlerExtR, NULL);
    
    // Open TIFF file
    TIFF* tif = TIFFOpenExt("./dummy_file", "r", opts);
    
    // Free options
    TIFFOpenOptionsFree(opts);
    
    // Call error and warning functions with various inputs
    if (tif) {
        TIFFErrorExtR(tif, "test_module", "Test error message: %d", 42);
        TIFFWarningExtR(tif, "test_module", "Test warning message: %s", "fuzzing");
        
        // Close TIFF if opened successfully
        TIFFClose(tif);
    } else {
        // Call with NULL TIFF pointer
        TIFFErrorExtR(NULL, "test_module", "Failed to open TIFF");
        TIFFWarningExtR(NULL, "test_module", "TIFF open failed");
    }
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}