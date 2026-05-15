// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFErrorExt at tif_error.c:63:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFGetVersion at tif_version.c:28:13 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFWarningExt at tif_warning.c:63:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "libtiff/tiffio.h"

static void dummy_warning_handler(const char *module, const char *fmt, va_list ap) {
    // Do nothing - just consume the warning
}

static void dummy_warning_handler_ext(thandle_t client_data, const char *module, const char *fmt, va_list ap) {
    // Do nothing - just consume the warning
}

static void dummy_error_handler(const char *module, const char *fmt, va_list ap) {
    // Do nothing - just consume the error
}

static void dummy_error_handler_ext(thandle_t client_data, const char *module, const char *fmt, va_list ap) {
    // Do nothing - just consume the error
}

static void dummy_warning_handler_ext_r(TIFF *tif, const char *module, const char *fmt, va_list ap) {
    // Do nothing - just consume the warning
}

int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Set up dummy handlers to avoid printing to stderr
    TIFFSetWarningHandler(dummy_warning_handler);
    TIFFSetErrorHandler(dummy_error_handler);
    TIFFSetWarningHandlerExt(dummy_warning_handler_ext);
    TIFFSetErrorHandlerExt(dummy_error_handler_ext);

    // Test TIFFGetVersion - always safe to call
    const char *version = TIFFGetVersion();
    if (version) {
        // Just to ensure the call has side effects
        volatile const char *v = version;
        (void)v;
    }

    // Create a dummy TIFF structure for functions that need it
    // We'll create a minimal TIFF file in memory
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    
    // Write minimal TIFF header (little-endian)
    uint8_t header[] = {
        0x49, 0x49, 0x2A, 0x00,  // Little-endian, TIFF magic
        0x08, 0x00, 0x00, 0x00,  // First IFD offset
        0x00, 0x00              // Padding to make it 10 bytes
    };
    fwrite(header, 1, sizeof(header), fp);
    fclose(fp);

    // Try to open the dummy file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    
    // Test TIFFWarning
    if (Size > 0) {
        const char *module = "fuzz_module";
        const char *format = "fuzz warning: %.*s";
        TIFFWarning(module, format, (int)(Size > 100 ? 100 : Size), Data);
    }

    // Test TIFFError
    if (Size > 0) {
        const char *module = "fuzz_module";
        const char *format = "fuzz error: %.*s";
        TIFFError(module, format, (int)(Size > 100 ? 100 : Size), Data);
    }

    // Test TIFFWarningExtR if we have a TIFF handle
    if (tif && Size > 0) {
        const char *module = "fuzz_module";
        const char *format = "fuzz warning extR: %.*s";
        TIFFWarningExtR(tif, module, format, (int)(Size > 100 ? 100 : Size), Data);
    }

    // Test TIFFWarningExt
    if (Size > 0) {
        thandle_t dummy_handle = (thandle_t)(void *)tif;  // Use TIFF* as thandle_t if available
        const char *module = "fuzz_module";
        const char *format = "fuzz warning ext: %.*s";
        TIFFWarningExt(dummy_handle, module, format, (int)(Size > 100 ? 100 : Size), Data);
    }

    // Test TIFFErrorExt
    if (Size > 0) {
        thandle_t dummy_handle = (thandle_t)(void *)tif;  // Use TIFF* as thandle_t if available
        const char *module = "fuzz_module";
        const char *format = "fuzz error ext: %.*s";
        TIFFErrorExt(dummy_handle, module, format, (int)(Size > 100 ? 100 : Size), Data);
    }

    // Clean up
    if (tif) {
        TIFFClose(tif);
    }
    
    // Remove dummy file
    remove("./dummy_file");

    return 0;
}