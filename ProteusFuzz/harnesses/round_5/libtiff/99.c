// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFGetVersion at tif_version.c:28:13 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFWarningExt at tif_warning.c:63:6 in tiffio.h
// TIFFErrorExt at tif_error.c:63:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <tiffio.h>

static void dummy_warning_handler(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_error_handler(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_warning_handler_ext(thandle_t clientdata, const char *module, const char *fmt, va_list ap) {
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_error_handler_ext(thandle_t clientdata, const char *module, const char *fmt, va_list ap) {
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

// TIFFSetErrorHandlerExtR is not a standard exported API in tiffio.h for setting handlers.
// The target function TIFFErrorExtR uses the handler set via TIFFSetErrorHandlerExtR internally
// or relies on the global handler if the specific one isn't set.
// Since we cannot call a non-existent setter, we rely on the global handler setup
// and the fact that TIFFErrorExtR will invoke the chain.
// We define a dummy function that matches the signature expected by the internal logic
// if we were to set it, but since we can't set it via a public API, we just call TIFFErrorExtR
// which will fall back to the global handlers we set earlier.
// Note: In some versions of libtiff, TIFFSetErrorHandlerExtR exists as an internal or specific
// function, but it's not in the standard header. We will skip setting it and rely on the
// global handler which TIFFErrorExtR also checks.

int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file to potentially open if needed
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Setup custom handlers to ensure the functions actually do something
    TIFFSetWarningHandlerExt(dummy_warning_handler_ext);
    TIFFSetErrorHandlerExt(dummy_error_handler_ext);
    TIFFSetWarningHandler(dummy_warning_handler);
    TIFFSetErrorHandler(dummy_error_handler);

    // 1. Test TIFFGetVersion (No args, always succeeds)
    const char *ver = TIFFGetVersion();
    if (ver) {
        (void)strlen(ver);
    }

    // 2. Test TIFFWarning
    TIFFWarning("FuzzModule", "Warning test");

    // 3. Test TIFFError
    TIFFError("FuzzModule", "Error test");

    // 4. Test TIFFWarningExt
    thandle_t dummy_handle = (thandle_t)Data;
    TIFFWarningExt(dummy_handle, "FuzzModule", "Warning Ext test");

    // 5. Test TIFFErrorExt
    TIFFErrorExt(dummy_handle, "FuzzModule", "Error Ext test");

    // 6. Test TIFFErrorExtR
    TIFF *tif = NULL;
    tif = TIFFOpen("./dummy_file", "r");

    if (tif) {
        // TIFFErrorExtR will use the global handlers if no specific one is set on the TIFF struct
        // via the internal mechanism (which we can't access directly via a public setter in this context).
        // The function signature expects a TIFF*, module, and format.
        TIFFErrorExtR(tif, "FuzzModule", "Error Ext R test");

        TIFFClose(tif);
    }

    return 0;
}