// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFErrorExt at tif_error.c:63:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFWarningExt at tif_warning.c:63:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <tiffio.h>

/* Static helper to create a dummy file for testing */
static int create_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (!f) return 0;
    if (size > 0 && data != NULL) {
        size_t written = fwrite(data, 1, size, f);
        if (written != size) {
            fclose(f);
            return 0;
        }
    } else {
        /* Write a minimal valid TIFF header if no data provided */
        uint16_t header[8] = {0x4949, 42, 8, 0, 0, 0, 0, 0}; /* II, magic, offset */
        fwrite(header, 1, sizeof(header), f);
    }
    fclose(f);
    return 1;
}

/* Static custom warning handler */
static void static_warning_handler(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

/* Static custom warning handler with handle */
static void static_warning_handler_ext(thandle_t clientdata, const char *module, const char *fmt, va_list ap) {
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

/* Static custom error handler */
static void static_error_handler(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

/* Static custom error handler with handle */
static void static_error_handler_ext(thandle_t clientdata, const char *module, const char *fmt, va_list ap) {
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

/* Static custom error handler for TIFF instance (R) */
static int static_error_handler_ext_r(TIFF *tif, const char *module, const char *fmt, va_list ap) {
    (void)tif;
    (void)module;
    (void)fmt;
    (void)ap;
    return 0;
}

/* Static custom warning handler for TIFF instance (R) */
static int static_warning_handler_ext_r(TIFF *tif, const char *module, const char *fmt, va_list ap) {
    (void)tif;
    (void)module;
    (void)fmt;
    (void)ap;
    return 0;
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    const char *dummy_file = "./dummy_file";
    
    /* Step 1: Prepare environment */
    /* Register custom handlers to ensure functions have something to call */
    TIFFSetWarningHandler(static_warning_handler);
    TIFFSetWarningHandlerExt(static_warning_handler_ext);
    TIFFSetErrorHandler(static_error_handler);
    TIFFSetErrorHandlerExt(static_error_handler_ext);

    /* Create a dummy file to potentially open a TIFF instance */
    if (!create_dummy_file(dummy_file, Data, Size)) {
        return 0;
    }

    /* Step 2 & 3: Invoke target functions diversely */

    /* 1. TIFFWarning */
    TIFFWarning("FuzzTest", "Warning: Input size %zu", Size);
    TIFFWarning("FuzzTest", "Warning: Data pointer %p", (void*)Data);

    /* 2. TIFFWarningExt */
    TIFFWarningExt(NULL, "FuzzTest", "WarningExt: Null handle, size %zu", Size);
    
    /* 3. TIFFError */
    TIFFError("FuzzTest", "Error: Input size %zu", Size);
    TIFFError("FuzzTest", "Error: Data pointer %p", (void*)Data);

    /* 4. TIFFErrorExt */
    TIFFErrorExt(NULL, "FuzzTest", "ErrorExt: Null handle, size %zu", Size);

    /* 5 & 6: TIFFWarningExtR and TIFFErrorExtR require a valid TIFF* instance */
    TIFF *tif = TIFFOpen(dummy_file, "r");
    if (tif != NULL) {
        /* Note: libtiff does not provide public setters for per-file handlers 
           in all versions, or they might be internal. We rely on the global
           handlers or direct invocation if the API allows. 
           Since TIFFSetWarningExtR/TIFFSetErrorExtR are not found in standard tiffio.h,
           we skip setting them per-file and rely on the global handlers 
           or the fact that the R functions might check internal state.
           However, the R functions are designed to be called with a TIFF*.
           We will call them directly. The behavior depends on internal handler registration.
           If the library expects per-file handlers to be set via internal APIs,
           these calls might just fall back to global handlers or do nothing.
           To strictly follow the task of fuzzing the API, we call the functions.
        */
        
        /* Invoke R variants */
        TIFFWarningExtR(tif, "FuzzTest", "WarningExtR: Valid handle, size %zu", Size);
        TIFFErrorExtR(tif, "FuzzTest", "ErrorExtR: Valid handle, size %zu", Size);

        /* Cleanup TIFF instance */
        TIFFClose(tif);
    }

    /* Additional exploration: Open in write mode if possible */
    tif = TIFFOpen(dummy_file, "w");
    if (tif != NULL) {
        /* Invoke R variants in write mode */
        TIFFWarningExtR(tif, "FuzzTest", "WarningExtR: Write mode", 0);
        TIFFErrorExtR(tif, "FuzzTest", "ErrorExtR: Write mode", 0);
        
        TIFFClose(tif);
    }

    /* Step 4: Cleanup */
    /* Remove the dummy file */
    remove(dummy_file);

    return 0;
}