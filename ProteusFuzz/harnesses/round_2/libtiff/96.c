// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFGetVersion at tif_version.c:28:13 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFWarningExt at tif_warning.c:63:6 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFErrorExt at tif_error.c:63:6 in tiffio.h
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

static int dummy_warning_handler_ext(thandle_t clientdata, const char *module, const char *fmt, va_list ap) {
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
    return 0;
}

static int dummy_error_handler_ext(thandle_t clientdata, const char *module, const char *fmt, va_list ap) {
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
    return 0;
}

static void setup_handlers(void) {
    // TIFFSetWarningHandlerExt and TIFFSetErrorHandlerExt take a single argument: the handler function.
    // The function signature expected is int (*)(thandle_t, const char *, const char *, va_list).
    // We cast our dummy functions to match the expected type TIFFErrorHandlerExt (which is used for both).
    TIFFSetWarningHandlerExt((TIFFErrorHandlerExt)dummy_warning_handler_ext);
    TIFFSetErrorHandlerExt((TIFFErrorHandlerExt)dummy_error_handler_ext);
}

int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    setup_handlers();

    // Test TIFFGetVersion
    const char *version = TIFFGetVersion();
    if (version != NULL) {
        (void)strlen(version);
    }

    // Create a dummy file to get a valid TIFF handle for TIFFWarningExtR
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "w+b");
    if (fp == NULL) {
        return 0;
    }

    // Write some data to the file to ensure it's not empty
    size_t write_size = (Size > 1024) ? 1024 : Size;
    fwrite(Data, 1, write_size, fp);
    fclose(fp);

    // Open the file for reading
    TIFF *tif = TIFFOpen(dummy_file, "r");
    thandle_t handle = (thandle_t)0;

    if (tif != NULL) {
        handle = (thandle_t)TIFFClientdata(tif);
    }

    // Test TIFFWarning
    TIFFWarning("fuzz_test", "Test warning from TIFFWarning: %s", "hello");

    // Test TIFFWarningExt
    TIFFWarningExt(handle, "fuzz_test", "Test warning from TIFFWarningExt: %s", "hello");

    if (tif != NULL) {
        // Test TIFFWarningExtR
        TIFFWarningExtR(tif, "fuzz_test", "Test warning from TIFFWarningExtR: %s", "hello");

        // Test TIFFError
        TIFFError("fuzz_test", "Test error from TIFFError: %s", "hello");

        // Test TIFFErrorExt
        TIFFErrorExt(handle, "fuzz_test", "Test error from TIFFErrorExt: %s", "hello");

        TIFFClose(tif);
    }

    // Cleanup dummy file
    remove(dummy_file);

    return 0;
}