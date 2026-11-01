// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFWarningExt at tif_warning.c:63:6 in tiffio.h
// TIFFErrorExt at tif_error.c:63:6 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFGetVersion at tif_version.c:28:13 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFWarningExt at tif_warning.c:63:6 in tiffio.h
// TIFFErrorExt at tif_error.c:63:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetVersion at tif_version.c:28:13 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <tiffio.h>

/* Static helper to create a dummy file for testing file-based operations */
static int create_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (!f) return -1;
    if (size > 0) {
        size_t written = fwrite(data, 1, size, f);
        if (written != size) {
            fclose(f);
            return -1;
        }
    }
    fclose(f);
    return 0;
}

/* Custom warning handler for testing */
static void custom_warning_handler(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

/* Custom warning handler with handle */
static void custom_warning_handler_ext(thandle_t clientdata, const char *module, const char *fmt, va_list ap) {
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

/* Custom error handler for testing */
static void custom_error_handler(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

/* Custom error handler with handle */
static void custom_error_handler_ext(thandle_t clientdata, const char *module, const char *fmt, va_list ap) {
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

/* Extended warning handler returning void (matching TIFFErrorHandlerExtR signature if available, 
   otherwise matching standard handler for compatibility) */
static void custom_warning_handler_ext_r(TIFF *tif, const char *module, const char *fmt, va_list ap) {
    (void)tif;
    (void)module;
    (void)fmt;
    (void)ap;
}

/* Extended error handler returning void */
static void custom_error_handler_ext_r(TIFF *tif, const char *module, const char *fmt, va_list ap) {
    (void)tif;
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    const char *dummy_file = "./dummy_file";
    TIFF *tif = NULL;
    thandle_t handle = NULL;
    const char *version_str;
    const char *module_name = "FuzzTestModule";

    /* 1. Prepare environment */
    /* Create a dummy file with fuzzed data to test file opening logic */
    if (create_dummy_file(dummy_file, Data, Size) != 0) {
        /* If file creation fails, we can still test functions that don't require a file */
        goto test_no_file;
    }

    /* Try to open the file for reading */
    tif = TIFFOpen(dummy_file, "r");

    /* 2. Test API functions */

    /* Test TIFFGetVersion (always safe) */
    version_str = TIFFGetVersion();
    if (version_str != NULL) {
        /* Just access it to ensure no crash */
        (void)version_str[0];
    }

    /* Test TIFFWarning and TIFFError (global handlers) */
    /* These invoke registered handlers. We set them up to avoid default stderr output or null pointer issues if default is null */
    TIFFSetWarningHandler(custom_warning_handler);
    TIFFSetErrorHandler(custom_error_handler);

    TIFFWarning(module_name, "Test warning: %s", "hello");
    TIFFError(module_name, "Test error: %s", "world");

    /* Test TIFFWarningExt and TIFFErrorExt with a handle */
    /* We can use the TIFF handle or a dummy handle. Let's use the TIFF handle if open, else NULL */
    handle = (tif != NULL) ? (thandle_t)tif : NULL;
    
    /* Set extended handlers */
    TIFFSetWarningHandlerExt(custom_warning_handler_ext);
    TIFFSetErrorHandlerExt(custom_error_handler_ext);

    TIFFWarningExt(handle, module_name, "Test warning ext: %s", "data");
    TIFFErrorExt(handle, module_name, "Test error ext: %s", "data");

    /* Test TIFFWarningExtR if TIFF is open */
    if (tif != NULL) {
        /* 
         * Note: TIFFSetWarningHandlerExtR and TIFFSetErrorHandlerExtR are not standard in older libtiff versions
         * or might have different signatures. The provided API list includes TIFFWarningExtR.
         * Since the compiler complained about TIFFSetWarningHandlerExtR, we will not call the setter if it's not available.
         * However, we can still call TIFFWarningExtR directly if the internal handler is set via the standard Ext setters
         * or if the library has a default fallback.
         * To be safe and compile, we only call TIFFWarningExtR which is explicitly listed in the target APIs.
         * We rely on the global handlers set above or internal defaults.
         */
        TIFFWarningExtR(tif, module_name, "Test warning ext R: %s", "data");
    }

    /* 3. Cleanup */
    if (tif != NULL) {
        TIFFClose(tif);
    }

    /* Remove dummy file */
    unlink(dummy_file);

    return 0;

    /* Fallback path if file creation failed */
test_no_file:
    /* Test functions that do not require a file */
    version_str = TIFFGetVersion();
    (void)version_str;

    TIFFSetWarningHandler(custom_warning_handler);
    TIFFSetErrorHandler(custom_error_handler);
    TIFFSetWarningHandlerExt(custom_warning_handler_ext);
    TIFFSetErrorHandlerExt(custom_error_handler_ext);

    TIFFWarning(module_name, "No file warning");
    TIFFError(module_name, "No file error");
    TIFFWarningExt(NULL, module_name, "No file warning ext");
    TIFFErrorExt(NULL, module_name, "No file error ext");

    return 0;
}