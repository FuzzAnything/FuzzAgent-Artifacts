// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFGetVersion at tif_version.c:28:13 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFWarningExt at tif_warning.c:63:6 in tiffio.h
// TIFFErrorExt at tif_error.c:63:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void dummy_warning_handler(const char* module, const char* fmt, va_list ap) {
    // Do nothing - just consume the warning
}

static void dummy_error_handler(const char* module, const char* fmt, va_list ap) {
    // Do nothing - just consume the error
}

static void dummy_warning_handler_ext(thandle_t client_data, const char* module, const char* fmt, va_list ap) {
    // Do nothing - just consume the warning
}

static void dummy_error_handler_ext(thandle_t client_data, const char* module, const char* fmt, va_list ap) {
    // Do nothing - just consume the error
}

static void dummy_warning_handler_ext_r(TIFF* tif, const char* module, const char* fmt, va_list ap) {
    // Do nothing - just consume the warning
}

int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    // Set up dummy handlers to avoid output pollution
    TIFFSetWarningHandler(dummy_warning_handler);
    TIFFSetErrorHandler(dummy_error_handler);
    TIFFSetWarningHandlerExt(dummy_warning_handler_ext);
    TIFFSetErrorHandlerExt(dummy_error_handler_ext);
    
    // Always call TIFFGetVersion - it's safe and returns constant string
    const char* version = TIFFGetVersion();
    (void)version; // Use version to avoid unused variable warning
    
    // Prepare module and format strings from fuzzer input
    // Use first half for module, second half for format
    size_t module_len = Size / 2;
    size_t fmt_len = Size - module_len;
    
    char* module = NULL;
    char* fmt = NULL;
    
    if (module_len > 0) {
        module = (char*)malloc(module_len + 1);
        if (module) {
            memcpy(module, Data, module_len);
            module[module_len] = '\0';
        }
    }
    
    if (fmt_len > 0) {
        fmt = (char*)malloc(fmt_len + 1);
        if (fmt) {
            memcpy(fmt, Data + module_len, fmt_len);
            fmt[fmt_len] = '\0';
        }
    }
    
    // If we have both module and format strings, test the functions
    if (module && fmt) {
        // Test TIFFWarning
        TIFFWarning(module, fmt);
        
        // Test TIFFError
        TIFFError(module, fmt);
        
        // Test TIFFWarningExt and TIFFErrorExt with dummy thandle_t
        thandle_t dummy_handle = (thandle_t)module; // Just use module pointer as dummy handle
        TIFFWarningExt(dummy_handle, module, fmt);
        TIFFErrorExt(dummy_handle, module, fmt);
        
        // Test TIFFWarningExtR with dummy TIFF structure
        // First create a dummy TIFF file to get a TIFF* handle
        FILE* dummy_file = fopen("./dummy_file", "wb");
        if (dummy_file) {
            // Write some minimal TIFF header data
            uint8_t header[] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
            fwrite(header, 1, sizeof(header), dummy_file);
            fclose(dummy_file);
            
            // Try to open the dummy file
            TIFF* tif = TIFFOpen("./dummy_file", "r");
            if (tif) {
                // Test TIFFWarningExtR with the opened TIFF
                TIFFWarningExtR(tif, module, fmt);
                TIFFClose(tif);
            }
            
            // Clean up dummy file
            remove("./dummy_file");
        }
    }
    
    // Clean up
    if (module) free(module);
    if (fmt) free(fmt);
    
    // Also test with NULL pointers and empty strings
    TIFFWarning(NULL, NULL);
    TIFFError(NULL, NULL);
    TIFFWarning(NULL, "");
    TIFFError("", NULL);
    
    // Test with various combinations
    if (Size > 0) {
        // Use single byte as simple format string
        char simple_fmt[2] = {Data[0], '\0'};
        TIFFWarning("test_module", simple_fmt);
        TIFFError("test_module", simple_fmt);
    }
    
    return 0;
}