// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFWarningExt at tif_warning.c:63:6 in tiffio.h
// TIFFWarningExt at tif_warning.c:63:6 in tiffio.h
// TIFFErrorExt at tif_error.c:63:6 in tiffio.h
// TIFFErrorExt at tif_error.c:63:6 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "libtiff/tiffio.h"

static void dummy_warning_handler(const char* module, const char* fmt, va_list ap) {
    // Do nothing - just consume the warning
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_warning_handler_ext(thandle_t clientdata, const char* module, const char* fmt, va_list ap) {
    // Do nothing - just consume the warning
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_error_handler(const char* module, const char* fmt, va_list ap) {
    // Do nothing - just consume the error
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_error_handler_ext(thandle_t clientdata, const char* module, const char* fmt, va_list ap) {
    // Do nothing - just consume the error
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Create dummy file for TIFF operations
    FILE* dummy = fopen("./dummy_file", "wb");
    if (!dummy) {
        return 0;
    }
    
    // Write some minimal TIFF header data to make TIFFOpen not fail immediately
    // Basic TIFF header (little-endian)
    uint8_t header[] = {
        0x49, 0x49, 0x2A, 0x00,  // Little-endian, magic number
        0x08, 0x00, 0x00, 0x00,  // IFD offset
        0x00, 0x00              // Padding
    };
    fwrite(header, 1, sizeof(header), dummy);
    
    // Write fuzzing data after header
    if (Size > 0) {
        fwrite(Data, 1, Size > 1024 ? 1024 : Size, dummy);
    }
    fclose(dummy);

    // Set dummy handlers to avoid output pollution
    TIFFSetWarningHandler(dummy_warning_handler);
    TIFFSetErrorHandler(dummy_error_handler);
    TIFFSetWarningHandlerExt(dummy_warning_handler_ext);
    TIFFSetErrorHandlerExt(dummy_error_handler_ext);

    // Try to open the dummy file as TIFF
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    
    // Split input data for module and format strings
    size_t half = Size / 2;
    const char* module = (const char*)Data;
    const char* fmt = (const char*)Data + half;
    
    // Ensure null termination for strings
    char* module_copy = malloc(half + 1);
    char* fmt_copy = malloc(Size - half + 1);
    
    if (module_copy && fmt_copy) {
        memcpy(module_copy, module, half);
        module_copy[half] = '\0';
        memcpy(fmt_copy, fmt, Size - half);
        fmt_copy[Size - half] = '\0';
        
        // Test TIFFWarning with various inputs
        TIFFWarning(module_copy, "%s", fmt_copy);
        TIFFWarning("TEST", "%s", module_copy);
        TIFFWarning(module_copy, "test %d %s", 123, fmt_copy);
        
        if (tif) {
            // Test TIFFWarningExtR
            TIFFWarningExtR(tif, module_copy, "%s", fmt_copy);
            TIFFWarningExtR(tif, "MODULE", "format %s", module_copy);
            
            // Test TIFFErrorExtR
            TIFFErrorExtR(tif, module_copy, "%s", fmt_copy);
            TIFFErrorExtR(tif, "ERROR", "error: %s", module_copy);
        }
        
        // Test TIFFWarningExt
        TIFFWarningExt((thandle_t)tif, module_copy, "%s", fmt_copy);
        TIFFWarningExt((thandle_t)0x1, "EXT", "warning: %s", fmt_copy);
        
        // Test TIFFErrorExt
        TIFFErrorExt((thandle_t)tif, module_copy, "%s", fmt_copy);
        TIFFErrorExt((thandle_t)0x2, "ERR", "error %d: %s", 456, fmt_copy);
        
        // Test TIFFError
        TIFFError(module_copy, "%s", fmt_copy);
        TIFFError("TIFF", "error with %s", module_copy);
        
        free(module_copy);
        free(fmt_copy);
    } else {
        // Clean up allocations if either malloc failed
        free(module_copy);
        free(fmt_copy);
    }
    
    if (tif) {
        TIFFClose(tif);
    }
    
    // Clean up
    remove("./dummy_file");
    
    return 0;
}