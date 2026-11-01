// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFWarningExt at tif_warning.c:63:6 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFGetVersion at tif_version.c:28:13 in tiffio.h
// TIFFErrorExt at tif_error.c:63:6 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "tiffio.h"

/*
 * Fuzz driver for libtiff error and warning handling functions.
 * Targets: TIFFError, TIFFWarningExt, TIFFWarningExtR, TIFFGetVersion,
 *          TIFFErrorExt, TIFFWarning.
 */

/* Custom error handler to prevent crashes and log to stderr */
static void custom_error_handler(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

/* Custom extended error handler */
static void custom_error_handler_ext(thandle_t clientdata, const char *module, const char *fmt, va_list ap) {
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

/* Custom warning handler */
static void custom_warning_handler(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

/* Custom extended warning handler */
static void custom_warning_handler_ext(thandle_t clientdata, const char *module, const char *fmt, va_list ap) {
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

/* Custom extended warning handler for TIFFWarningExtR (returns bool to control propagation) */
static int custom_warning_handler_ext_r(TIFF *tif, const char *module, const char *fmt, va_list ap) {
    (void)tif;
    (void)module;
    (void)fmt;
    (void)ap;
    return 0;
}

/* Helper to safely extract a null-terminated string from fuzz input */
static char *get_safe_string(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) {
        return NULL;
    }
    size_t str_len = 0;
    while (*offset + str_len < size && data[*offset + str_len] != 0 && str_len < 256) {
        str_len++;
    }
    if (str_len == 0) {
        *offset = size; 
        return NULL;
    }
    char *str = malloc(str_len + 1);
    if (!str) return NULL;
    memcpy(str, data + *offset, str_len);
    str[str_len] = '\0';
    *offset += str_len + 1;
    return str;
}

int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Setup custom handlers to ensure functions have something to call */
    TIFFSetErrorHandler(custom_error_handler);
    TIFFSetErrorHandlerExt(custom_error_handler_ext);
    TIFFSetWarningHandler(custom_warning_handler);
    TIFFSetWarningHandlerExt(custom_warning_handler_ext);

    /* Create a dummy file to potentially open a TIFF handle if needed */
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        /* Write some dummy data to ensure file exists */
        size_t write_size = Size > 1024 ? 1024 : Size;
        fwrite(Data, 1, write_size, f);
        fclose(f);
    }

    /* Open a TIFF file to test TIFFWarningExtR which requires a TIFF* handle */
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    
    size_t offset = 0;
    char *module = get_safe_string(Data, Size, &offset);
    char *format_str = get_safe_string(Data, Size, &offset);

    const char *safe_module = module ? module : "dummy_module";
    const char *safe_format = format_str ? format_str : "Test %d";

    /* 1. Test TIFFError */
    TIFFError(safe_module, safe_format, 123);

    /* 2. Test TIFFWarningExt (requires thandle_t) */
    thandle_t dummy_handle = (thandle_t)0x12345; /* Dummy handle */
    TIFFWarningExt(dummy_handle, safe_module, safe_format, 456);

    /* 3. Test TIFFWarningExtR (requires TIFF*) */
    if (tif) {
        TIFFWarningExtR(tif, safe_module, safe_format, 789);
    }

    /* 4. Test TIFFGetVersion */
    const char *version = TIFFGetVersion();
    (void)version;

    /* 5. Test TIFFErrorExt */
    TIFFErrorExt(dummy_handle, safe_module, safe_format, 999);

    /* 6. Test TIFFWarning */
    TIFFWarning(safe_module, safe_format, 111);

    /* Cleanup */
    if (tif) {
        TIFFClose(tif);
    }

    /* Remove dummy file */
    remove("./dummy_file");

    /* Free allocated strings */
    if (module) free(module);
    if (format_str) free(format_str);

    return 0;
}