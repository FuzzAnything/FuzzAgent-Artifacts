// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFWarningExt at tif_warning.c:63:6 in tiffio.h
// TIFFWarningExt at tif_warning.c:63:6 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFErrorExt at tif_error.c:63:6 in tiffio.h
// TIFFErrorExt at tif_error.c:63:6 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Empty handler to suppress output
}

static void dummy_handler_ext(thandle_t clientdata, const char* module, const char* fmt, va_list ap) {
    // Empty handler to suppress output
}

static void dummy_handler_extR(TIFF* tif, const char* module, const char* fmt, va_list ap) {
    // Empty handler to suppress output
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    // Set custom handlers to suppress output
    TIFFSetWarningHandler(dummy_handler);
    TIFFSetErrorHandler(dummy_handler);
    
    // Prepare module and format strings from fuzz data
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
    
    // Use default strings if allocation failed
    const char* default_module = "fuzz";
    const char* default_fmt = "test %s";
    
    const char* use_module = module ? module : default_module;
    const char* use_fmt = fmt ? fmt : default_fmt;
    
    // Call all target functions with NULL for TIFF* and thandle_t parameters
    // 1. TIFFWarning
    TIFFWarning(use_module, use_fmt, "warning");
    TIFFWarning(use_module, "%s", use_fmt);
    
    // 2. TIFFError
    TIFFError(use_module, use_fmt, "error");
    TIFFError(use_module, "%s", use_fmt);
    
    // 3. TIFFWarningExtR - pass NULL for TIFF*
    TIFFWarningExtR(NULL, use_module, use_fmt, "warning");
    TIFFWarningExtR(NULL, use_module, "%s", use_fmt);
    
    // 4. TIFFWarningExt - pass NULL for thandle_t
    TIFFWarningExt(NULL, use_module, use_fmt, "warning");
    TIFFWarningExt(NULL, use_module, "%s", use_fmt);
    
    // 5. TIFFErrorExtR - pass NULL for TIFF*
    TIFFErrorExtR(NULL, use_module, use_fmt, "error");
    TIFFErrorExtR(NULL, use_module, "%s", use_fmt);
    
    // 6. TIFFErrorExt - pass NULL for thandle_t
    TIFFErrorExt(NULL, use_module, use_fmt, "error");
    TIFFErrorExt(NULL, use_module, "%s", use_fmt);
    
    // Test with empty strings
    TIFFWarning("", "", "");
    TIFFError("", "", "");
    
    // Test with NULL pointers (should be handled by library)
    TIFFWarning(NULL, NULL, NULL);
    TIFFError(NULL, NULL, NULL);
    
    // Cleanup
    if (module) free(module);
    if (fmt) free(fmt);
    
    return 0;
}