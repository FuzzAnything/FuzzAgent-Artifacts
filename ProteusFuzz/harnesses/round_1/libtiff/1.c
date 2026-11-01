// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetErrorHandlerExtR at tif_open.c:121:6 in tiffio.h
// TIFFOpenOptionsSetWarningHandlerExtR at tif_open.c:129:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "tiffio.h"

static int custom_error_handler_ext(TIFF *tif, void *clientdata, const char *module, const char *fmt, va_list ap) {
    (void)tif;
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
    return 0;
}

static int custom_warning_handler_ext(TIFF *tif, void *clientdata, const char *module, const char *fmt, va_list ap) {
    (void)tif;
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
    return 0;
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Write fuzz data to a dummy file
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        return 0;
    }
    size_t written = fwrite(Data, 1, Size, fp);
    fclose(fp);
    if (written != Size) {
        return 0;
    }

    // 1. Allocate options
    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (opts == NULL) {
        return 0;
    }

    // 2. Set error handler
    TIFFOpenOptionsSetErrorHandlerExtR(opts, custom_error_handler_ext, NULL);

    // 3. Set warning handler
    TIFFOpenOptionsSetWarningHandlerExtR(opts, custom_warning_handler_ext, NULL);

    // 4. Open file
    TIFF *tif = TIFFOpenExt(filename, "r", opts);

    // 5. Free options (even if open failed, we must free)
    TIFFOpenOptionsFree(opts);

    if (tif != NULL) {
        // 6. Invoke error handler
        TIFFErrorExtR(tif, "Fuzzer", "Test error message: %s", "fuzz");

        // 7. Invoke warning handler
        TIFFWarningExtR(tif, "Fuzzer", "Test warning message: %s", "fuzz");

        // 8. Close file
        TIFFClose(tif);
    }

    // Cleanup dummy file
    remove(filename);

    return 0;
}