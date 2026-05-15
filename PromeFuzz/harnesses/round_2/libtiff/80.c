// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWarningExt at tif_warning.c:63:6 in tiffio.h
// TIFFErrorExt at tif_error.c:63:6 in tiffio.h
// TIFFErrorExt at tif_error.c:63:6 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetVersion at tif_version.c:28:13 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFWarningExt at tif_warning.c:63:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void dummy_warning_handler(const char* module, const char* fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_warning_handler_ext(thandle_t client_data, const char* module, const char* fmt, va_list ap) {
    (void)client_data;
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_error_handler(const char* module, const char* fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_error_handler_ext(thandle_t client_data, const char* module, const char* fmt, va_list ap) {
    (void)client_data;
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_error_handler_ext_r(TIFF* tif, const char* module, const char* fmt, va_list ap) {
    (void)tif;
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create dummy file
    FILE* dummy = fopen("./dummy_file", "wb");
    if (!dummy) return 0;
    fwrite(Data, 1, Size, dummy);
    fclose(dummy);
    
    // Try to open as TIFF
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    
    // Test TIFFGetVersion
    const char* version = TIFFGetVersion();
    (void)version;
    
    // Set custom handlers to avoid output during fuzzing
    TIFFSetWarningHandler(dummy_warning_handler);
    TIFFSetErrorHandler(dummy_error_handler);
    TIFFSetWarningHandlerExt(dummy_warning_handler_ext);
    TIFFSetErrorHandlerExt(dummy_error_handler_ext);
    
    // Prepare test data
    char module[32];
    char message[64];
    int int_arg = (int)Size;
    const char* str_arg = (const char*)Data;
    
    // Ensure null termination for strings
    if (Size > 31) {
        memcpy(module, Data, 31);
        module[31] = '\0';
    } else {
        memcpy(module, Data, Size);
        module[Size] = '\0';
    }
    
    if (Size > 63) {
        memcpy(message, Data, 63);
        message[63] = '\0';
    } else {
        memcpy(message, Data, Size);
        message[Size] = '\0';
    }
    
    // Test TIFFWarning
    TIFFWarning(module, "Warning: %s (size: %d)", message, int_arg);
    
    // Test TIFFError
    TIFFError(module, "Error: %s (size: %d)", message, int_arg);
    
    // Test TIFFWarningExt
    if (tif) {
        TIFFWarningExt((thandle_t)tif, module, "WarningExt: %s (size: %d)", message, int_arg);
    } else {
        TIFFWarningExt(NULL, module, "WarningExt: %s (size: %d)", message, int_arg);
    }
    
    // Test TIFFErrorExt
    if (tif) {
        TIFFErrorExt((thandle_t)tif, module, "ErrorExt: %s (size: %d)", message, int_arg);
    } else {
        TIFFErrorExt(NULL, module, "ErrorExt: %s (size: %d)", message, int_arg);
    }
    
    // Test TIFFErrorExtR
    if (tif) {
        TIFFErrorExtR(tif, module, "ErrorExtR: %s (size: %d)", message, int_arg);
    }
    
    // Clean up
    if (tif) {
        TIFFClose(tif);
    }
    
    remove("./dummy_file");
    
    return 0;
}